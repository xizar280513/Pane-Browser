#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <string>
#include <vector>
#include "resource_v2.h"

static std::wstring TempFolder() {
    wchar_t base[MAX_PATH] = {};
    DWORD n = GetTempPathW(MAX_PATH, base);
    std::wstring root = n ? std::wstring(base, n) : L".";
    root += L"PaneBrowser-v2-" + std::to_wstring(GetCurrentProcessId());
    CreateDirectoryW(root.c_str(), nullptr);
    return root;
}

static bool ExtractResource(int id, const std::wstring& path) {
    HRSRC res = FindResourceW(nullptr, MAKEINTRESOURCEW(id), RT_RCDATA);
    if (!res) return false;
    HGLOBAL loaded = LoadResource(nullptr, res);
    DWORD size = SizeofResource(nullptr, res);
    void* data = LockResource(loaded);
    if (!loaded || !data || !size) return false;
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0;
    bool ok = WriteFile(file, data, size, &written, nullptr) && written == size;
    CloseHandle(file);
    return ok;
}

static void RemoveTree(const std::wstring& folder) {
    WIN32_FIND_DATAW fd{};
    HANDLE find = FindFirstFileW((folder + L"\\*").c_str(), &fd);
    if (find == INVALID_HANDLE_VALUE) { RemoveDirectoryW(folder.c_str()); return; }
    do {
        if (wcscmp(fd.cFileName, L".") == 0 || wcscmp(fd.cFileName, L"..") == 0) continue;
        std::wstring p = folder + L"\\" + fd.cFileName;
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) RemoveTree(p);
        else { SetFileAttributesW(p.c_str(), FILE_ATTRIBUTE_NORMAL); DeleteFileW(p.c_str()); }
    } while (FindNextFileW(find, &fd));
    FindClose(find);
    RemoveDirectoryW(folder.c_str());
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    std::wstring folder = TempFolder();
    std::wstring core = folder + L"\\PaneBrowserCore.exe";
    std::wstring loader = folder + L"\\WebView2Loader.dll";
    if (!ExtractResource(IDR_PANE_BROWSER_CORE, core) || !ExtractResource(IDR_WEBVIEW2_LOADER, loader)) {
        MessageBoxW(nullptr, L"Pane Browser could not unpack its embedded runtime files.", L"Pane Browser", MB_OK | MB_ICONERROR);
        RemoveTree(folder);
        return 2;
    }
    STARTUPINFOW si{}; si.cb = sizeof(si);
    PROCESS_INFORMATION pi{};
    std::wstring command = L"\"" + core + L"\"";
    BOOL started = CreateProcessW(core.c_str(), command.data(), nullptr, nullptr, FALSE, 0, nullptr, folder.c_str(), &si, &pi);
    if (!started) {
        MessageBoxW(nullptr, L"Pane Browser could not start.", L"Pane Browser", MB_OK | MB_ICONERROR);
        RemoveTree(folder);
        return 3;
    }
    CloseHandle(pi.hThread);
    WaitForSingleObject(pi.hProcess, INFINITE);
    CloseHandle(pi.hProcess);
    RemoveTree(folder);
    return 0;
}
