#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include "mini_resource.h"

namespace {

bool JoinPath(wchar_t* out, DWORD capacity, const wchar_t* folder, const wchar_t* name) {
    if (!out || !folder || !name || !capacity) return false;
    int n = wsprintfW(out, L"%s\\%s", folder, name);
    return n > 0 && static_cast<DWORD>(n) < capacity;
}

bool TempFolder(wchar_t* out, DWORD capacity) {
    wchar_t base[MAX_PATH]{};
    DWORD length = GetTempPathW(_countof(base), base);
    if (!length || length >= _countof(base) || capacity < MAX_PATH) return false;
    wchar_t candidate[MAX_PATH]{};
    if (!GetTempFileNameW(base, L"PBM", 0, candidate)) return false;
    DeleteFileW(candidate);
    if (!CreateDirectoryW(candidate, nullptr)) return false;
    lstrcpynW(out, candidate, capacity);
    return true;
}

bool ExtractResource(int id, const wchar_t* path) {
    HRSRC resource = FindResourceW(nullptr, MAKEINTRESOURCEW(id), RT_RCDATA);
    if (!resource) return false;
    HGLOBAL loaded = LoadResource(nullptr, resource);
    DWORD size = SizeofResource(nullptr, resource);
    void* data = loaded ? LockResource(loaded) : nullptr;
    if (!loaded || !data || !size) return false;
    HANDLE file = CreateFileW(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0;
    BOOL ok = WriteFile(file, data, size, &written, nullptr) && written == size;
    FlushFileBuffers(file);
    CloseHandle(file);
    return ok != FALSE;
}

bool WriteBytes(const wchar_t* path, const void* data, DWORD size) {
    HANDLE file = CreateFileW(path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) return false;
    DWORD written = 0; BOOL ok = WriteFile(file, data, size, &written, nullptr) && written == size;
    FlushFileBuffers(file); CloseHandle(file); return ok != FALSE;
}

bool ExtractCompressedResource(int id, const wchar_t* path) {
    HRSRC resource = FindResourceW(nullptr, MAKEINTRESOURCEW(id), RT_RCDATA);
    if (!resource) return false;
    HGLOBAL loaded = LoadResource(nullptr, resource); DWORD size = SizeofResource(nullptr, resource);
    const unsigned char* input = loaded ? static_cast<const unsigned char*>(LockResource(loaded)) : nullptr;
    if (!input || size < 4) return false;
    DWORD outputSize = static_cast<DWORD>(input[0]) | (static_cast<DWORD>(input[1]) << 8) | (static_cast<DWORD>(input[2]) << 16) | (static_cast<DWORD>(input[3]) << 24);
    if (!outputSize || outputSize > 2 * 1024 * 1024) return false;
    unsigned char* output = static_cast<unsigned char*>(HeapAlloc(GetProcessHeap(), 0, outputSize));
    if (!output) return false;
    DWORD inPos = 4, outPos = 0; bool ok = true;
    while (inPos < size && ok) {
        unsigned char token = input[inPos++]; DWORD literalLength = token >> 4;
        if (literalLength == 15) { unsigned char extra = 255; while (extra == 255) { if (inPos >= size) { ok = false; break; } extra = input[inPos++]; literalLength += extra; } }
        if (!ok || inPos + literalLength > size || outPos + literalLength > outputSize) { ok = false; break; }
        CopyMemory(output + outPos, input + inPos, literalLength); inPos += literalLength; outPos += literalLength;
        if (inPos == size) break;
        if (inPos + 2 > size) { ok = false; break; }
        DWORD offset = static_cast<DWORD>(input[inPos]) | (static_cast<DWORD>(input[inPos + 1]) << 8); inPos += 2;
        if (!offset || offset > outPos) { ok = false; break; }
        DWORD matchLength = token & 15;
        if (matchLength == 15) { unsigned char extra = 255; while (extra == 255) { if (inPos >= size) { ok = false; break; } extra = input[inPos++]; matchLength += extra; } }
        matchLength += 4;
        if (!ok || outPos + matchLength > outputSize) { ok = false; break; }
        for (DWORD i = 0; i < matchLength; ++i) { output[outPos] = output[outPos - offset]; ++outPos; }
    }
    ok = ok && outPos == outputSize && WriteBytes(path, output, outputSize);
    HeapFree(GetProcessHeap(), 0, output); return ok;
}

void RemoveTree(const wchar_t* path) {
    DWORD attrs = GetFileAttributesW(path);
    if (attrs == INVALID_FILE_ATTRIBUTES) return;
    if (attrs & FILE_ATTRIBUTE_REPARSE_POINT) {
        SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL);
        if (attrs & FILE_ATTRIBUTE_DIRECTORY) RemoveDirectoryW(path); else DeleteFileW(path);
        return;
    }
    if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) { SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL); DeleteFileW(path); return; }
    wchar_t pattern[MAX_PATH]{};
    if (!JoinPath(pattern, _countof(pattern), path, L"*")) return;
    WIN32_FIND_DATAW data{};
    HANDLE find = FindFirstFileW(pattern, &data);
    if (find != INVALID_HANDLE_VALUE) {
        do {
            if (lstrcmpW(data.cFileName, L".") && lstrcmpW(data.cFileName, L"..")) {
                wchar_t child[MAX_PATH]{};
                if (JoinPath(child, _countof(child), path, data.cFileName)) RemoveTree(child);
            }
        } while (FindNextFileW(find, &data));
        FindClose(find);
    }
    SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL);
    RemoveDirectoryW(path);
}

void RemoveTreeWithRetry(const wchar_t* path) {
    for (int attempt = 0; attempt < 20; ++attempt) {
        RemoveTree(path);
        if (GetFileAttributesW(path) == INVALID_FILE_ATTRIBUTES) return;
        Sleep(150);
    }
}
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    wchar_t folder[MAX_PATH]{};
    if (!TempFolder(folder, _countof(folder))) { MessageBoxW(nullptr, L"Pane Browser MiniEdition could not create its temporary folder.", L"Pane Browser MiniEdition", MB_OK | MB_ICONERROR); return 2; }
    wchar_t core[MAX_PATH]{}, loader[MAX_PATH]{};
    if (!JoinPath(core, _countof(core), folder, L"PaneBrowserMiniCore.exe") || !JoinPath(loader, _countof(loader), folder, L"WebView2Loader.dll") || !ExtractResource(IDR_MINI_CORE, core) || !ExtractCompressedResource(IDR_MINI_LOADER, loader)) {
        MessageBoxW(nullptr, L"Pane Browser MiniEdition could not unpack its embedded browser files.", L"Pane Browser MiniEdition", MB_OK | MB_ICONERROR);
        RemoveTreeWithRetry(folder); return 3;
    }
    STARTUPINFOW startup{}; startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    wchar_t command[MAX_PATH]{}; lstrcpynW(command, core, _countof(command));
    BOOL started = CreateProcessW(core, command, nullptr, nullptr, FALSE, 0, nullptr, folder, &startup, &process);
    if (!started) {
        MessageBoxW(nullptr, L"Pane Browser MiniEdition could not start. Microsoft Edge WebView2 Runtime may be missing.", L"Pane Browser MiniEdition", MB_OK | MB_ICONERROR);
        RemoveTreeWithRetry(folder); return 4;
    }
    CloseHandle(process.hThread);
    WaitForSingleObject(process.hProcess, INFINITE);
    DWORD exitCode = 0; GetExitCodeProcess(process.hProcess, &exitCode); CloseHandle(process.hProcess);
    RemoveTreeWithRetry(folder);
    return static_cast<int>(exitCode);
}
