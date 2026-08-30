from pathlib import Path

root = Path(__file__).parent
src = (root / "PaneBrowser.cpp").read_text(encoding="utf-8")
build = (root / "build_v5.sh").read_text(encoding="utf-8")
portable_rc = (root / "portable_launcher.rc").read_text(encoding="utf-8")
installer_rc = (root / "installer.rc").read_text(encoding="utf-8")
mini_root = root.parent / "pane-browser-v5-mini"
mini_available = (mini_root / "PaneBrowserMini.cpp").is_file()
mini_src = (mini_root / "PaneBrowserMini.cpp").read_text(encoding="utf-8") if mini_available else ""

checks = [
    ("authentication popup classification", "bool authenticationPopup = blankPopup || IsAuthenticationUri(requestedUri);" in src),
    ("ordinary user popup branch", "if (!authenticationPopup && userInitiated)" in src),
    ("ordinary navigation clears internal flags", "ClearInternalFlags(opener);" in src),
    ("ordinary navigation uses opener tab", "opener->webview->Navigate(requestedUri.c_str());" in src),
    ("non-user ordinary popup is handled", "if (!authenticationPopup && !userInitiated)" in src),
    ("authentication popup uses managed NewWindow path", "argsHold->put_NewWindow(popup->webview.Get())" in src),
    ("profile load resets settings defaults", "g_settings = v3data::Settings{};" in src),
    ("authentication tabs are excluded from persistence", "!tab->authenticationWindow" in src),
    ("authentication tabs are excluded from closed-tab recovery", "!g_tabs[index]->privateMode && !g_tabs[index]->authenticationWindow" in src),
    ("tabs-disabled private mode uses delayed replacement", "CreateNewTab(L\"dashboard\", true, replaceTab);" in src and "if (replaceTab && replaceTab.get() != tab.get())" in src),
    ("protected and temporary profiles cannot be deleted", "g_activeProfile.kind != L\"normal\" || g_activeProfile.temporary" in src),
    ("trusted internal message source boundary", "document == L\"about:blank\"" in src),
    ("history selection is limited to visible rows", "function visibleRows()" in src and "function selectAll(){visibleRows().forEach" in src),
    ("hidden history rows are not selected globally", "document.querySelectorAll('.pick').forEach(p=>p.checked=true)" not in src),
    ("session metadata helper exists", "void ClearSessionMetadata();" in src and "void ClearSessionMetadata()" in src),
    ("session metadata is cleared on restore disable", "v3data::ClearSession(g_dataDirectory); ClearSessionMetadata(); SavePersistentData();" in src),
    ("save path clears disabled restore state", "v3data::ClearSession(g_dataDirectory);\n        ClearSessionMetadata();" in src),
    ("v5 build naming is present", build.count("v5.0.0") >= 4),
    ("portable resource embeds v5 core", "PaneBrowser-v5.0.0-core.exe" in portable_rc),
    ("installer resource embeds v5 core", "PaneBrowser-v5.0.0-core.exe" in installer_rc),
    ("regular registers WebView2 fullscreen event", "add_ContainsFullScreenElementChanged" in src and "ICoreWebView2ContainsFullScreenElementChangedEventHandler" in src),
    ("regular has native F11 toggle", "if(!ctrl && vk==VK_F11){ToggleBrowserFullscreen();return true;}" in src),
    ("regular fullscreen restores saved window placement", "SetWindowPlacement(g_main, &g_fullscreenPlacement);" in src and "g_fullscreenSaved" in src),
    ("regular fullscreen hides toolbar and restores address bar", "ShowWindow(g_tabStrip, SW_HIDE)" in src and "ShowWindow(g_address, SW_HIDE)" in src and "ShowWindow(g_address, SW_SHOW)" in src and "SetWindowTextW(g_address, restored->lastUri" in src),
    ("View Source helper uses view-source scheme", "void OpenViewSource()" in src and "std::wstring sourceUri = L\"view-source:\" + uri;" in src),
    ("Ctrl+U dispatches View Source", "ctrl&&!shift&&vk=='U'" in src and "kOpenViewSourceMessage" in src),
    ("Inspect helper opens WebView2 DevTools", "void OpenDevTools()" in src and "OpenDevToolsWindow()" in src),
    ("F12 dispatches Inspect", "!ctrl && vk==VK_F12" in src and "kOpenDevToolsMessage" in src),
    ("context menu exposes View Source", "kContextViewSource" in src and "L\"View Source\"" in src),
    ("context menu exposes Inspect", "kContextInspect" in src and "L\"Inspect\"" in src),
    ("view-source links can open a new tab", "sourceDocument" in src and "CreateNewTab(requestedUri, opener->privateMode);" in src),
]


if mini_available:
    checks.extend([
        ("mini registers WebView2 fullscreen event", "add_ContainsFullScreenElementChanged" in mini_src and "ICoreWebView2ContainsFullScreenElementChangedEventHandler" in mini_src),
        ("mini has independent F11 state", "g_f11Fullscreen" in mini_src and "VK_F11" in mini_src),
        ("mini fullscreen restores saved window placement", "SetWindowPlacement(g_main, &g_fullscreenPlacement);" in mini_src and "g_fullscreenSaved" in mini_src),
        ("mini fullscreen hides browser chrome", "ShowWindow(g_back, SW_HIDE)" in mini_src and "ShowWindow(g_address, SW_HIDE)" in mini_src),
    ])

failed = [name for name, passed in checks if not passed]
if failed:
    raise AssertionError("failed checks: " + ", ".join(failed))

print(f"{len(checks)}/{len(checks)} v5 targeted source regression checks passed")
