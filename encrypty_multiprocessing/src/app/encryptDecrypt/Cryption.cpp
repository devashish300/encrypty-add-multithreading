#include "Cryption.hpp"
#include "../processes/Task.hpp"
#include "../fileHandling/ReadEnv.cpp"
#include <ctime>
#include <iomanip>
#include <cstdint>

#ifndef _WIN32
#error "This build simplifies to Windows-only memory-mapped I/O. Build on Windows or add a POSIX path."
#endif

#define NOMINMAX
#include <windows.h>

static void cryptBuffer(uint8_t* data, size_t size, int key, bool encrypt) {
    key = key%256;
    if (encrypt) {
        for (size_t i = 0; i < size; ++i)
            data[i] = static_cast<uint8_t>((data[i] + key) & 0xFF);
    } else {
        for (size_t i = 0; i < size; ++i)
            data[i] = static_cast<uint8_t>((data[i] - key) & 0xFF);
    }
}

int executeCryption(const std::string& taskData) {
    Task task = Task::fromString(taskData);
    ReadEnv env;
    std::string envKey = env.getenv();
    int key = std::stoi(envKey);
    bool encrypt = (task.action == Action::ENCRYPT);

    std::wstring wpath(task.filePath.begin(), task.filePath.end());
    HANDLE hFile = CreateFileW(wpath.c_str(), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to open file: " << task.filePath << std::endl;
        return 1;
    }
    LARGE_INTEGER liSize{};
    if (!GetFileSizeEx(hFile, &liSize) || liSize.QuadPart == 0) {
        CloseHandle(hFile);
        return 0;
    }
    HANDLE hMap = CreateFileMappingW(hFile, nullptr, PAGE_READWRITE, liSize.HighPart, liSize.LowPart, nullptr);
    if (!hMap) {
        std::cerr << "CreateFileMapping failed: " << task.filePath << std::endl;
        CloseHandle(hFile);
        return 1;
    }
    uint8_t* view = reinterpret_cast<uint8_t*>(MapViewOfFile(hMap, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    if (!view) {
        std::cerr << "MapViewOfFile failed: " << task.filePath << std::endl;
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }
    cryptBuffer(view, static_cast<size_t>(liSize.QuadPart), key, encrypt);
    FlushViewOfFile(view, 0);
    UnmapViewOfFile(view);
    CloseHandle(hMap);
    CloseHandle(hFile);
    std::time_t t = std::time(nullptr);
    std::tm* now = std::localtime(&t);
    std::cout << "Exiting the encryption/decryption at: " << std::put_time(now, "%Y-%m-%d %H:%M:%S") << std::endl;
    return 0;
}