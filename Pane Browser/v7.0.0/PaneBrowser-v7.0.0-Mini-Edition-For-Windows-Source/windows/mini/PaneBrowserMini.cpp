#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#define NOMINMAX
#include <windows.h>
#include <winreg.h>
#include <wrl.h>
#include "sdk/webview2/build/native/include/WebView2.h"

using Microsoft::WRL::ComPtr;

template <typename Interface, typename Fn> class CallbackHolder;

template <typename Fn>
class CallbackHolder<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler, Fn> : public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, ICoreWebView2Environment* env) override { return fn_(hr, env); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler, Fn> : public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, ICoreWebView2Controller* controller) override { return fn_(hr, controller); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2SourceChangedEventHandler, Fn> : public ICoreWebView2SourceChangedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2SourceChangedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2SourceChangedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2ExecuteScriptCompletedHandler, Fn> : public ICoreWebView2ExecuteScriptCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ExecuteScriptCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ExecuteScriptCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, LPCWSTR resultObjectAsJson) override { return fn_(hr, resultObjectAsJson); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2AcceleratorKeyPressedEventHandler, Fn> : public ICoreWebView2AcceleratorKeyPressedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2AcceleratorKeyPressedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2AcceleratorKeyPressedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2Controller* sender, ICoreWebView2AcceleratorKeyPressedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2ContainsFullScreenElementChangedEventHandler, Fn> : public ICoreWebView2ContainsFullScreenElementChangedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ContainsFullScreenElementChangedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ContainsFullScreenElementChangedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, IUnknown* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2NewWindowRequestedEventHandler, Fn> : public ICoreWebView2NewWindowRequestedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2NewWindowRequestedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2NewWindowRequestedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2WebMessageReceivedEventHandler, Fn> : public ICoreWebView2WebMessageReceivedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(fn) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2WebMessageReceivedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2WebMessageReceivedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Interface, typename Fn>
Interface* Callback(Fn fn) { return new CallbackHolder<Interface, Fn>(fn); }

namespace {
constexpr wchar_t kWindowClass[] = L"PaneBrowserMiniEdition";
constexpr int kBackId = 101, kForwardId = 102, kRefreshId = 103, kHomeId = 104, kAddressId = 105;
HWND g_main = nullptr, g_back = nullptr, g_forward = nullptr, g_refresh = nullptr, g_home = nullptr, g_address = nullptr;
HBRUSH g_windowBrush = nullptr, g_controlBrush = nullptr;
bool g_darkTheme = false;
bool g_homeVisible = false;
WNDPROC g_oldAddressProc = nullptr;
bool g_browserFullscreen = false;
bool g_f11Fullscreen = false;
bool g_videoFullscreen = false;
bool g_fullscreenSaved = false;
LONG_PTR g_fullscreenStyle = 0;
LONG_PTR g_fullscreenExStyle = 0;
WINDOWPLACEMENT g_fullscreenPlacement{sizeof(WINDOWPLACEMENT)};
ComPtr<ICoreWebView2Environment> g_environment;
ComPtr<ICoreWebView2Controller> g_controller;
ComPtr<ICoreWebView2> g_webview;
wchar_t g_lastUri[8192]{};
wchar_t g_ephemeralUserData[MAX_PATH]{};

const wchar_t* kDashboardHtml = LR"HTML(<!doctype html><html><head><meta charset="utf-8"><meta name="color-scheme" content="light dark"><title>Pane Browser Mini v7.0.0 Edition</title><style>:root{color-scheme:light;--page-bg:#f4f5f7;--text:#1f2328;--muted:#5f6368;--field-bg:#ffffff;--field-border:#c7cbd1;--button-bg:#e5e7eb}:root.dark{color-scheme:dark;--page-bg:#101214;--text:#f1f3f4;--muted:#9aa0a6;--field-bg:#202124;--field-border:#5f6368;--button-bg:#303134}html,body{height:100%;margin:0}body{display:grid;place-items:center;background:var(--page-bg);color:var(--text);font-family:Segoe UI,system-ui,sans-serif}.wrap{width:min(620px,88vw);text-align:center}.brand{font-size:34px;font-weight:600;letter-spacing:.2px;margin-bottom:22px}.search{height:54px;display:flex;border:1px solid var(--field-border);border-radius:6px;background:var(--field-bg);overflow:hidden}.search input{flex:1;border:0;outline:0;padding:0 16px;background:transparent;color:var(--text);font:16px Segoe UI,system-ui,sans-serif}.search button{width:56px;border:0;background:var(--button-bg);color:var(--text);font-size:22px;cursor:pointer}.note{margin-top:18px;color:var(--muted);font-size:12px}</style></head><body><main class="wrap"><div class="brand">Pane Browser</div><form class="search" onsubmit="go();return false"><input id="q" autocomplete="off" autofocus><button aria-label="Search">⌕</button></form><div class="note">Single tab · no history · no persistent browser data</div></main><script>function setPaneTheme(dark){document.documentElement.classList.toggle('dark',!!dark)}setPaneTheme(window.matchMedia&&window.matchMedia('(prefers-color-scheme: dark)').matches);function go(){const q=document.getElementById('q').value.trim();if(q)window.chrome.webview.postMessage('search:'+q)}</script></body></html>)HTML";

void Append(wchar_t* out, int cap, int* used, const wchar_t* text) {
    if (!out || !used || !text) return;
    while (*text && *used + 1 < cap) out[(*used)++] = *text++;
    out[*used] = 0;
}

void NormalizeInput(const wchar_t* input, wchar_t* output, int cap) {
    if (!output || cap <= 0) return;
    output[0] = 0; if (!input || !*input) return;
    if (wcsncmp(input, L"http://", 7) == 0 || wcsncmp(input, L"https://", 8) == 0) { wcsncpy(output, input, cap - 1); output[cap - 1] = 0; return; }
    bool host = wcschr(input, L'.') != nullptr && wcschr(input, L' ') == nullptr;
    int used = 0;
    if (host) { Append(output, cap, &used, L"https://"); Append(output, cap, &used, input); return; }
    Append(output, cap, &used, L"https://www.startpage.com/sp/search?query=");
    const wchar_t* hex = L"0123456789ABCDEF";
    for (const wchar_t* p = input; *p && used + 4 < cap; ++p) {
        wchar_t c = *p;
        if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || c == L'-' || c == L'_' || c == L'.' || c == L'~') { output[used++] = c; output[used] = 0; }
        else if (c == L' ') Append(output, cap, &used, L"+");
        else if (c <= 0x7F) { output[used++] = L'%'; output[used++] = hex[(c >> 4) & 15]; output[used++] = hex[c & 15]; output[used] = 0; }
        else Append(output, cap, &used, L"%3F");
    }
}

void ResizeChildren();
void ApplyFullscreenState();
void ToggleFullscreen();

bool ReadSystemDarkTheme() {
    HKEY key = nullptr;
    DWORD value = 1;
    DWORD size = sizeof(value);
    LONG result = RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &key);
    if (result == ERROR_SUCCESS) {
        RegQueryValueExW(key, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size);
        RegCloseKey(key);
        return value == 0;
    }
    COLORREF windowColor = GetSysColor(COLOR_WINDOW);
    return (GetRValue(windowColor) + GetGValue(windowColor) + GetBValue(windowColor)) < 384;
}

void ReleaseThemeBrushes() {
    if (g_windowBrush) { DeleteObject(g_windowBrush); g_windowBrush = nullptr; }
    if (g_controlBrush) { DeleteObject(g_controlBrush); g_controlBrush = nullptr; }
}

void UpdateSystemTheme() {
    g_darkTheme = ReadSystemDarkTheme();
    ReleaseThemeBrushes();
    g_windowBrush = CreateSolidBrush(g_darkTheme ? RGB(16, 18, 20) : RGB(244, 245, 247));
    g_controlBrush = CreateSolidBrush(g_darkTheme ? RGB(32, 33, 36) : RGB(255, 255, 255));
    if (g_main) {
        HWND controls[] = {g_main, g_back, g_forward, g_refresh, g_home, g_address};
        for (HWND control : controls) if (control) { InvalidateRect(control, nullptr, TRUE); UpdateWindow(control); }
    }
    if (g_webview && g_homeVisible) {
        const wchar_t* script = g_darkTheme ? L"window.setPaneTheme && window.setPaneTheme(true);" : L"window.setPaneTheme && window.setPaneTheme(false);";
        g_webview->ExecuteScript(script, nullptr);
    }
}

void ApplyFullscreenState() {
    if (!g_main) return;
    bool shouldBeFullscreen = g_f11Fullscreen || g_videoFullscreen;
    if (shouldBeFullscreen == g_browserFullscreen) { ResizeChildren(); return; }
    if (shouldBeFullscreen) {
        g_fullscreenStyle = GetWindowLongPtrW(g_main, GWL_STYLE);
        g_fullscreenExStyle = GetWindowLongPtrW(g_main, GWL_EXSTYLE);
        g_fullscreenPlacement.length = sizeof(WINDOWPLACEMENT);
        GetWindowPlacement(g_main, &g_fullscreenPlacement);
        g_fullscreenSaved = true;
        HMONITOR monitor = MonitorFromWindow(g_main, MONITOR_DEFAULTTONEAREST);
        MONITORINFO info{sizeof(MONITORINFO)};
        if (monitor && GetMonitorInfoW(monitor, &info)) {
            SetWindowLongPtrW(g_main, GWL_STYLE, g_fullscreenStyle & ~static_cast<LONG_PTR>(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU | WS_SIZEBOX));
            SetWindowLongPtrW(g_main, GWL_EXSTYLE, g_fullscreenExStyle & ~static_cast<LONG_PTR>(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
            SetWindowPos(g_main, HWND_TOP, info.rcMonitor.left, info.rcMonitor.top, info.rcMonitor.right - info.rcMonitor.left, info.rcMonitor.bottom - info.rcMonitor.top, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        }
        g_browserFullscreen = true;
    } else {
        if (g_fullscreenSaved) {
            SetWindowLongPtrW(g_main, GWL_STYLE, g_fullscreenStyle);
            SetWindowLongPtrW(g_main, GWL_EXSTYLE, g_fullscreenExStyle);
            SetWindowPlacement(g_main, &g_fullscreenPlacement);
            SetWindowPos(g_main, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
        }
        g_browserFullscreen = false;
    }
    ResizeChildren();
    InvalidateRect(g_main, nullptr, TRUE);
}

void ToggleFullscreen() {
    g_f11Fullscreen = !g_f11Fullscreen;
    ApplyFullscreenState();
}

void ResizeChildren() {
    if (!g_main) return;
    RECT rc{}; GetClientRect(g_main, &rc); int width = rc.right - rc.left;
    if (g_browserFullscreen) {
        ShowWindow(g_back, SW_HIDE); ShowWindow(g_forward, SW_HIDE); ShowWindow(g_refresh, SW_HIDE); ShowWindow(g_home, SW_HIDE); ShowWindow(g_address, SW_HIDE);
        if (g_controller) { RECT bounds{0, 0, rc.right, rc.bottom}; g_controller->put_Bounds(bounds); g_controller->put_IsVisible(TRUE); }
        return;
    }
    ShowWindow(g_back, SW_SHOW); ShowWindow(g_forward, SW_SHOW); ShowWindow(g_refresh, SW_SHOW); ShowWindow(g_home, SW_SHOW); ShowWindow(g_address, SW_SHOW);
    MoveWindow(g_back, 6, 6, 32, 28, TRUE); MoveWindow(g_forward, 42, 6, 32, 28, TRUE); MoveWindow(g_refresh, 78, 6, 72, 28, TRUE); MoveWindow(g_home, 154, 6, 58, 28, TRUE); MoveWindow(g_address, 218, 6, width > 224 ? width - 224 : 160, 28, TRUE);
    if (g_controller) { RECT bounds{0, 40, rc.right, rc.bottom}; g_controller->put_Bounds(bounds); }
}

void RemoveEphemeralTree(const wchar_t* path) {
    if (!path || !*path) return;
    DWORD attrs = GetFileAttributesW(path); if (attrs == INVALID_FILE_ATTRIBUTES) return;
    if (attrs & FILE_ATTRIBUTE_REPARSE_POINT) { SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL); if (attrs & FILE_ATTRIBUTE_DIRECTORY) RemoveDirectoryW(path); else DeleteFileW(path); return; }
    if (!(attrs & FILE_ATTRIBUTE_DIRECTORY)) { SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL); DeleteFileW(path); return; }
    wchar_t pattern[MAX_PATH]{}; wcsncpy(pattern, path, _countof(pattern) - 3); pattern[_countof(pattern) - 3] = 0; wcscat(pattern, L"\\*");
    WIN32_FIND_DATAW data{}; HANDLE find = FindFirstFileW(pattern, &data);
    if (find != INVALID_HANDLE_VALUE) {
        do { if (wcscmp(data.cFileName, L".") && wcscmp(data.cFileName, L"..")) { wchar_t child[MAX_PATH]{}; wcsncpy(child, path, _countof(child) - 2); child[_countof(child) - 2] = 0; wcscat(child, L"\\"); wcscat(child, data.cFileName); RemoveEphemeralTree(child); } } while (FindNextFileW(find, &data));
        FindClose(find);
    }
    SetFileAttributesW(path, FILE_ATTRIBUTE_NORMAL); RemoveDirectoryW(path);
}

bool CreateEphemeralUserDataFolder() {
    wchar_t temp[MAX_PATH]{}; DWORD n = GetTempPathW(_countof(temp), temp); if (!n || n >= _countof(temp)) return false;
    wchar_t candidate[MAX_PATH]{}; if (!GetTempFileNameW(temp, L"PBM", 0, candidate)) return false; DeleteFileW(candidate);
    if (!CreateDirectoryW(candidate, nullptr)) return false; wcsncpy(g_ephemeralUserData, candidate, _countof(g_ephemeralUserData) - 1); return true;
}

void ShowHome() { if (!g_webview) return; g_homeVisible = true; g_lastUri[0] = 0; SetWindowTextW(g_address, L""); g_webview->NavigateToString(kDashboardHtml); }
void NavigateText(const wchar_t* text) { if (!g_webview) return; wchar_t uri[8192]{}; NormalizeInput(text, uri, _countof(uri)); if (!uri[0]) return; g_homeVisible = false; wcsncpy(g_lastUri, uri, _countof(g_lastUri) - 1); SetWindowTextW(g_address, uri); g_webview->Navigate(uri); }
void NavigateFromAddress() { wchar_t input[8192]{}; GetWindowTextW(g_address, input, _countof(input)); NavigateText(input); }
void HandleWebMessage(ICoreWebView2WebMessageReceivedEventArgs* args);

LRESULT CALLBACK AddressProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) { if (msg == WM_KEYDOWN && wp == VK_F11) { ToggleFullscreen(); return 0; } if (msg == WM_KEYDOWN && wp == VK_RETURN) { NavigateFromAddress(); return 0; } return g_oldAddressProc ? CallWindowProcW(g_oldAddressProc, hwnd, msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp); }

void CreateWebView() {
    if (!g_environment || g_controller) return;
    auto callback = Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>([](HRESULT hr, ICoreWebView2Controller* controller) -> HRESULT {
        if (FAILED(hr) || !controller) { MessageBoxW(g_main, L"WebView2 could not create the browser view.", L"Pane Browser Mini v7.0.0 Edition", MB_OK | MB_ICONERROR); return hr; }
        g_controller = controller; HRESULT getHr = g_controller->get_CoreWebView2(&g_webview); if (FAILED(getHr) || !g_webview) return getHr; g_controller->put_IsVisible(TRUE);
        auto acceleratorHandler = Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>([](ICoreWebView2Controller*, ICoreWebView2AcceleratorKeyPressedEventArgs* args) -> HRESULT { if (!args) return E_INVALIDARG; COREWEBVIEW2_KEY_EVENT_KIND kind{}; UINT key = 0; if (SUCCEEDED(args->get_KeyEventKind(&kind)) && SUCCEEDED(args->get_VirtualKey(&key)) && (kind == COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN || kind == COREWEBVIEW2_KEY_EVENT_KIND_SYSTEM_KEY_DOWN) && key == VK_F11) { args->put_Handled(TRUE); ToggleFullscreen(); } return S_OK; }); EventRegistrationToken acceleratorToken{}; g_controller->add_AcceleratorKeyPressed(acceleratorHandler, &acceleratorToken); acceleratorHandler->Release();
        auto messageHandler = Callback<ICoreWebView2WebMessageReceivedEventHandler>([](ICoreWebView2*, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT { HandleWebMessage(args); return S_OK; }); EventRegistrationToken messageToken{}; g_webview->add_WebMessageReceived(messageHandler, &messageToken); messageHandler->Release();
        auto newWindowHandler = Callback<ICoreWebView2NewWindowRequestedEventHandler>([](ICoreWebView2*, ICoreWebView2NewWindowRequestedEventArgs* args) -> HRESULT { if (!args) return E_INVALIDARG; args->put_Handled(TRUE); LPWSTR uri = nullptr; if (SUCCEEDED(args->get_Uri(&uri)) && uri) { NavigateText(uri); CoTaskMemFree(uri); } return S_OK; }); EventRegistrationToken newWindowToken{}; g_webview->add_NewWindowRequested(newWindowHandler, &newWindowToken); newWindowHandler->Release();
        auto sourceHandler = Callback<ICoreWebView2SourceChangedEventHandler>([](ICoreWebView2* sender, ICoreWebView2SourceChangedEventArgs*) -> HRESULT { LPWSTR source = nullptr; if (sender && SUCCEEDED(sender->get_Source(&source)) && source) { wcsncpy(g_lastUri, source, _countof(g_lastUri) - 1); g_lastUri[_countof(g_lastUri) - 1] = 0; SetWindowTextW(g_address, (wcsncmp(g_lastUri, L"about:blank", 10) == 0) ? L"" : g_lastUri); CoTaskMemFree(source); } return S_OK; }); EventRegistrationToken sourceToken{}; g_webview->add_SourceChanged(sourceHandler, &sourceToken); sourceHandler->Release();
        auto fullscreenHandler = Callback<ICoreWebView2ContainsFullScreenElementChangedEventHandler>([](ICoreWebView2* sender, IUnknown*) -> HRESULT { BOOL contains = FALSE; if (sender && SUCCEEDED(sender->get_ContainsFullScreenElement(&contains))) { g_videoFullscreen = contains != FALSE; ApplyFullscreenState(); } return S_OK; }); EventRegistrationToken fullscreenToken{}; g_webview->add_ContainsFullScreenElementChanged(fullscreenHandler, &fullscreenToken); fullscreenHandler->Release();
        g_homeVisible = true; ResizeChildren(); return g_webview->NavigateToString(kDashboardHtml);
    });
    HRESULT hr = g_environment->CreateCoreWebView2Controller(g_main, callback); callback->Release(); if (FAILED(hr)) MessageBoxW(g_main, L"WebView2 could not create the browser controller.", L"Pane Browser Mini v7.0.0 Edition", MB_OK | MB_ICONERROR);
}

HRESULT InitializeWebView() {
    if (!CreateEphemeralUserDataFolder()) return E_FAIL;
    auto callback = Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>([](HRESULT hr, ICoreWebView2Environment* environment) -> HRESULT { if (FAILED(hr) || !environment) { MessageBoxW(g_main, L"Microsoft Edge WebView2 Runtime is required.", L"Pane Browser Mini v7.0.0 Edition", MB_OK | MB_ICONERROR); return hr; } g_environment = environment; CreateWebView(); return S_OK; });
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(nullptr, g_ephemeralUserData, nullptr, callback); callback->Release(); return hr;
}

void HandleWebMessage(ICoreWebView2WebMessageReceivedEventArgs* args) { if (!args || !g_webview) return; LPWSTR raw = nullptr; if (FAILED(args->TryGetWebMessageAsString(&raw)) || !raw) return; if (wcsncmp(raw, L"search:", 7) == 0) NavigateText(raw + 7); CoTaskMemFree(raw); }

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_KEYDOWN: if (wp == VK_F11) { ToggleFullscreen(); return 0; } break;
    case WM_SETTINGCHANGE:
    case WM_THEMECHANGED: UpdateSystemTheme(); return 0;
    case WM_ERASEBKGND: if (g_windowBrush) { RECT client{}; GetClientRect(hwnd, &client); FillRect(reinterpret_cast<HDC>(wp), &client, g_windowBrush); return 1; } break;
    case WM_CTLCOLORBTN:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT: {
        HDC dc = reinterpret_cast<HDC>(wp);
        SetTextColor(dc, g_darkTheme ? RGB(241, 243, 244) : RGB(31, 35, 40));
        SetBkColor(dc, g_darkTheme ? RGB(32, 33, 36) : RGB(255, 255, 255));
        return reinterpret_cast<LRESULT>(g_controlBrush ? g_controlBrush : g_windowBrush);
    }
    case WM_SIZE: ResizeChildren(); return 0;
    case WM_COMMAND: switch (LOWORD(wp)) { case kBackId: if (g_webview) g_webview->GoBack(); return 0; case kForwardId: if (g_webview) g_webview->GoForward(); return 0; case kRefreshId: if (g_webview) g_webview->Reload(); return 0; case kHomeId: ShowHome(); return 0; case kAddressId: if (HIWORD(wp) == EN_UPDATE) return 0; break; } break;
    case WM_DESTROY: ReleaseThemeBrushes(); g_webview.Reset(); g_controller.Reset(); g_environment.Reset(); PostQuitMessage(0); return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    SetProcessDPIAware(); HRESULT comHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED); if (FAILED(comHr)) return 1;
    WNDCLASSW wc{}; wc.hInstance = instance; wc.lpfnWndProc = WindowProc; wc.lpszClassName = kWindowClass; wc.hCursor = LoadCursorW(nullptr, IDC_ARROW); wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1); RegisterClassW(&wc);
    g_main = CreateWindowExW(0, kWindowClass, L"Pane Browser Mini v7.0.0 Edition", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1100, 760, nullptr, nullptr, instance, nullptr); if (!g_main) { CoUninitialize(); return 1; }
    g_back = CreateWindowExW(0, L"BUTTON", L"<", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kBackId), instance, nullptr); g_forward = CreateWindowExW(0, L"BUTTON", L">", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kForwardId), instance, nullptr); g_refresh = CreateWindowExW(0, L"BUTTON", L"Refresh", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kRefreshId), instance, nullptr); g_home = CreateWindowExW(0, L"BUTTON", L"Home", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kHomeId), instance, nullptr); g_address = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kAddressId), instance, nullptr); g_oldAddressProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(g_address, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(AddressProc)));
    UpdateSystemTheme(); ShowWindow(g_main, show); ResizeChildren(); UpdateWindow(g_main); HRESULT hr = InitializeWebView();
    if (FAILED(hr)) { DestroyWindow(g_main); CoUninitialize(); for (int attempt = 0; attempt < 20 && g_ephemeralUserData[0]; ++attempt) { RemoveEphemeralTree(g_ephemeralUserData); if (GetFileAttributesW(g_ephemeralUserData) == INVALID_FILE_ATTRIBUTES) break; Sleep(150); } return 1; }
    MSG msg{}; while (GetMessageW(&msg, nullptr, 0, 0) > 0) { TranslateMessage(&msg); DispatchMessageW(&msg); }
    CoUninitialize(); for (int attempt = 0; attempt < 20 && g_ephemeralUserData[0]; ++attempt) { RemoveEphemeralTree(g_ephemeralUserData); if (GetFileAttributesW(g_ephemeralUserData) == INVALID_FILE_ATTRIBUTES) break; Sleep(150); }
    return static_cast<int>(msg.wParam);
}
