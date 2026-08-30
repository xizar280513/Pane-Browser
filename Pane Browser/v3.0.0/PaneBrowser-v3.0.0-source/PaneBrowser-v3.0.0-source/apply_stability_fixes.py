from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=p.read_text()
s=s.replace('#include <functional>\n', '#include <functional>\n#include <atomic>\n#include <memory>\n', 1)
# Avoid duplicate memory include introduced by conservative replacement.
s=s.replace('#include <memory>\n#include <string>', '#include <string>', 1) if s.count('#include <memory>') > 1 else s
# State for asynchronous update checks.
s=s.replace('constexpr UINT kThemeRefreshTimerId = 0x5A7;\n', 'constexpr UINT kThemeRefreshTimerId = 0x5A7;\nconstexpr UINT kUpdateResultMessage = WM_APP + 0x31;\n', 1)
s=s.replace('UINT_PTR g_downloadTimer = 0;\n', 'UINT_PTR g_downloadTimer = 0;\nstd::atomic_bool g_updateCheckRunning{false};\n', 1)
# Replace synchronous checker with a worker plus UI result message.
start=s.index('void CheckForUpdates() {')
end=s.index('bool HandleShortcut(', start)
new=r'''struct UpdateCheckResult { bool requestSucceeded = false; std::wstring tag; };

DWORD WINAPI UpdateCheckWorker(LPVOID) {
    auto result = std::make_unique<UpdateCheckResult>();
    HINTERNET internet = InternetOpenW(L"Pane Browser/3.0", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, INTERNET_FLAG_NO_CACHE_WRITE);
    if (internet) {
        DWORD timeout = 5000;
        InternetSetOptionW(internet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
        InternetSetOptionW(internet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
        const wchar_t* headers = L"User-Agent: PaneBrowser/3.0\r\nAccept: application/vnd.github+json\r\n";
        HINTERNET request = InternetOpenUrlW(internet, L"https://api.github.com/repos/xizar280513/Pane-Browser/releases/latest", headers, static_cast<DWORD>(-1), INTERNET_FLAG_SECURE | INTERNET_FLAG_NO_CACHE_WRITE, 0);
        std::string body; char buffer[2048]; DWORD n = 0;
        if (request) {
            while (InternetReadFile(request, buffer, sizeof(buffer), &n) && n) body.append(buffer, n);
            InternetCloseHandle(request);
        }
        InternetCloseHandle(internet);
        size_t key = body.find("\"tag_name\"");
        size_t colon = key == std::string::npos ? std::string::npos : body.find(':', key);
        size_t q = colon == std::string::npos ? std::string::npos : body.find('"', colon + 1);
        size_t q2 = q == std::string::npos ? std::string::npos : body.find('"', q + 1);
        if (q != std::string::npos && q2 != std::string::npos) {
            result->tag = v3data::FromUtf8(body.substr(q + 1, q2 - q - 1));
            result->requestSucceeded = !result->tag.empty();
        }
    }
    HWND window = g_main;
    UpdateCheckResult* raw = result.release();
    if (!window || !IsWindow(window) || !PostMessageW(window, kUpdateResultMessage, 0, reinterpret_cast<LPARAM>(raw))) delete raw;
    return 0;
}

void CheckForUpdates() {
    bool expected = false;
    if (!g_updateCheckRunning.compare_exchange_strong(expected, true)) return;
    HANDLE thread = CreateThread(nullptr, 0, UpdateCheckWorker, nullptr, 0, nullptr);
    if (!thread) {
        g_updateCheckRunning.store(false);
        MessageBoxW(g_main, L"Update check could not start.", L"Pane Browser", MB_OK | MB_ICONWARNING);
        return;
    }
    CloseHandle(thread);
}

'''
s=s[:start]+new+s[end:]
# Make title-bar buttons client hit targets before resize-edge checks.
old='''        if (g_settings.customTitleBar) { POINT pt{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)}; ScreenToClient(hwnd,&pt); RECT r{};GetClientRect(hwnd,&r); const int b=6; if(pt.y<b&&pt.x<b)return HTTOPLEFT;'''
new='''        if (g_settings.customTitleBar) { POINT pt{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)}; ScreenToClient(hwnd,&pt); RECT r{};GetClientRect(hwnd,&r); const int b=6; if(pt.y < 32 && pt.x >= r.right - 138) return HTCLIENT; if(pt.y<b&&pt.x<b)return HTTOPLEFT;'''
if old not in s: raise SystemExit('title hit-test marker missing')
s=s.replace(old,new,1)
# Add UI handling for async update result before WM_TIMER.
needle='''    case WM_TIMER:
        if (wp == 0x733) { UpdateDownloadStates(); return 0; }'''
insert='''    case kUpdateResultMessage: {
        std::unique_ptr<UpdateCheckResult> result(reinterpret_cast<UpdateCheckResult*>(lp));
        g_updateCheckRunning.store(false);
        if (!result) return 0;
        if (!result->requestSucceeded) { MessageBoxW(g_main, L"No release information was available.", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        if (result->tag == L"v3.0.0" || result->tag == L"3.0.0") { MessageBoxW(g_main, L"You are using the latest Pane Browser release (v3.0.0).", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        std::wstring prompt = L"Latest GitHub release: " + result->tag + L"\\nOpen the releases page to download it?";
        if (MessageBoxW(g_main, prompt.c_str(), L"Pane Browser", MB_YESNO | MB_ICONINFORMATION) == IDYES) ShellExecuteW(g_main, L"open", L"https://github.com/xizar280513/Pane-Browser/releases", nullptr, nullptr, SW_SHOWNORMAL);
        return 0;
    }
    case WM_TIMER:
        if (wp == 0x733) { UpdateDownloadStates(); return 0; }'''
if needle not in s: raise SystemExit('timer marker missing')
s=s.replace(needle,insert,1)
# If initialization API fails immediately, destroy the window and exit instead of running a blank loop.
old='''    HRESULT hr = InitializeWebView();
    if (FAILED(hr)) MessageBoxW(g_main, L"WebView2 initialization failed.", L"Pane Browser", MB_OK | MB_ICONERROR);

    MSG msg{};'''
new='''    HRESULT hr = InitializeWebView();
    if (FAILED(hr)) { MessageBoxW(g_main, L"WebView2 initialization failed.", L"Pane Browser", MB_OK | MB_ICONERROR); DestroyWindow(g_main); CoUninitialize(); return 1; }

    MSG msg{};'''
if old not in s: raise SystemExit('init failure block missing')
s=s.replace(old,new,1)
p.write_text(s)
print('stability fixes applied')
