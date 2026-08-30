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
    wchar_t base[MAX_PATH]={}; DWORD n=GetTempPathW(MAX_PATH,base); std::wstring root=n?std::wstring(base,n):L".";
    wchar_t name[MAX_PATH]={}; if(GetTempFileNameW(root.c_str(),L"PBR",0,name)) { DeleteFileW(name); std::wstring folder=name; if(CreateDirectoryW(folder.c_str(),nullptr))return folder; }
    root += L"PaneBrowser-" + std::to_wstring(GetCurrentProcessId()) + L"-" + std::to_wstring(GetTickCount64()); CreateDirectoryW(root.c_str(),nullptr); return root;
}
static bool ExtractResource(int id,const std::wstring& path) {
    HRSRC res=FindResourceW(nullptr,MAKEINTRESOURCEW(id),RT_RCDATA); if(!res)return false; HGLOBAL loaded=LoadResource(nullptr,res); DWORD size=SizeofResource(nullptr,res); void* data=LockResource(loaded); if(!loaded||!data||!size)return false;
    HANDLE file=CreateFileW(path.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr); if(file==INVALID_HANDLE_VALUE)return false; DWORD written=0; bool ok=WriteFile(file,data,size,&written,nullptr)&&written==size; FlushFileBuffers(file); CloseHandle(file); return ok;
}
static void RemoveTree(const std::wstring& folder) {
    WIN32_FIND_DATAW fd{}; HANDLE find=FindFirstFileW((folder+L"\\*").c_str(),&fd); if(find==INVALID_HANDLE_VALUE){RemoveDirectoryW(folder.c_str());return;}
    do { if(wcscmp(fd.cFileName,L".")==0||wcscmp(fd.cFileName,L"..")==0)continue; std::wstring child=folder+L"\\"+fd.cFileName; if(fd.dwFileAttributes&FILE_ATTRIBUTE_DIRECTORY)RemoveTree(child); else {SetFileAttributesW(child.c_str(),FILE_ATTRIBUTE_NORMAL);DeleteFileW(child.c_str());} } while(FindNextFileW(find,&fd)); FindClose(find); RemoveDirectoryW(folder.c_str());
}
static void RemoveTreeWithRetry(const std::wstring& folder) {
    for (int attempt = 0; attempt < 15; ++attempt) {
        RemoveTree(folder);
        if (GetFileAttributesW(folder.c_str()) == INVALID_FILE_ATTRIBUTES) return;
        Sleep(200);
    }
}
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    std::wstring folder=TempFolder(); if(folder.empty()){MessageBoxW(nullptr,L"Pane Browser could not create a temporary folder.",L"Pane Browser",MB_OK|MB_ICONERROR);return 2;}
    std::wstring core=folder+L"\\PaneBrowserCore.exe", loader=folder+L"\\WebView2Loader.dll";
    if(!ExtractResource(IDR_PANE_BROWSER_CORE,core)||!ExtractResource(IDR_WEBVIEW2_LOADER,loader)){MessageBoxW(nullptr,L"Pane Browser could not unpack its embedded runtime files.",L"Pane Browser",MB_OK|MB_ICONERROR);RemoveTreeWithRetry(folder);return 3;}
    STARTUPINFOW si{}; si.cb=sizeof(si); PROCESS_INFORMATION pi{}; std::vector<wchar_t> command(core.begin(),core.end()); command.push_back(L'\0');
    BOOL started=CreateProcessW(core.c_str(),command.data(),nullptr,nullptr,FALSE,0,nullptr,folder.c_str(),&si,&pi); if(!started){MessageBoxW(nullptr,L"Pane Browser could not start. Make sure Microsoft WebView2 Runtime is installed.",L"Pane Browser",MB_OK|MB_ICONERROR);RemoveTreeWithRetry(folder);return 4;}
    CloseHandle(pi.hThread); WaitForSingleObject(pi.hProcess,INFINITE); DWORD exitCode=0;GetExitCodeProcess(pi.hProcess,&exitCode);CloseHandle(pi.hProcess); RemoveTreeWithRetry(folder); return static_cast<int>(exitCode);
}
