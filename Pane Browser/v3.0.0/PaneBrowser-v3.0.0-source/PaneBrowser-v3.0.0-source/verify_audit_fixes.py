from pathlib import Path
cpp=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp').read_text()
data=Path('/home/ubuntu/pane-browser-v3/v3_data.h').read_text()
portable=Path('/home/ubuntu/pane-browser-v3/single_portable_launcher.cpp').read_text()
installer=Path('/home/ubuntu/pane-browser-v3/single_installer.cpp').read_text()
checks=[
('S-01 source guard', 'IsTrustedInternalMessage(tab, args)' in cpp and 'args->get_Source(&source)' in cpp and 'return document == L"about:blank"' in cpp),
('S-02 no external favicon', 'google.com/s2/favicons' not in cpp and 'siteicon' in cpp),
('S-03 title button hit target', 'if(pt.y < 32 && pt.x >= r.right - 138) return HTCLIENT;' in cpp),
('S-04 directory validation', 'GetFileAttributesW(path.c_str())' in data and 'FILE_ATTRIBUTE_DIRECTORY' in data),
('S-05 unique temp writes', 'GetCurrentProcessId()' in data and 'GetTickCount64()' in data and 'DeleteFileW(temp.c_str())' in data),
('S-06 stale persistence cleanup', 'else { v3data::ClearHistory(g_dataDirectory); }' in cpp and 'else v3data::ClearSession(g_dataDirectory);' in cpp),
('S-07 clear completion', 'ICoreWebView2ClearBrowsingDataCompletedHandler' in cpp and 'ClearBrowsingDataAll(callback.Get())' in cpp and 'This may sign you out' in cpp),
('S-08 portable retry', 'RemoveTreeWithRetry' in portable and 'Sleep(200)' in portable),
('S-09 installer atomic/diagnostic', 'ExtractResourceAtomic' in installer and 'shortcutsOk' in installer),
('S-10 async update', 'UpdateCheckWorker' in cpp and 'CreateThread' in cpp and 'kUpdateResultMessage' in cpp),
('S-11 tabs disabled', 'if (!g_settings.tabsEnabled || g_closedTabs.empty()) return;' in cpp),
('S-12 init failure exit', 'if (FAILED(hr)) { MessageBoxW(g_main' in cpp and 'DestroyWindow(g_main)' in cpp),
('S-13 COM QI', 'return E_NOINTERFACE' in cpp),
('S-15 history cap', 'kMaxHistoryEntries' in cpp),
('S-17 context event', 'add_ContextMenuRequested' in cpp and 'ShowNativeContextMenu(point)' in cpp),
('S-18 trim input', 'find_first_not_of' in cpp and 'value.find_first_of' in cpp),
('state external nav', 'tab->internalPage = false;' in cpp and 'ClearInternalFlags(tab.get());' in cpp),
('DPI V2 fallback', 'DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2' in cpp and 'SetProcessDPIAware' in cpp),
]
for name,ok in checks: print(f'[{'PASS' if ok else 'FAIL'}] {name}')
print(f'passed={sum(1 for _,ok in checks if ok)}/{len(checks)}')
