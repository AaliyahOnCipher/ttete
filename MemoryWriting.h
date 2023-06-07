#pragma once

#include <Windows.h>
#include <cstdint>
#include <vector>

namespace MemoryWriting
{
    // Read a value from memory at the specified address with an offset
    template<typename T>
    T ReadMemory(HANDLE processHandle, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets)
    {
        uintptr_t address = baseAddress;

        // Iterate through the offsets to calculate the final address
        for (const auto& offset : offsets)
        {
            ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &address, sizeof(address), nullptr);
            address += offset;
        }

        T value;
        ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &value, sizeof(value), nullptr);
        return value;
    }

    // Write a value to memory at the specified address with an offset
    template<typename T>
    void WriteMemory(HANDLE processHandle, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const T& value)
    {
        uintptr_t address = baseAddress;

        // Iterate through the offsets to calculate the final address
        for (size_t i = 0; i < offsets.size() - 1; i++)
        {
            ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &address, sizeof(address), nullptr);
            address += offsets[i];
        }

        // Write the value to memory
        WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address + offsets.back()), &value, sizeof(value), nullptr);
    }

    // Replace bytes in memory at the specified address with an offset
    void ReplaceBytes(HANDLE processHandle, uintptr_t baseAddress, const std::vector<uintptr_t>& offsets, const std::vector<uint8_t>& originalBytes, const std::vector<uint8_t>& newBytes)
    {
        uintptr_t address = baseAddress;

        // Iterate through the offsets to calculate the final address
        for (const auto& offset : offsets)
        {
            ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), &address, sizeof(address), nullptr);
            address += offset;
        }

        // Verify the original bytes before replacing
        std::vector<uint8_t> buffer(originalBytes.size());
        ReadProcessMemory(processHandle, reinterpret_cast<LPCVOID>(address), buffer.data(), buffer.size(), nullptr);
        if (buffer == originalBytes)
        {
            // Write the new bytes to memory
            WriteProcessMemory(processHandle, reinterpret_cast<LPVOID>(address), newBytes.data(), newBytes.size(), nullptr);
        }
    }
}
