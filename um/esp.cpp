#include "esp.h"

void ESP::Update() {
    std::vector<Entity> tempEntities;

    uintptr_t clientBase = mem->GetModuleAddress(L"client.dll");
    if (!clientBase) return;

    // CS2: dwEntityList can be the struct address or a pointer to it.
    uintptr_t entityList = clientBase + LunixOffsets::dwEntityList;
    
    // Check if it's a pointer
    if (mem->Read<uintptr_t>(entityList + 16) == 0) {
        uintptr_t potentialPtr = mem->Read<uintptr_t>(entityList);
        if (potentialPtr) entityList = potentialPtr;
    }
    
    uintptr_t localPlayerController = mem->Read<uintptr_t>(clientBase + LunixOffsets::dwLocalPlayerController);
    uint8_t localTeam = mem->Read<uint8_t>(localPlayerController + LunixOffsets::m_iTeamNum);

    bool localFound = false;

    for (int i = 1; i < 512; i++) {
        uintptr_t listEntry = mem->Read<uintptr_t>(entityList + (8 * (i & 0x7FFF) >> 9) + 16);
        if (!listEntry) continue;

        uintptr_t controller = mem->Read<uintptr_t>(listEntry + 112 * (i & 0x1FF));

        if (!controller) continue;

        if (controller == localPlayerController) {
            localFound = true;
            continue; 
        }

        uint32_t pawnHandle = mem->Read<uint32_t>(controller + LunixOffsets::m_hPawn);
        if (!pawnHandle) continue;

        uintptr_t listEntry2 = mem->Read<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 16);
        if (!listEntry2) continue;

        uintptr_t pawn = mem->Read<uintptr_t>(listEntry2 + 112 * (pawnHandle & 0x1FF));

        if (!pawn) continue;

        int health = mem->Read<int>(pawn + LunixOffsets::m_iHealth);
        if (health <= 0 || health > 100) continue;

        uint8_t team = mem->Read<uint8_t>(pawn + LunixOffsets::m_iTeamNum);
        Vector3 origin = mem->Read<Vector3>(pawn + LunixOffsets::m_vOldOrigin);
        
        Vector3 head = origin;
        head.z += 75.0f; // Simple head pos

        tempEntities.push_back({ pawn, (int)health, (int)team, origin, head });
    }

    // Console Logging
    static auto lastLogged = GetTickCount64();
    if (GetTickCount64() - lastLogged > 1000) { 
        system("cls");
        std::cout << "[Lunix CS2] Debug Info:" << std::endl;
        std::cout << "  - ClientBase: " << (void*)clientBase << std::endl;
        std::cout << "  - EntityList Struct/Base: " << (void*)entityList << std::endl;
        std::cout << "  - LocalController: " << (void*)localPlayerController << std::endl;
        std::cout << "  - Local found in list: " << (localFound ? "YES" : "NO") << std::endl;
        std::cout << "  - Entities found total: " << tempEntities.size() << std::endl;
        
        for (int i = 0; i < (tempEntities.size() > 5 ? 5 : (int)tempEntities.size()); i++) {
            const auto& ent = tempEntities[i];
            std::cout << "    * Pawn: " << (void*)ent.pawn << " | HP: " << ent.health << " | T: " << ent.team << std::endl;
        }
        lastLogged = GetTickCount64();
    }

    entities = tempEntities;
}




ESP* esp = new ESP();
