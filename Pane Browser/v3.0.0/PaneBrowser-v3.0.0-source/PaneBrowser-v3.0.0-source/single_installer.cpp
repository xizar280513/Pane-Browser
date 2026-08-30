#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <shlobj.h>
#include <shobjidl.h>
#include <objbase.h>
#include <string>
#include <fstream>
#include <vector>
#include "resource_v2.h"

static std::wstring Join(const std::wstring& a, const std::wstring& b) { return a + (a.empty() || a.back() == L'\\' ? L"" : L"\\") + b; }
static bool ExtractResource(int id,const std::wstring& path) {
    HRSRC res=FindResourceW(nullptr,MAKEINTRESOURCEW(id),RT_RCDATA); if(!res)return false; HGLOBAL loaded=LoadResource(nullptr,res); DWORD size=SizeofResource(nullptr,res); void* data=LockResource(loaded); if(!loaded||!data||!size)return false;
    HANDLE file=CreateFileW(path.c_str(),GENERIC_WRITE,0,nullptr,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,nullptr); if(file==INVALID_HANDLE_VALUE)return false; DWORD written=0; bool ok=WriteFile(file,data,size,&written,nullptr)&&written==size; FlushFileBuffers(file); CloseHandle(file); return ok;
}
static bool ExtractResourceAtomic(int id, const std::wstring& target) {
    std::wstring temp = target + L".new";
    DeleteFileW(temp.c_str());
    if (!ExtractResource(id, temp)) { DeleteFileW(temp.c_str()); return false; }
    BOOL moved = MoveFileExW(temp.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
    if (!moved) DeleteFileW(temp.c_str());
    return moved != FALSE;
}

static std::wstring ModulePath() { wchar_t p[32768]={}; DWORD n=GetModuleFileNameW(nullptr,p,32768); return n?std::wstring(p,n):L""; }
static void RemoveShortcut(const std::wstring& path) { SetFileAttributesW(path.c_str(),FILE_ATTRIBUTE_NORMAL); DeleteFileW(path.c_str()); }
static bool MakeShortcut(const std::wstring& link,const std::wstring& target,const std::wstring& work,const std::wstring& icon,const std::wstring& args=L"") {
    IShellLinkW* shell=nullptr; if(FAILED(CoCreateInstance(CLSID_ShellLink,nullptr,CLSCTX_INPROC_SERVER,IID_IShellLinkW,reinterpret_cast<void**>(&shell))))return false;
    shell->SetPath(target.c_str()); shell->SetArguments(args.c_str()); shell->SetWorkingDirectory(work.c_str()); shell->SetIconLocation(icon.c_str(),0); shell->SetDescription(L"Pane Browser");
    IPersistFile* persist=nullptr; bool ok=SUCCEEDED(shell->QueryInterface(IID_IPersistFile,reinterpret_cast<void**>(&persist)))&&SUCCEEDED(persist->Save(link.c_str(),TRUE)); if(persist)persist->Release(); shell->Release(); return ok;
}
static bool IsUninstall(PWSTR cmd) { return cmd && (wcsstr(cmd,L"/uninstall")||wcsstr(cmd,L"-uninstall")); }
static bool WriteUninstallScript(const std::wstring& folder) {
    std::wstring path=Join(folder,L"Uninstall Pane Browser.cmd"); std::ofstream out(path.c_str(),std::ios::binary|std::ios::trunc); if(!out)return false;
    out << "@echo off\r\ntimeout /t 1 /nobreak >nul\r\ndel /f /q \"%~dp0PaneBrowser.exe\" >nul 2>&1\r\ndel /f /q \"%~dp0WebView2Loader.dll\" >nul 2>&1\r\ndel /f /q \"%~dp0PaneBrowserSetup.exe\" >nul 2>&1\r\ndel /f /q \"%~dp0Uninstall Pane Browser.cmd\" >nul 2>&1\r\nrmdir \"%~dp0\" >nul 2>&1\r\n";
    out.close(); return true;
}
static void RunUninstallScript(const std::wstring& script) {
    std::wstring command=L"cmd.exe /c \"\""+script+L"\"\""; STARTUPINFOW si{};si.cb=sizeof(si);PROCESS_INFORMATION pi{}; std::vector<wchar_t> buffer(command.begin(),command.end());buffer.push_back(L'\0');
    if(CreateProcessW(nullptr,buffer.data(),nullptr,nullptr,FALSE,CREATE_NO_WINDOW,nullptr,nullptr,&si,&pi)){CloseHandle(pi.hThread);CloseHandle(pi.hProcess);}
}
static void RemoveInstalled(const std::wstring& folder) {
    wchar_t desktop[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_DESKTOPDIRECTORY,nullptr,SHGFP_TYPE_CURRENT,desktop); RemoveShortcut(Join(desktop,L"Pane Browser.lnk"));
    wchar_t programs[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_PROGRAMS,nullptr,SHGFP_TYPE_CURRENT,programs); std::wstring menu=Join(programs,L"Pane Browser"); RemoveShortcut(Join(menu,L"Pane Browser.lnk")); RemoveShortcut(Join(menu,L"Uninstall Pane Browser.lnk")); RemoveDirectoryW(menu.c_str());
    std::wstring script=Join(folder,L"Uninstall Pane Browser.cmd"); if(GetFileAttributesW(script.c_str())!=INVALID_FILE_ATTRIBUTES){RunUninstallScript(script);return;}
    DeleteFileW(Join(folder,L"PaneBrowser.exe").c_str()); DeleteFileW(Join(folder,L"WebView2Loader.dll").c_str()); DeleteFileW(Join(folder,L"PaneBrowserSetup.exe").c_str()); RemoveDirectoryW(folder.c_str());
}
int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR cmd, int) {
    wchar_t local[MAX_PATH]={}; if(FAILED(SHGetFolderPathW(nullptr,CSIDL_LOCAL_APPDATA,nullptr,SHGFP_TYPE_CURRENT,local)))return 2; std::wstring folder=Join(local,L"Pane Browser");
    HRESULT co=CoInitializeEx(nullptr,COINIT_APARTMENTTHREADED); if(FAILED(co))return 2;
    if(IsUninstall(cmd)) { RemoveInstalled(folder); MessageBoxW(nullptr,L"Pane Browser uninstall cleanup was started. Your History and Bookmarks data were kept.",L"Pane Browser Setup",MB_OK|MB_ICONINFORMATION); CoUninitialize(); return 0; }
    if(!CreateDirectoryW(folder.c_str(),nullptr) && (GetLastError()!=ERROR_ALREADY_EXISTS || (GetFileAttributesW(folder.c_str()) == INVALID_FILE_ATTRIBUTES || !(GetFileAttributesW(folder.c_str()) & FILE_ATTRIBUTE_DIRECTORY)))){CoUninitialize();return 3;}
    std::wstring core=Join(folder,L"PaneBrowser.exe"), loader=Join(folder,L"WebView2Loader.dll"), setup=Join(folder,L"PaneBrowserSetup.exe"); std::wstring self=ModulePath();
    if(!ExtractResourceAtomic(IDR_PANE_BROWSER_CORE,core)||!ExtractResourceAtomic(IDR_WEBVIEW2_LOADER,loader)||self.empty()||(!(_wcsicmp(self.c_str(),setup.c_str())==0)&&!CopyFileW(self.c_str(),setup.c_str(),FALSE))){MessageBoxW(nullptr,L"Pane Browser setup could not install or upgrade the application. Close Pane Browser and try again.",L"Pane Browser Setup",MB_OK|MB_ICONERROR);CoUninitialize();return 4;}
    bool shortcutsOk = WriteUninstallScript(folder);
    wchar_t desktop[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_DESKTOPDIRECTORY,nullptr,SHGFP_TYPE_CURRENT,desktop); shortcutsOk = MakeShortcut(Join(desktop,L"Pane Browser.lnk"),core,folder,core) && shortcutsOk;
    wchar_t programs[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_PROGRAMS,nullptr,SHGFP_TYPE_CURRENT,programs); std::wstring menu=Join(programs,L"Pane Browser"); if(!CreateDirectoryW(menu.c_str(),nullptr) && GetLastError()!=ERROR_ALREADY_EXISTS) shortcutsOk=false; shortcutsOk = MakeShortcut(Join(menu,L"Pane Browser.lnk"),core,folder,core) && shortcutsOk; wchar_t systemDir[MAX_PATH]={}; GetSystemDirectoryW(systemDir,MAX_PATH); shortcutsOk = MakeShortcut(Join(menu,L"Uninstall Pane Browser.lnk"),Join(systemDir,L"cmd.exe"),folder,core,L"/c \"\""+Join(folder,L"Uninstall Pane Browser.cmd")+L"\"\"") && shortcutsOk;
    MessageBoxW(nullptr,shortcutsOk?L"Pane Browser v3.0.0 was installed or upgraded. Shortcuts were updated.":L"Pane Browser v3.0.0 was installed or upgraded, but one or more shortcuts could not be created.",L"Pane Browser Setup",MB_OK|(shortcutsOk?MB_ICONINFORMATION:MB_ICONWARNING)); CoUninitialize(); return 0;
}
