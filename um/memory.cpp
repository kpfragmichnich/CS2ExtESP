#include "memory.h"

Memory::Memory(const wchar_t* processName) {

    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry)) {
        do {
            if (!_wcsicmp(entry.szExeFile, processName)) {

                processId = entry.th32ProcessID;
                processHandle = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processId);
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }

    if (snapshot)
        CloseHandle(snapshot);
}

Memory::~Memory() {
    if (processHandle)
        CloseHandle(processHandle);
}

uintptr_t Memory::GetModuleAddress(const wchar_t* moduleName) {

    MODULEENTRY32 entry;
    entry.dwSize = sizeof(MODULEENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, processId);

    uintptr_t baseAddress = 0;

    if (Module32First(snapshot, &entry)) {
        do {
            if (!_wcsicmp(entry.szModule, moduleName)) {

                baseAddress = (uintptr_t)entry.modBaseAddr;
                break;
            }
        } while (Module32Next(snapshot, &entry));
    }

    if (snapshot)
        CloseHandle(snapshot);

    return baseAddress;
}

Memory* mem = nullptr;
