#include <iostream>
#include <thread>
#include "../um/memory.h"
#include "../um/esp.h"
#include "../um/render.h"
#include "../um/overlay_hijacker.h"

// Forward declaration
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int main() {
    std::cout << "[Lunix CS2] Waiting for CS2..." << std::endl;
    
    while (!mem) {
        mem = new Memory(L"cs2.exe");
        if (mem->GetProcessId() == 0) {
            delete mem;
            mem = nullptr;
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }

    std::cout << "[Lunix CS2] Attached to CS2 (PID: " << mem->GetProcessId() << ")" << std::endl;

    // Hijack Discord Overlay
    HWND discordHwnd = NULL;
    while (!discordHwnd) {
        discordHwnd = OverlayHijacker::FindDiscordOverlay();
        if (!discordHwnd) {
            std::cout << "[Lunix CS2] Discord overlay not found. Make sure it is enabled." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
        }
    }

    std::cout << "[Lunix CS2] Discord Overlay Hijacked (HWND: " << (void*)discordHwnd << ")" << std::endl;
    OverlayHijacker::PrepareHijackedWindow(discordHwnd);

    if (!renderer->Initialize(discordHwnd)) {
        std::cout << "[Lunix CS2] Failed to initialize renderer on hijacked window." << std::endl;
        return 1;
    }

    std::cout << "[Lunix CS2] ESP Started." << std::endl;

    bool running = true;
    while (running) {
        if (!IsWindow(discordHwnd)) {
            std::cout << "[Lunix CS2] Hijacked window lost." << std::endl;
            break;
        }

        // Update ESP
        esp->Update();

        // Render frame
        renderer->BeginFrame();
        renderer->RenderESP();
        renderer->EndFrame();

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    renderer->Cleanup();
    return 0;
}
