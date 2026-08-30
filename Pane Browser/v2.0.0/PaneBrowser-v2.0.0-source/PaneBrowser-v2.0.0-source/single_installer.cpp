#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <shlobj.h>
#include <string>
#include "resource_v2.h"

static std::wstring Join(const std::wstring& a, const std::wstring& b) { return a + (a.empty() || a.back() == L'\\' ? L"" : L"\\") + b; }
static bool ExtractResource(int id, const std::wstring& path) {
    HRSRC res = FindResourceW(nullptr, MAKEINTRESOURCEW(id), RT_RCDATA); if (!res) return false;
    HGLOBAL loaded = LoadResource(nullptr, res); DWORD size = SizeofResource(nullptr, res); void* data = LockResource(loaded); if (!loaded || !data || !size) return false;
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); if (file == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0; bool ok = WriteFile(file, data, size, &written, nullptr) && written == size; CloseHandle(file); return ok;
}
static bool CopySelfIcon(const std::wstring& path) {
    HRSRC res = FindResourceW(nullptr, MAKEINTRESOURCEW(IDI_PANE_BROWSER), RT_GROUP_ICON); if (!res) return false;
    HGLOBAL loaded = LoadResource(nullptr, res); DWORD size = SizeofResource(nullptr, res); void* data = LockResource(loaded); if (!data || !size) return false;
    HANDLE file = CreateFileW(path.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr); if (file == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0; bool ok = WriteFile(file, data, size, &written, nullptr) && written == size; CloseHandle(file); return ok;
}
static void Shortcut(const std::wstring& link, const std::wstring& target, const std::wstring& work, const std::wstring& icon) {
    std::wstring script = L"$ws=New-Object -ComObject WScript.Shell;$s=$ws.CreateShortcut('" + link + L"');$s.TargetPath='" + target + L"';$s.WorkingDirectory='" + work + L"';$s.IconLocation='" + icon + L",0';$s.Save()";
    std::wstring command = L"powershell.exe -NoProfile -ExecutionPolicy Bypass -WindowStyle Hidden -Command \"" + script + L"\"";
    STARTUPINFOW si{}; si.cb = sizeof(si); PROCESS_INFORMATION pi{}; CreateProcessW(nullptr, command.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &si, &pi); if (pi.hThread) CloseHandle(pi.hThread); if (pi.hProcess) { WaitForSingleObject(pi.hProcess, 10000); CloseHandle(pi.hProcess); }
}
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    wchar_t local[MAX_PATH] = {}; if (FAILED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, local))) return 2;
    std::wstring folder = Join(local, L"Pane Browser"); CreateDirectoryW(folder.c_str(), nullptr);
    std::wstring core = Join(folder, L"PaneBrowser.exe"), loader = Join(folder, L"WebView2Loader.dll"), icon = core;
    if (!ExtractResource(IDR_PANE_BROWSER_CORE, core) || !ExtractResource(IDR_WEBVIEW2_LOADER, loader)) {
        MessageBoxW(nullptr, L"Pane Browser setup could not unpack the application files.", L"Pane Browser Setup", MB_OK | MB_ICONERROR); return 3;
    }
    wchar_t desktop[MAX_PATH] = {}; SHGetFolderPathW(nullptr, CSIDL_DESKTOPDIRECTORY, nullptr, SHGFP_TYPE_CURRENT, desktop);
    Shortcut(Join(desktop, L"Pane Browser.lnk"), core, folder, icon);
    wchar_t start[MAX_PATH] = {}; SHGetFolderPathW(nullptr, CSIDL_PROGRAMS, nullptr, SHGFP_TYPE_CURRENT, start);
    std::wstring menu = Join(start, L"Pane Browser"); CreateDirectoryW(menu.c_str(), nullptr); Shortcut(Join(menu, L"Pane Browser.lnk"), core, folder, icon);
    MessageBoxW(nullptr, L"Pane Browser v2.0.0 was installed. A shortcut was added to the Desktop.", L"Pane Browser Setup", MB_OK | MB_ICONINFORMATION);
    return 0;
}
