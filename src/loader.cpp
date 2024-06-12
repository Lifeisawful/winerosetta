#include <windows.h>
#include <iostream>
#include <filesystem>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Path to the target executable
    const char* exePath = ".\\wow.exe";

    // Path to the DLL we want to inject
    std::filesystem::path dllPath = std::filesystem::absolute(".\\winerosetta.dll");

    std::cout << "Absolute DLL Path: " << dllPath << std::endl;

    // Create a new process
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcess(exePath, NULL, NULL, NULL, FALSE, CREATE_SUSPENDED, NULL, NULL, &si, &pi)) {
        std::cerr << "CreateProcess failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Process created successfully." << std::endl;

    // Allocate memory in the process for the DLL path
    void* remoteDllPath = VirtualAllocEx(pi.hProcess, NULL, dllPath.string().length() + 1, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

    if (remoteDllPath == NULL) {
        std::cerr << "VirtualAllocEx failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "Memory allocated successfully in the process." << std::endl;

    // Write the DLL path into the allocated memory
    if (!WriteProcessMemory(pi.hProcess, remoteDllPath, dllPath.string().c_str(), dllPath.string().length() + 1, NULL)) {
        std::cerr << "WriteProcessMemory failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "DLL path written successfully into the process memory." << std::endl;

    // Load the DLL into the process
    HANDLE hThread = CreateRemoteThread(pi.hProcess, NULL, 0, (LPTHREAD_START_ROUTINE)LoadLibraryA, remoteDllPath, 0, NULL);

    if (hThread == NULL) {
        std::cerr << "CreateRemoteThread failed: " << GetLastError() << std::endl;
        return 1;
    }

    std::cout << "DLL loaded successfully into the process." << std::endl;

    // Wait for the DLL to load
    WaitForSingleObject(hThread, INFINITE);


    // Clean up
    VirtualFreeEx(pi.hProcess, remoteDllPath, 0, MEM_RELEASE);
    CloseHandle(hThread);

    ResumeThread(pi.hThread);

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    std::cout << "Clean up done successfully." << std::endl;

    return 0;
}