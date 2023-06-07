#pragma once

#include <Windows.h>
#include <Psapi.h>
#include <string>
#include <vector>

namespace MemoryUtil
{
    // Get the base address of the module in the target process
    uintptr_t GetModuleBaseAddress(HANDLE processHandle, const std::wstring& moduleName)
    {
        HMODULE* moduleHandles;
        DWORD moduleHandleSize;
        EnumProcessModules(processHandle, nullptr, 0, &moduleHandleSize);

        moduleHandles = new HMODULE[moduleHandleSize / sizeof(HMODULE)];
        EnumProcessModules(processHandle, moduleHandles, moduleHandleSize, &moduleHandleSize);

        uintptr_t baseAddress = 0;

        for (DWORD i = 0; i < moduleHandleSize / sizeof(HMODULE); i++)
        {
            wchar_t name[MAX_PATH];
            GetModuleBaseNameW(processHandle, moduleHandles[i], name, sizeof(name));

            if (moduleName.compare(name) == 0)
            {
                baseAddress = reinterpret_cast<uintptr_t>(moduleHandles[i]);
                break;
            }
        }

        delete[] moduleHandles;

        return baseAddress;
    }
}
