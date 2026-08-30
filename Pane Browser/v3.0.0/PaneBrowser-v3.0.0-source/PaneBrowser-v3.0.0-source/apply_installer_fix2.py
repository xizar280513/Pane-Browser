from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/single_installer.cpp')
s=p.read_text()
if 'ExtractResourceAtomic' not in s:
    marker='static std::wstring ModulePath() {'
    helper='''static bool ExtractResourceAtomic(int id, const std::wstring& target) {
    std::wstring temp = target + L".new";
    DeleteFileW(temp.c_str());
    if (!ExtractResource(id, temp)) { DeleteFileW(temp.c_str()); return false; }
    BOOL moved = MoveFileExW(temp.c_str(), target.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
    if (!moved) DeleteFileW(temp.c_str());
    return moved != FALSE;
}

'''
    if marker not in s: raise SystemExit('module marker missing')
    s=s.replace(marker,helper+marker,1)
if 'ExtractResourceAtomic(IDR_PANE_BROWSER_CORE' not in s:
    s=s.replace('if(!ExtractResource(IDR_PANE_BROWSER_CORE,core)||!ExtractResource(IDR_WEBVIEW2_LOADER,loader)||self.empty()', 'if(!ExtractResourceAtomic(IDR_PANE_BROWSER_CORE,core)||!ExtractResourceAtomic(IDR_WEBVIEW2_LOADER,loader)||self.empty()', 1)
start=s.find('    WriteUninstallScript(folder);')
end=s.find('\n}', start)
if start < 0 or end < 0: raise SystemExit('install tail bounds missing')
new='''    bool shortcutsOk = WriteUninstallScript(folder);
    wchar_t desktop[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_DESKTOPDIRECTORY,nullptr,SHGFP_TYPE_CURRENT,desktop); shortcutsOk = MakeShortcut(Join(desktop,L"Pane Browser.lnk"),core,folder,core) && shortcutsOk;
    wchar_t programs[MAX_PATH]={}; SHGetFolderPathW(nullptr,CSIDL_PROGRAMS,nullptr,SHGFP_TYPE_CURRENT,programs); std::wstring menu=Join(programs,L"Pane Browser"); if(!CreateDirectoryW(menu.c_str(),nullptr) && GetLastError()!=ERROR_ALREADY_EXISTS) shortcutsOk=false; shortcutsOk = MakeShortcut(Join(menu,L"Pane Browser.lnk"),core,folder,core) && shortcutsOk; wchar_t systemDir[MAX_PATH]={}; GetSystemDirectoryW(systemDir,MAX_PATH); shortcutsOk = MakeShortcut(Join(menu,L"Uninstall Pane Browser.lnk"),Join(systemDir,L"cmd.exe"),folder,core,L"/c \\\"\\\""+Join(folder,L"Uninstall Pane Browser.cmd")+L"\\\"\\\"") && shortcutsOk;
    MessageBoxW(nullptr,shortcutsOk?L"Pane Browser v3.0.0 was installed or upgraded. Shortcuts were updated.":L"Pane Browser v3.0.0 was installed or upgraded, but one or more shortcuts could not be created.",L"Pane Browser Setup",MB_OK|(shortcutsOk?MB_ICONINFORMATION:MB_ICONWARNING)); CoUninitialize(); return 0;'''
s=s[:start]+new+s[end:]
p.write_text(s)
print('installer fix 2 applied')
