from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=p.read_text()
needle='void ConfigureProfilePrivacy(BrowserTab* tab) {'
insert='''bool HandleGlobalShortcutMessage(const MSG& message) {
    if (message.message != WM_KEYDOWN && message.message != WM_SYSKEYDOWN) return false;
    bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    bool alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    return HandleShortcut(static_cast<UINT>(message.wParam), ctrl, shift, alt);
}

'''
if needle not in s: raise SystemExit('shortcut insertion marker missing')
s=s.replace(needle,insert+needle,1)
old='''    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }'''
new='''    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (HandleGlobalShortcutMessage(msg)) continue;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }'''
if old not in s: raise SystemExit('message loop marker missing')
s=s.replace(old,new,1)
p.write_text(s)
print('global shortcuts fix applied')
