from pathlib import Path
import re

cpp=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=cpp.read_text()

def replace_once(old,new,name):
    global s
    if old not in s:
        raise SystemExit(f'missing {name}')
    s=s.replace(old,new,1)

# Correct COM QueryInterface behavior for the multiline callback holders.
for iface in [
    'ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler',
    'ICoreWebView2CreateCoreWebView2ControllerCompletedHandler',
    'ICoreWebView2WebMessageReceivedEventHandler',
    'ICoreWebView2NewWindowRequestedEventHandler',
    'ICoreWebView2NavigationCompletedEventHandler',
]:
    old=f'''HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {{
        if (!out) return E_POINTER;
        (void)riid;
        *out = static_cast<{iface}*>(this);'''
    new=f'''HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {{
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_{iface}) {{ *out = nullptr; return E_NOINTERFACE; }}
        *out = static_cast<{iface}*>(this);'''
    replace_once(old,new,iface)

for iface in [
    'ICoreWebView2ExecuteScriptCompletedHandler',
    'ICoreWebView2DownloadStartingEventHandler',
    'ICoreWebView2AcceleratorKeyPressedEventHandler',
]:
    old=f'''HRESULT STDMETHODCALLTYPE QueryInterface(REFIID, void** out) override {{ if (!out) return E_POINTER; *out = static_cast<{iface}*>(this);'''
    new=f'''HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {{ if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_{iface}) {{ *out = nullptr; return E_NOINTERFACE; }} *out = static_cast<{iface}*>(this);'''
    replace_once(old,new,iface)

replace_once("    if (input.find(L'.') != std::wstring::npos) return L\"https://\" + input;\n", "    if (input.find_first_of(L\" \\t\\r\\n\") == std::wstring::npos && input.find(L'.') != std::wstring::npos) return L\"https://\" + input;\n", 'query whitespace')
replace_once('void ReopenClosedTab() { if (g_closedTabs.empty()) return;', 'void ReopenClosedTab() { if (!g_settings.tabsEnabled || g_closedTabs.empty()) return;', 'tabs-disabled reopen')
replace_once('        SetActiveTab(0);\n        return;\n    }\n', '        SetActiveTab(0);\n        SavePersistentData();\n        return;\n    }\n', 'single-tab persistence')

# Add an internal-source check immediately before the message parser.
marker='void HandleHistoryMessage(BrowserTab* tab, ICoreWebView2WebMessageReceivedEventArgs* args) {'
if 'bool IsTrustedInternalMessage(' not in s:
    helper='''bool IsTrustedInternalMessage(BrowserTab* tab, ICoreWebView2WebMessageReceivedEventArgs* args) {
    if (!tab || !args || !(tab->internalPage || tab->historyPage || tab->bookmarksPage || tab->settingsPage || tab->downloadsPage)) return false;
    LPWSTR source = nullptr;
    if (FAILED(args->get_Source(&source)) || !source) return false;
    std::wstring document(source);
    CoTaskMemFree(source);
    return document == L"about:blank";
}

'''
    replace_once(marker,helper+marker,'internal message helper')
handler_start=s.index(marker)
tail=s[handler_start:]
replace_once_tail='    if (!tab || !args) return;\n    LPWSTR raw = nullptr;'
if replace_once_tail not in tail:
    raise SystemExit('message guard missing')
tail=tail.replace(replace_once_tail,'    if (!tab || !args || !IsTrustedInternalMessage(tab, args)) return;\n    LPWSTR raw = nullptr;',1)
s=s[:handler_start]+tail

# When users disable persistence, remove stale files rather than leave old private data on disk.
replace_once('    if (message.rfind(L"settings-history:", 0) == 0) {\n        g_settings.persistHistory = message.substr(17) != L"0"; SavePersistentData(); RefreshSettingsPage(); return;\n    }', '    if (message.rfind(L"settings-history:", 0) == 0) {\n        g_settings.persistHistory = message.substr(17) != L"0"; if (!g_settings.persistHistory) v3data::ClearHistory(g_dataDirectory); else SavePersistentData(); RefreshSettingsPage(); return;\n    }', 'history toggle cleanup')
replace_once('    if (message.rfind(L"settings-restore:",0)==0) { g_settings.restoreSession=message.substr(18)!=L"0"; SavePersistentData(); return; }', '    if (message.rfind(L"settings-restore:",0)==0) { g_settings.restoreSession=message.substr(18)!=L"0"; if (!g_settings.restoreSession) v3data::ClearSession(g_dataDirectory); else SavePersistentData(); RefreshSettingsPage(); return; }', 'restore toggle cleanup')

cpp.write_text(s)

# Data layer path and atomic-write fixes.
data=Path('/home/ubuntu/pane-browser-v3/v3_data.h')
t=data.read_text()
old='''inline bool EnsureDirectory(const std::wstring& path) {
    if (path.empty()) return false;
    if (CreateDirectoryW(path.c_str(), nullptr)) return true;
    return GetLastError() == ERROR_ALREADY_EXISTS;
}'''
new='''inline bool EnsureDirectory(const std::wstring& path) {
    if (path.empty()) return false;
    if (CreateDirectoryW(path.c_str(), nullptr)) return true;
    if (GetLastError() != ERROR_ALREADY_EXISTS) return false;
    DWORD attrs = GetFileAttributesW(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}'''
if old in t: t=t.replace(old,new,1)
old='''    std::wstring temp = path + L".tmp";
    std::ofstream file(temp.c_str(), std::ios::binary | std::ios::trunc);'''
new='''    std::wstring temp = path + L".tmp." + std::to_wstring(GetCurrentProcessId()) + L"." + std::to_wstring(GetTickCount64());
    std::ofstream file(temp.c_str(), std::ios::binary | std::ios::trunc);'''
if old in t: t=t.replace(old,new,1)
old='''    if (!file.good()) return false;
    file.close();
    return MoveFileExW(temp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != FALSE;'''
new='''    if (!file.good()) { file.close(); DeleteFileW(temp.c_str()); return false; }
    file.close();
    BOOL moved = MoveFileExW(temp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
    if (!moved) DeleteFileW(temp.c_str());
    return moved != FALSE;'''
if old in t: t=t.replace(old,new,1)
old='''    std::wstring fallback = Join(ModuleDirectory(), L"PaneBrowser.Data");
    EnsureDirectory(fallback);
    return fallback;'''
new='''    std::wstring fallback = Join(ModuleDirectory(), L"PaneBrowser.Data");
    return EnsureDirectory(fallback) ? fallback : L"";'''
if old in t: t=t.replace(old,new,1)
data.write_text(t)
print('audit fixes 2 applied')
