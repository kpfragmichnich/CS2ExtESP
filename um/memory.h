#pragma once
#include <windows.h>
#include <vector>
#include <iostream>
#include <TlHelp32.h>

class Memory {
public:
    Memory(const wchar_t* processName);

    ~Memory();

    uintptr_t GetModuleAddress(const wchar_t* moduleName);


    template <typename T>
    T Read(uintptr_t address) {
        T value = { 0 };
        ReadProcessMemory(processHandle, (LPCVOID)address, &value, sizeof(T), NULL);
        return value;
    }


    template <typename T>
    bool Write(uintptr_t address, T value) {
        return WriteProcessMemory(processHandle, (LPVOID)address, &value, sizeof(T), NULL);
    }

    bool ReadRaw(uintptr_t address, void* buffer, size_t size) {
        return ReadProcessMemory(processHandle, (LPCVOID)address, buffer, size, NULL);
    }

    HANDLE GetProcessHandle() { return processHandle; }
    DWORD GetProcessId() { return processId; }

private:
    DWORD processId = 0;
    HANDLE processHandle = NULL;
};

extern Memory* mem;
