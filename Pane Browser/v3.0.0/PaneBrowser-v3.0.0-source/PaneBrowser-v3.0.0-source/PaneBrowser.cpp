#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <windows.h>
#include <windowsx.h>
#include <wrl.h>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include <utility>
#include <ctime>
#include <cwchar>
#include <functional>
#include <atomic>
#include <memory>
#include <cctype>
#include <fstream>
#include <sstream>
#include <set>
#include <shellapi.h>
#include <commdlg.h>
#include <wininet.h>
#include "resource.h"
#include "v3_data.h"
#include "sdk/webview2/build/native/include/WebView2.h"

using Microsoft::WRL::ComPtr;

template <typename Interface, typename Fn> class CallbackHolder;

template <typename Fn>
class CallbackHolder<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler, Fn>
    : public ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler) { *out = nullptr; return E_NOINTERFACE; }
        *out = static_cast<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG r = InterlockedDecrement(&refs_);
        if (!r) delete this;
        return r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, ICoreWebView2Environment* env) override { return fn_(hr, env); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler, Fn>
    : public ICoreWebView2CreateCoreWebView2ControllerCompletedHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2CreateCoreWebView2ControllerCompletedHandler) { *out = nullptr; return E_NOINTERFACE; }
        *out = static_cast<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG r = InterlockedDecrement(&refs_);
        if (!r) delete this;
        return r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, ICoreWebView2Controller* controller) override { return fn_(hr, controller); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2WebMessageReceivedEventHandler, Fn>
    : public ICoreWebView2WebMessageReceivedEventHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2WebMessageReceivedEventHandler) { *out = nullptr; return E_NOINTERFACE; }
        *out = static_cast<ICoreWebView2WebMessageReceivedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG r = InterlockedDecrement(&refs_);
        if (!r) delete this;
        return r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) override {
        return fn_(sender, args);
    }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2ContextMenuRequestedEventHandler, Fn> : public ICoreWebView2ContextMenuRequestedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ContextMenuRequestedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ContextMenuRequestedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2ContextMenuRequestedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2NewWindowRequestedEventHandler, Fn>
    : public ICoreWebView2NewWindowRequestedEventHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2NewWindowRequestedEventHandler) { *out = nullptr; return E_NOINTERFACE; }
        *out = static_cast<ICoreWebView2NewWindowRequestedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG r = InterlockedDecrement(&refs_);
        if (!r) delete this;
        return r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) override {
        return fn_(sender, args);
    }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2NavigationCompletedEventHandler, Fn>
    : public ICoreWebView2NavigationCompletedEventHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        if (riid != IID_IUnknown && riid != IID_ICoreWebView2NavigationCompletedEventHandler) { *out = nullptr; return E_NOINTERFACE; }
        *out = static_cast<ICoreWebView2NavigationCompletedEventHandler*>(this);
        AddRef();
        return S_OK;
    }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override {
        ULONG r = InterlockedDecrement(&refs_);
        if (!r) delete this;
        return r;
    }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) override {
        return fn_(sender, args);
    }
};


template <typename Fn>
class CallbackHolder<ICoreWebView2ExecuteScriptCompletedHandler, Fn> : public ICoreWebView2ExecuteScriptCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ExecuteScriptCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ExecuteScriptCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr, LPCWSTR result) override { return fn_(hr, result); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2DownloadStartingEventHandler, Fn> : public ICoreWebView2DownloadStartingEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2DownloadStartingEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2DownloadStartingEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2DownloadStartingEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2ClearBrowsingDataCompletedHandler, Fn> : public ICoreWebView2ClearBrowsingDataCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ClearBrowsingDataCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ClearBrowsingDataCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT hr) override { return fn_(hr); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2AcceleratorKeyPressedEventHandler, Fn> : public ICoreWebView2AcceleratorKeyPressedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2AcceleratorKeyPressedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2AcceleratorKeyPressedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2Controller* controller, ICoreWebView2AcceleratorKeyPressedEventArgs* args) override { return fn_(controller, args); }
};

template <typename Interface, typename Fn>
struct CallbackResult {
    Interface* ptr;
    Interface* Get() const { return ptr; }
};

template <typename Interface, typename Fn>
CallbackResult<Interface, Fn> Callback(Fn fn) {
    return {new CallbackHolder<Interface, Fn>(std::move(fn))};
}

namespace {
constexpr int kBackId = 0x65;
constexpr int kForwardId = 0x66;
constexpr int kReloadId = 0x67;
constexpr int kHomeId = 0x68;
constexpr int kHistoryId = 0x69;
constexpr int kBookmarksId = 0x6b;
constexpr int kSettingsId = 0x6c;
constexpr int kPrivateId = 0x6d;
constexpr int kDownloadsId = 0x6e;
constexpr int kFindId = 0x6f;
constexpr int kOverflowId = 0x71;
constexpr int kContextNewTab = 0x5201;
constexpr int kContextReopen = 0x5202;
constexpr int kContextFind = 0x5203;
constexpr int kContextZoomIn = 0x5204;
constexpr int kContextZoomOut = 0x5205;
constexpr int kContextZoomReset = 0x5206;
constexpr int kContextClearData = 0x5207;
constexpr int kContextDownloads = 0x5208;
constexpr int kContextBookmarks = 0x5209;
constexpr int kContextHistory = 0x520A;
constexpr int kAddressId = 0x6a;
constexpr int kTabStripId = 0x70;
constexpr int kTabWidth = 174;
constexpr UINT kThemeRefreshTimerId = 0x5A7;
constexpr UINT kUpdateResultMessage = WM_APP + 0x31;

struct BrowserTab {
    ComPtr<ICoreWebView2Controller> controller;
    ComPtr<ICoreWebView2> webview;
    std::wstring lastUri = L"about:blank";
    bool internalPage = true;
    bool historyPage = false;
    bool bookmarksPage = false;
    bool settingsPage = false;
    bool downloadsPage = false;
    bool privateMode = false;
};

struct HistoryEntry {
    std::wstring uri;
    std::wstring time;
};

HWND g_main = nullptr;
HWND g_tabStrip = nullptr;
HWND g_back = nullptr;
HWND g_forward = nullptr;
HWND g_reload = nullptr;
HWND g_home = nullptr;
HWND g_historyButton = nullptr;
HWND g_bookmarksButton = nullptr;
HWND g_settingsButton = nullptr;
HWND g_privateButton = nullptr;
HWND g_address = nullptr;
WNDPROC g_oldEditProc = nullptr;
ComPtr<ICoreWebView2Environment> g_environment;
std::vector<std::shared_ptr<BrowserTab>> g_tabs;
std::vector<HistoryEntry> g_history;
int g_activeTab = -1;
bool g_darkMode = false;
std::wstring g_dataDirectory;
v3data::Settings g_settings;
std::vector<v3data::Bookmark> g_bookmarks;
std::vector<std::wstring> g_closedTabs;
std::vector<v3data::SessionEntry> g_restoreSession;
int g_restoreActive = 0;
struct ActiveDownload { ComPtr<ICoreWebView2DownloadOperation> operation; v3data::DownloadRecord record; bool privateMode = false; };
std::vector<ActiveDownload> g_downloads;
HWND g_downloadsButton = nullptr;
HWND g_findButton = nullptr;
HWND g_overflowButton = nullptr;
UINT_PTR g_downloadTimer = 0;
std::atomic_bool g_updateCheckRunning{false};

const wchar_t* kDashboardHtmlDark = LR"HTML(<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Pane Browser</title><style>
html,body{height:100%;}body{margin:0;background:#202124;color:#f1f3f4;font-family:Segoe UI,system-ui,sans-serif}.shell{height:100%;display:flex;align-items:center;justify-content:center;padding:24px;box-sizing:border-box}.content{width:min(640px,100%);display:flex;flex-direction:column;align-items:center;gap:24px}h1{margin:0;font-size:30px;font-weight:600;letter-spacing:.1px;color:#f1f3f4}.search{width:100%;height:66px;display:flex;align-items:center;background:#303134;border:1px solid #5f6368;border-radius:4px;box-shadow:0 2px 6px rgba(0,0,0,.35);box-sizing:border-box}.search input{flex:1;min-width:0;height:100%;padding:0 20px;border:0;outline:0;background:transparent;color:#f1f3f4;font:inherit;font-size:18px}.search input::placeholder{color:#9aa0a6}.search input[type=search]::-webkit-search-cancel-button{-webkit-appearance:none;display:none}.search button{width:62px;height:100%;display:grid;place-items:center;border:0;background:transparent;color:#bdc1c6;cursor:pointer}.search button:hover{color:#f1f3f4}.search svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2.5;stroke-linecap:round}
</style></head><body><main class="shell"><section class="content"><h1>Pane Browser</h1><form class="search" onsubmit="submitSearch();return false;"><input id="q" type="search" autocomplete="off" autofocus><button type="submit" aria-label="Search"><svg viewBox="0 0 24 24"><circle cx="10.8" cy="10.8" r="6.8"></circle><path d="M16 16l5 5"></path></svg></button></form></section></main><script>function submitSearch(){const q=document.getElementById('q').value.trim();if(!q)return;const uri='https://www.startpage.com/sp/search?query='+encodeURIComponent(q);if(window.chrome&&chrome.webview)chrome.webview.postMessage('search:'+q);else window.location.href=uri;}</script></body></html>)HTML";

const wchar_t* kDashboardHtmlLight = LR"HTML(<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Pane Browser</title><style>
html,body{height:100%;}body{margin:0;background:#ffffff;color:#202124;font-family:Segoe UI,system-ui,sans-serif}.shell{height:100%;display:flex;align-items:center;justify-content:center;padding:24px;box-sizing:border-box}.content{width:min(640px,100%);display:flex;flex-direction:column;align-items:center;gap:24px}h1{margin:0;font-size:30px;font-weight:600;letter-spacing:.1px;color:#202124}.search{width:100%;height:66px;display:flex;align-items:center;background:#ffffff;border:1px solid #dadce0;border-radius:4px;box-shadow:0 2px 6px rgba(60,64,67,.18);box-sizing:border-box}.search input{flex:1;min-width:0;height:100%;padding:0 20px;border:0;outline:0;background:transparent;color:#202124;font:inherit;font-size:18px}.search input::placeholder{color:#9aa0a6}.search input[type=search]::-webkit-search-cancel-button{-webkit-appearance:none;display:none}.search button{width:62px;height:100%;display:grid;place-items:center;border:0;background:transparent;color:#5f6368;cursor:pointer}.search button:hover{color:#202124}.search svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2.5;stroke-linecap:round}
</style></head><body><main class="shell"><section class="content"><h1>Pane Browser</h1><form class="search" onsubmit="submitSearch();return false;"><input id="q" type="search" autocomplete="off" autofocus><button type="submit" aria-label="Search"><svg viewBox="0 0 24 24"><circle cx="10.8" cy="10.8" r="6.8"></circle><path d="M16 16l5 5"></path></svg></button></form></section></main><script>function submitSearch(){const q=document.getElementById('q').value.trim();if(!q)return;const uri='https://www.startpage.com/sp/search?query='+encodeURIComponent(q);if(window.chrome&&chrome.webview)chrome.webview.postMessage('search:'+q);else window.location.href=uri;}</script></body></html>)HTML";

bool DetectWindowsDarkMode() {
    if (g_settings.theme == L"dark") return true;
    if (g_settings.theme == L"light") return false;
    HKEY key = nullptr;
    DWORD value = 1;
    DWORD size = sizeof(value);
    if (RegOpenKeyExW(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize", 0, KEY_READ, &key) == ERROR_SUCCESS) {
        RegQueryValueExW(key, L"AppsUseLightTheme", nullptr, nullptr, reinterpret_cast<LPBYTE>(&value), &size);
        RegCloseKey(key);
    }
    return value == 0;
}

const wchar_t* DashboardHtml() {
    return g_darkMode ? kDashboardHtmlDark : kDashboardHtmlLight;
}

void ApplyWindowTheme(HWND hwnd) {
    HMODULE dwm = LoadLibraryW(L"dwmapi.dll");
    if (!dwm) return;
    using DwmSetWindowAttributeFn = HRESULT (WINAPI*)(HWND, DWORD, LPCVOID, DWORD);
    auto setAttribute = reinterpret_cast<DwmSetWindowAttributeFn>(GetProcAddress(dwm, "DwmSetWindowAttribute"));
    if (setAttribute) {
        BOOL useDark = g_darkMode ? TRUE : FALSE;
        setAttribute(hwnd, 20, &useDark, sizeof(useDark));
        setAttribute(hwnd, 19, &useDark, sizeof(useDark));
    }
    FreeLibrary(dwm);
}

void ApplyControlTheme() {
    HMODULE theme = LoadLibraryW(L"uxtheme.dll");
    if (!theme) return;
    using SetWindowThemeFn = HRESULT (WINAPI*)(HWND, LPCWSTR, LPCWSTR);
    auto setTheme = reinterpret_cast<SetWindowThemeFn>(GetProcAddress(theme, "SetWindowTheme"));
    if (setTheme) {
        const wchar_t* visualStyle = g_darkMode ? L"DarkMode_Explorer" : nullptr;
        HWND controls[] = {g_tabStrip, g_back, g_forward, g_reload, g_home, g_historyButton, g_bookmarksButton, g_settingsButton, g_privateButton, g_downloadsButton, g_findButton, g_overflowButton, g_address};
        for (HWND control : controls) {
            if (control) setTheme(control, visualStyle, nullptr);
        }
    }
    FreeLibrary(theme);
}

HBRUSH ToolbarBackgroundBrush() {
    static HBRUSH darkBrush = CreateSolidBrush(RGB(28, 30, 35));
    static HBRUSH lightBrush = CreateSolidBrush(RGB(255, 255, 255));
    return g_darkMode ? darkBrush : lightBrush;
}

HBRUSH ToolbarButtonBrush() {
    static HBRUSH darkBrush = CreateSolidBrush(RGB(63, 65, 71));
    static HBRUSH lightBrush = CreateSolidBrush(RGB(240, 240, 240));
    return g_darkMode ? darkBrush : lightBrush;
}

void DrawToolbarButton(const DRAWITEMSTRUCT* item) {
    FillRect(item->hDC, &item->rcItem, ToolbarButtonBrush());
    COLORREF border = g_darkMode ? RGB(95, 98, 105) : RGB(130, 130, 130);
    HBRUSH borderBrush = CreateSolidBrush(border);
    FrameRect(item->hDC, &item->rcItem, borderBrush);
    DeleteObject(borderBrush);
    SetBkMode(item->hDC, TRANSPARENT);
    SetTextColor(item->hDC, g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
    wchar_t label[64] = {};
    GetWindowTextW(item->hwndItem, label, 63);
    RECT textRect = item->rcItem;
    DrawTextW(item->hDC, label, -1, &textRect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
}

void SavePersistentData() {
    if (g_dataDirectory.empty()) g_dataDirectory = v3data::DataDirectory();
    if (g_dataDirectory.empty()) { OutputDebugStringW(L"Pane Browser: no writable data directory.\n"); return; }
    bool ok = true;
    if (g_settings.persistHistory) {
        std::vector<v3data::HistoryEntry> out;
        for (const auto& e : g_history) out.push_back({e.uri, e.time});
        ok = v3data::SaveHistory(out, g_dataDirectory) && ok;
    }
    std::vector<v3data::SessionEntry> session;
    int normalActive = 0, normalIndex = 0;
    for (size_t i = 0; i < g_tabs.size(); ++i) { const auto& tab = g_tabs[i]; if (!tab->privateMode) { session.push_back({tab->lastUri, false}); if (static_cast<int>(i) == g_activeTab) normalActive = normalIndex; ++normalIndex; } }
    if (g_settings.restoreSession) ok = v3data::SaveSession(session, normalActive, g_dataDirectory) && ok;
    ok = v3data::SaveBookmarks(g_bookmarks, g_dataDirectory) && ok;
    ok = v3data::SaveDownloads([&] { std::vector<v3data::DownloadRecord> out; for (const auto& d : g_downloads) if (!d.privateMode) out.push_back(d.record); return out; }(), g_dataDirectory) && ok;
    ok = v3data::SaveSettings(g_settings, g_dataDirectory) && ok;
    if (!ok) OutputDebugStringW(L"Pane Browser: one or more local data files could not be written.\n");
}

void LoadPersistentData() {
    g_dataDirectory = v3data::DataDirectory();
    v3data::LoadSettings(g_settings, g_dataDirectory);
    g_history.clear();
    std::vector<v3data::HistoryEntry> history;
    if (g_settings.persistHistory) { v3data::LoadHistory(history, g_dataDirectory); for (const auto& e : history) g_history.push_back({e.uri, e.time}); } else { v3data::ClearHistory(g_dataDirectory); }
    v3data::LoadBookmarks(g_bookmarks, g_dataDirectory);
    g_downloads.clear(); std::vector<v3data::DownloadRecord> savedDownloads; v3data::LoadDownloads(savedDownloads, g_dataDirectory); for (const auto& d : savedDownloads) g_downloads.push_back({ComPtr<ICoreWebView2DownloadOperation>(), d, false});
    g_restoreSession.clear(); g_restoreActive = 0;
    if (g_settings.restoreSession) v3data::LoadSession(g_restoreSession, g_restoreActive, g_dataDirectory); else v3data::ClearSession(g_dataDirectory);
}

std::wstring GetWebViewUserDataFolder() {
    std::wstring root = g_dataDirectory.empty() ? v3data::DataDirectory() : g_dataDirectory;
    std::wstring webviewFolder = v3data::Join(root, L"WebView2");
    CreateDirectoryW(webviewFolder.c_str(), nullptr);
    return webviewFolder;
    /*
    wchar_t modulePath[32768] = {};
    DWORD length = GetModuleFileNameW(nullptr, modulePath, static_cast<DWORD>(sizeof(modulePath) / sizeof(modulePath[0])));
    if (!length || length >= sizeof(modulePath) / sizeof(modulePath[0])) return L"";
    std::wstring fullPath(modulePath, length);
    size_t slash = fullPath.find_last_of(L"\\/");
    std::wstring directory = slash == std::wstring::npos ? L"." : fullPath.substr(0, slash + 1);
    std::wstring executableName = slash == std::wstring::npos ? fullPath : fullPath.substr(slash + 1);
    return directory + executableName + L".WebView2";
    */
}

std::wstring UrlEncode(const std::wstring& input) {
    if (input.empty()) return L"";
    int bytesNeeded = WideCharToMultiByte(CP_UTF8, 0, input.c_str(), static_cast<int>(input.size()), nullptr, 0, nullptr, nullptr);
    if (bytesNeeded <= 0) return input;
    std::string utf8(static_cast<size_t>(bytesNeeded), '\0');
    WideCharToMultiByte(CP_UTF8, 0, input.c_str(), static_cast<int>(input.size()), utf8.data(), bytesNeeded, nullptr, nullptr);
    static const wchar_t hex[] = L"0123456789ABCDEF";
    std::wstring encoded;
    for (unsigned char c : utf8) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
            encoded.push_back(static_cast<wchar_t>(c));
        } else {
            encoded.push_back(L'%');
            encoded.push_back(hex[(c >> 4) & 0x0F]);
            encoded.push_back(hex[c & 0x0F]);
        }
    }
    return encoded;
}

std::wstring HtmlEscape(const std::wstring& value);
struct SearchEngineOption { const wchar_t* id; const wchar_t* name; const wchar_t* templateUrl; };
const SearchEngineOption kSearchEngines[] = {
    {L"google", L"Google", L"https://www.google.com/search?q="},
    {L"bing", L"Bing", L"https://www.bing.com/search?q="},
    {L"duckduckgo", L"DuckDuckGo", L"https://duckduckgo.com/?q="},
    {L"brave", L"Brave Search", L"https://search.brave.com/search?q="},
    {L"startpage", L"Startpage", L"https://www.startpage.com/sp/search?query="},
    {L"mojeek", L"Mojeek", L"https://www.mojeek.com/search?q="},
    {L"swisscows", L"Swisscows", L"https://swisscows.com/en/web?query="},
    {L"yahoo", L"Yahoo", L"https://search.yahoo.com/search?p="},
    {L"baidu", L"Baidu", L"https://www.baidu.com/s?wd="},
    {L"naver", L"Naver", L"https://search.naver.com/search.naver?query="},
    {L"yandex", L"Yandex", L"https://yandex.com/search/?text="},
    {L"ecosia", L"Ecosia", L"https://www.ecosia.org/search?q="},
    {L"qwant", L"Qwant", L"https://www.qwant.com/?q="},
    {L"seznam", L"Seznam", L"https://search.seznam.cz/?q="},
    {L"ask", L"Ask.com", L"https://www.ask.com/web?q="},
    {L"aol", L"AOL Search", L"https://search.aol.com/aol/search?q="},
    {L"kagi", L"Kagi", L"https://kagi.com/search?q="},
    {L"metager", L"MetaGer", L"https://metager.org/meta/meta.ger3?eingabe="},
    {L"searxng", L"SearXNG", L"https://search.bus-hit.me/search?q="},
    {L"you", L"You.com", L"https://you.com/search?q="},
    {L"perplexity", L"Perplexity", L"https://www.perplexity.ai/search?q="},
    {L"yep", L"Yep", L"https://yep.com/web?q="},
    {L"sogou", L"Sogou", L"https://www.sogou.com/web?query="},
    {L"360", L"360 Search", L"https://www.so.com/s?q="},
    {L"rambler", L"Rambler", L"https://nova.rambler.ru/search?query="},
    {L"daum", L"Daum", L"https://search.daum.net/search?q="},
    {L"goo", L"Goo", L"https://search.goo.ne.jp/web.jsp?MT="},
    {L"excite", L"Excite", L"https://results.excite.com/serp?q="},
    {L"dogpile", L"Dogpile", L"https://www.dogpile.com/serp?q="},
    {L"gibiru", L"Gibiru", L"https://gibiru.com/results.html?q="},
    {L"wolframalpha", L"WolframAlpha", L"https://www.wolframalpha.com/input?i="},
    {L"custom", L"Custom URL", L""}
};
const SearchEngineOption* FindSearchEngine(const std::wstring& id) {
    for (const auto& item : kSearchEngines) if (id == item.id) return &item;
    return nullptr;
}
std::wstring SearchEngineOptionsHtml() {
    std::wstring html;
    for (const auto& item : kSearchEngines) html += L"<option value='" + std::wstring(item.id) + L"'" + (g_settings.searchEngine == item.id ? L" selected" : L"") + L">" + HtmlEscape(item.name) + L"</option>";
    return html;
}
std::wstring SearchUrlForQuery(const std::wstring& query) {
    std::wstring encoded = UrlEncode(query);
    if (g_settings.searchEngine == L"custom" && !g_settings.customSearchUrl.empty()) {
        std::wstring custom = g_settings.customSearchUrl;
        size_t marker = custom.find(L"{query}");
        if (marker != std::wstring::npos) { custom.replace(marker, 7, encoded); return custom; }
        marker = custom.find(L"%s");
        if (marker != std::wstring::npos) { custom.replace(marker, 2, encoded); return custom; }
        return custom + encoded;
    }
    const auto* item = FindSearchEngine(g_settings.searchEngine);
    if (!item || !item->templateUrl[0]) item = FindSearchEngine(L"startpage");
    return std::wstring(item->templateUrl) + encoded;
}

std::wstring NormalizeInput(const std::wstring& input) {
    size_t first = input.find_first_not_of(L" \t\r\n"), last = input.find_last_not_of(L" \t\r\n");
    if (first == std::wstring::npos) return L"";
    std::wstring value = input.substr(first, last - first + 1);
    if (value.rfind(L"http://", 0) == 0 || value.rfind(L"https://", 0) == 0) return value;
    if (value.find_first_of(L" \t\r\n") == std::wstring::npos && value.find(L'.') != std::wstring::npos) return L"https://" + value;
    return SearchUrlForQuery(value);
}

std::wstring HtmlEscape(const std::wstring& value) {
    std::wstring out;
    for (wchar_t c : value) {
        switch (c) {
        case L'&': out += L"&amp;"; break;
        case L'<': out += L"&lt;"; break;
        case L'>': out += L"&gt;"; break;
        case L'\"': out += L"&quot;"; break;
        case 39: out += L"&#39;"; break;
        default: out += c; break;
        }
    }
    return out;
}

int FindTab(const BrowserTab* tab) {
    for (size_t i = 0; i < g_tabs.size(); ++i) {
        if (g_tabs[i].get() == tab) return static_cast<int>(i);
    }
    return -1;
}

BrowserTab* ActiveTab() {
    if (g_activeTab < 0 || g_activeTab >= static_cast<int>(g_tabs.size())) return nullptr;
    return g_tabs[g_activeTab].get();
}

ICoreWebView2* ActiveWebView() {
    BrowserTab* tab = ActiveTab();
    return tab ? tab->webview.Get() : nullptr;
}

std::wstring TabTitle(const BrowserTab& tab) {
    if (tab.lastUri.empty() || tab.lastUri == L"about:blank") return tab.privateMode ? L"Private Tab" : L"New Tab";
    std::wstring title = tab.privateMode ? L"Private - " : L"";
    title += tab.lastUri;
    if (title.size() > 22) title = title.substr(0, 19) + L"...";
    return title;
}

bool GetTabRect(int index, RECT& rect) {
    if (index < 0 || index >= static_cast<int>(g_tabs.size())) return false;
    rect.left = index * kTabWidth + 4;
    rect.top = 3;
    rect.right = rect.left + kTabWidth - 8;
    rect.bottom = 30;
    return true;
}

void InvalidateTabs() {
    if (g_tabStrip) InvalidateRect(g_tabStrip, nullptr, TRUE);
}

void SetTabLabel(BrowserTab* tab) {
    (void)tab;
    InvalidateTabs();
}

void ResizeChildren();
void CreateNewTab(const std::wstring& initialUri, bool privateMode = false);
void CloseTab(int index);
void ShowBookmarks();
void ShowSettings();
void SetActiveTab(int index);
void ShowDownloads();
void FindInPage();
void UpdateDownloadStates();
void ClearBrowsingData();
void ReopenClosedTab();
void ExportBookmarks();
void ImportBookmarks();
void CheckForUpdates();
void ShowOverflowMenu();
void ShowNativeContextMenu(POINT point);
void ConfigureProfilePrivacy(BrowserTab* tab);
bool HandleShortcut(UINT vk, bool ctrl, bool shift, bool alt);

LRESULT CALLBACK TabStripProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_ERASEBKGND:
        return 1;
    case WM_PAINT: {
        PAINTSTRUCT ps{};
        HDC dc = BeginPaint(hwnd, &ps);
        RECT client{};
        GetClientRect(hwnd, &client);
        HBRUSH background = CreateSolidBrush(g_darkMode ? RGB(28, 30, 35) : RGB(255, 255, 255));
        FillRect(dc, &client, background);
        DeleteObject(background);
        SetBkMode(dc, TRANSPARENT);
        HFONT font = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
        HFONT oldFont = static_cast<HFONT>(SelectObject(dc, font));
        for (int i = 0; i < static_cast<int>(g_tabs.size()); ++i) {
            RECT tabRect{};
            GetTabRect(i, tabRect);
            COLORREF color = g_darkMode ? RGB(63, 65, 71) : RGB(225, 225, 225);
            HBRUSH tabBrush = CreateSolidBrush(color);
            FillRect(dc, &tabRect, tabBrush);
            DeleteObject(tabBrush);
            SetTextColor(dc, g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
            RECT titleRect = tabRect;
            titleRect.left += 12;
            titleRect.right -= 34;
            DrawTextW(dc, TabTitle(*g_tabs[i]).c_str(), -1, &titleRect, DT_SINGLELINE | DT_VCENTER | DT_LEFT | DT_END_ELLIPSIS);
            RECT closeRect = tabRect;
            closeRect.left = tabRect.right - 34;
            closeRect.right = tabRect.right - 6;
            HBRUSH closeBrush = CreateSolidBrush(g_darkMode ? RGB(63, 65, 71) : RGB(225, 225, 225));
            FillRect(dc, &closeRect, closeBrush);
            DeleteObject(closeBrush);
            HPEN whitePen = CreatePen(PS_SOLID, 2, g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
            HPEN oldPen = static_cast<HPEN>(SelectObject(dc, whitePen));
            int x1 = closeRect.left + 7;
            int x2 = closeRect.right - 7;
            int y1 = closeRect.top + 7;
            int y2 = closeRect.bottom - 7;
            MoveToEx(dc, x1, y1, nullptr); LineTo(dc, x2, y2);
            MoveToEx(dc, x2, y1, nullptr); LineTo(dc, x1, y2);
            SelectObject(dc, oldPen);
            DeleteObject(whitePen);
        }
        if (!g_settings.tabsEnabled) { SelectObject(dc, oldFont); EndPaint(hwnd, &ps); return 0; }
        int plusLeft = static_cast<int>(g_tabs.size()) * kTabWidth + 6;
        RECT plusRect{plusLeft, 2, plusLeft + 36, 31};
        HBRUSH plusBrush = CreateSolidBrush(g_darkMode ? RGB(63, 65, 71) : RGB(225, 225, 225));
        FillRect(dc, &plusRect, plusBrush);
        DeleteObject(plusBrush);
        HPEN whitePen = CreatePen(PS_SOLID, 2, g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
        HPEN oldPen = static_cast<HPEN>(SelectObject(dc, whitePen));
        int cx = (plusRect.left + plusRect.right) / 2;
        int cy = (plusRect.top + plusRect.bottom) / 2;
        MoveToEx(dc, cx - 7, cy, nullptr); LineTo(dc, cx + 7, cy);
        MoveToEx(dc, cx, cy - 7, nullptr); LineTo(dc, cx, cy + 7);
        SelectObject(dc, oldPen);
        DeleteObject(whitePen);
        SelectObject(dc, oldFont);
        EndPaint(hwnd, &ps);
        return 0;
    }
    case WM_LBUTTONDOWN: {
        int x = GET_X_LPARAM(lp);
        int y = GET_Y_LPARAM(lp);
        for (int i = 0; i < static_cast<int>(g_tabs.size()); ++i) {
            RECT tabRect{};
            GetTabRect(i, tabRect);
            if (PtInRect(&tabRect, POINT{x, y})) {
                if (x >= tabRect.right - 34) CloseTab(i);
                else SetActiveTab(i);
                return 0;
            }
        }
        int plusLeft = static_cast<int>(g_tabs.size()) * kTabWidth + 6;
        if (g_settings.tabsEnabled && x >= plusLeft && x < plusLeft + 42 && y >= 0 && y < 32) CreateNewTab(L"dashboard", false);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

void SetActiveTab(int index) {
    if (index < 0 || index >= static_cast<int>(g_tabs.size())) return;
    g_activeTab = index;
    for (size_t i = 0; i < g_tabs.size(); ++i) {
        if (g_tabs[i]->controller) g_tabs[i]->controller->put_IsVisible(i == static_cast<size_t>(g_activeTab) ? TRUE : FALSE);
    }
    BrowserTab* tab = ActiveTab();
    if (g_address && tab) SetWindowTextW(g_address, tab->lastUri == L"about:blank" ? L"" : tab->lastUri.c_str());
    InvalidateTabs();
    ResizeChildren();
}

void CloseTab(int index) {
    if (index < 0 || index >= static_cast<int>(g_tabs.size())) return;
    if (g_tabs.size() == 1) {
        BrowserTab* tab = g_tabs[0].get();
        if (!tab->privateMode && (tab->lastUri.rfind(L"http://", 0) == 0 || tab->lastUri.rfind(L"https://", 0) == 0)) { g_closedTabs.push_back(tab->lastUri); if (g_closedTabs.size() > 10) g_closedTabs.erase(g_closedTabs.begin()); }
        tab->lastUri = L"about:blank";
        tab->internalPage = true;
        tab->historyPage = false;
        tab->bookmarksPage = false;
        tab->settingsPage = false;
        tab->downloadsPage = false;
        tab->downloadsPage = false;
        tab->privateMode = false;
        if (tab->webview) tab->webview->NavigateToString(DashboardHtml());
        if (g_address) SetWindowTextW(g_address, L"");
        SetTabLabel(tab);
        SetActiveTab(0);
        SavePersistentData();
        return;
    }
    if (!g_tabs[index]->privateMode && (g_tabs[index]->lastUri.rfind(L"http://", 0) == 0 || g_tabs[index]->lastUri.rfind(L"https://", 0) == 0)) { g_closedTabs.push_back(g_tabs[index]->lastUri); if (g_closedTabs.size() > 10) g_closedTabs.erase(g_closedTabs.begin()); }
    g_tabs[index]->webview.Reset();
    g_tabs[index]->controller.Reset();
    g_tabs.erase(g_tabs.begin() + index);
    if (g_activeTab > index) --g_activeTab;
    else if (g_activeTab >= static_cast<int>(g_tabs.size())) g_activeTab = static_cast<int>(g_tabs.size()) - 1;
    SetActiveTab(g_activeTab);
    SavePersistentData();
}

std::wstring CurrentTimeLabel() {
    std::time_t now = std::time(nullptr);
    std::tm* local = std::localtime(&now);
    if (!local) return L"";
    wchar_t buffer[16] = {};
    std::wcsftime(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%H.%M", local);
    return buffer;
}

std::wstring HistoryHost(const std::wstring& uri) {
    size_t start = uri.find(L"://");
    start = start == std::wstring::npos ? 0 : start + 3;
    size_t end = uri.find_first_of(L"/?#", start);
    std::wstring host = uri.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
    if (host.rfind(L"www.", 0) == 0) host.erase(0, 4);
    return host.empty() ? L"Local page" : host;
}

void ClearInternalFlags(BrowserTab* tab) {
    if (!tab) return;
    tab->internalPage = false;
    tab->historyPage = false;
    tab->bookmarksPage = false;
    tab->settingsPage = false;
    tab->downloadsPage = false;
}

std::wstring HistoryLabel(const std::wstring& uri) {
    if (uri == L"about:blank") return L"Dashboard";
    if (uri.find(L"www.startpage.com/sp/search?query=") != std::wstring::npos) return L"Startpage Search";
    if (uri.find(L"google.com/search?q=") != std::wstring::npos) return L"Google Search";
    if (uri.find(L"translate.google.") != std::wstring::npos) return L"Google Translate";
    return HistoryHost(uri);
}

void RecordCommittedNavigation(BrowserTab* tab) {
    if (!tab || !tab->webview) return;
    LPWSTR rawUri = nullptr;
    if (FAILED(tab->webview->get_Source(&rawUri)) || !rawUri) return;
    std::wstring uri(rawUri);
    CoTaskMemFree(rawUri);
    if (uri.empty() || uri == L"about:blank") return;
    bool externalNavigation = uri.rfind(L"http://", 0) == 0 || uri.rfind(L"https://", 0) == 0;
    if (tab->internalPage && !externalNavigation) return;
    tab->internalPage = false;
    tab->historyPage = false;
    tab->bookmarksPage = false;
    tab->settingsPage = false;
    tab->downloadsPage = false;
    tab->lastUri = uri;
    if (!tab->privateMode) {
        g_history.push_back({uri, CurrentTimeLabel()});
        constexpr size_t kMaxHistoryEntries = 10000;
        if (g_history.size() > kMaxHistoryEntries) g_history.erase(g_history.begin(), g_history.begin() + static_cast<std::ptrdiff_t>(g_history.size() - kMaxHistoryEntries));
        SavePersistentData();
    }
    if (tab == ActiveTab()) {
        SetWindowTextW(g_address, uri.c_str());
        SetTabLabel(tab);
    }
}

std::wstring BookmarksHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    const wchar_t* muted = g_darkMode ? L"#bdc1c6" : L"#5f6368";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:1000px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px;box-shadow:0 4px 24px rgba(0,0,0,.15)}h1{margin:0 0 20px;font-size:34px}button{border:0;border-radius:6px;padding:9px 13px;margin:4px;cursor:pointer;background:" + std::wstring(g_darkMode ? L"#3c4043" : L"#f1f3f4") + L";color:" + text + L"}a{color:" + std::wstring(g_darkMode ? L"#8ab4f8" : L"#1967d2") + L";font-size:17px}.item{padding:14px 0;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}.uri{color:" + muted + L";font-size:14px;margin-top:4px}</style></head><body><main class='wrap'><section class='card'><h1>Bookmarks</h1><button onclick=\"addCurrent()\">Add current page</button><div id='items'>";
    if (g_bookmarks.empty()) html += L"<p style='color:" + std::wstring(muted) + L"'>No bookmarks yet.</p>";
    for (int i = static_cast<int>(g_bookmarks.size()) - 1; i >= 0; --i) {
        const auto& b = g_bookmarks[static_cast<size_t>(i)];
        html += L"<div class='item'><a href='#' onclick=\"openBookmark(" + std::to_wstring(i) + L");return false;\">" + HtmlEscape(b.title) + L"</a><div class='uri'>" + HtmlEscape(b.uri) + L"</div><button onclick=\"deleteBookmark(" + std::to_wstring(i) + L")\">Delete</button></div>";
    }
    html += LR"HTML(</div><button onclick="send('clear-data')">Clear browsing data</button><button onclick="send('bookmarks-export')">Export bookmarks</button><button onclick="send('bookmarks-import')">Import bookmarks</button><button onclick="send('check-updates')">Check for updates</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m);function addCurrent(){send('bookmark-add')}function openBookmark(i){send('bookmark-open:'+i)}function deleteBookmark(i){send('bookmark-delete:'+i)}</script></body></html>)HTML";
    return html;
}

std::wstring DownloadsHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:1050px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px;box-shadow:0 4px 24px rgba(0,0,0,.15)}h1{margin:0 0 20px;font-size:34px}.item{padding:14px 0;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}.muted{opacity:.7;font-size:14px}button{border:0;border-radius:6px;padding:8px 12px;margin:3px;cursor:pointer;background:" + std::wstring(g_darkMode ? L"#3c4043" : L"#f1f3f4") + L";color:" + text + L"}</style></head><body><main class='wrap'><section class='card'><h1>Downloads</h1><button onclick=\"send('downloads-clear')\">Clear download list</button>";
    if (g_downloads.empty()) html += L"<p class='muted'>No downloads in this session.</p>";
    for (size_t i = 0; i < g_downloads.size(); ++i) {
        const auto& d = g_downloads[i];
        html += L"<div class='item'><strong>" + HtmlEscape(d.record.fileName) + L"</strong><div class='muted'>" + HtmlEscape(d.record.state) + L" — " + HtmlEscape(d.record.path) + L"</div><button onclick=\"send('download-open:" + std::to_wstring(i) + L"')\">Open file</button><button onclick=\"send('download-folder:" + std::to_wstring(i) + L"')\">Open folder</button></div>";
    }
    html += L"</section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
    return html;
}

std::wstring SettingsHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:900px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px;box-shadow:0 4px 24px rgba(0,0,0,.15)}h1{font-size:34px;margin:0 0 24px}.setting{display:flex;justify-content:space-between;align-items:center;padding:17px 0;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}select,input{font:inherit;padding:8px;border-radius:6px}button{font:inherit;padding:9px 13px;margin:10px 6px 0 0;border:0;border-radius:6px;cursor:pointer}.shortcuts{width:100%;border-collapse:collapse;margin-top:12px}.shortcuts th,.shortcuts td{text-align:left;padding:7px;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}</style></head><body><main class='wrap'><section class='card'><h1>Settings</h1>";
    html += L"<div class='setting'><span>Search engine</span><select onchange=\"send('settings-search-engine:'+this.value)\">" + SearchEngineOptionsHtml() + L"</select></div><div class='setting'><span>Custom search URL</span><input style='width:52%' value='" + HtmlEscape(g_settings.customSearchUrl) + L"' placeholder='https://example.com/search?q={query}' onchange=\"send('settings-custom-search:'+this.value)\"></div>";
    html += std::wstring(L"<div class='setting'><span>Theme</span><select onchange=\"send('settings-theme:'+this.value)\"><option value='system'") + (g_settings.theme == L"system" ? L" selected" : L"") + L">System</option><option value='light'" + (g_settings.theme == L"light" ? L" selected" : L"") + L">Light</option><option value='dark'" + (g_settings.theme == L"dark" ? L" selected" : L"") + L">Dark</option></select></div>";
    html += std::wstring(L"<div class='setting'><span>Enable tabs</span><input type='checkbox' ") + (g_settings.tabsEnabled ? L"checked" : L"") + L" onchange=\"send('settings-tabs:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Persistent history</span><input type='checkbox' ") + (g_settings.persistHistory ? L"checked" : L"") + L" onchange=\"send('settings-history:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Restore previous session</span><input type='checkbox' ") + (g_settings.restoreSession ? L"checked" : L"") + L" onchange=\"send('settings-restore:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Check for updates on startup</span><input type='checkbox' ") + (g_settings.checkUpdates ? L"checked" : L"") + L" onchange=\"send('settings-updates:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Custom title bar</span><input type='checkbox' ") + (g_settings.customTitleBar ? L"checked" : L"") + L" onchange=\"send('settings-titlebar:'+(this.checked?'1':'0'))\"></div>";
    html += L"<h2>Privacy and site data</h2><p>Normal cookies and site data are kept locally so trusted websites can remember your sign-in. Private tabs use an isolated profile, are excluded from History and session restore, and their download records are not persisted. Pane Browser does not upload its own History to a Pane Browser server.</p>";
    html += LR"HTML(<h2>Keyboard shortcuts</h2><table class='shortcuts'><tr><th>Shortcut</th><th>Action</th></tr><tr><td>Ctrl+L</td><td>Focus address bar</td></tr><tr><td>Ctrl+T</td><td>Open a new tab</td></tr><tr><td>Ctrl+W</td><td>Close the active tab</td></tr><tr><td>Ctrl+H</td><td>Open History</td></tr><tr><td>Ctrl+Shift+T</td><td>Restore the most recently closed normal tab</td></tr><tr><td>Ctrl+Tab</td><td>Next tab</td></tr><tr><td>Ctrl+Shift+Tab</td><td>Previous tab</td></tr><tr><td>Ctrl+R / F5</td><td>Reload page</td></tr><tr><td>Ctrl+F</td><td>Find in page</td></tr><tr><td>Ctrl+D</td><td>Add bookmark</td></tr><tr><td>Ctrl+J</td><td>Downloads</td></tr><tr><td>Ctrl+Shift+P</td><td>Open private tab</td></tr><tr><td>Ctrl++ / Ctrl+-</td><td>Zoom in / out</td></tr><tr><td>Ctrl+0</td><td>Reset zoom</td></tr></table>)HTML";
    html += L"<button onclick=\"send('choose-download-folder')\">Choose download folder</button><button onclick=\"send('clear-data')\">Clear browsing data</button><button onclick=\"send('bookmarks-export')\">Export bookmarks</button><button onclick=\"send('bookmarks-import')\">Import bookmarks</button><button onclick=\"send('check-updates')\">Check for updates</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
    return html;
}
std::wstring HistoryHtml() {
    const wchar_t* css = g_darkMode
        ? LR"CSS(html,body{min-height:100%;}body{margin:0;font-family:system-ui,Segoe UI,sans-serif;background:#202124;color:#e8eaed}.container{max-width:1200px;margin:auto;padding:36px 28px}.card{background:#292a2d;border-radius:16px;padding:28px 26px 36px;box-shadow:0 0 36px rgba(0,0,0,.35)}h1{margin:0 0 22px;font-family:Segoe UI,system-ui,sans-serif;font-size:34px;font-weight:600;letter-spacing:.1px;color:#f1f3f4}.search{width:100%;box-sizing:border-box;background:#303134;color:#e8eaed;border:1px solid #5f6368;border-radius:26px;padding:13px 20px;font-size:17px;outline:none}.selection{display:flex;align-items:center;gap:12px;padding:16px 2px 8px;color:#e8eaed}.selection.show{display:flex}.selection .when-selected{display:none}.selection.has-selection .when-selected{display:inline-block}.selection button,.menu button{background:transparent;border:0;color:#8ab4f8;padding:8px 10px;border-radius:6px;cursor:pointer;font-size:14px}.selection button:hover,.menu button:hover{background:#3c4043}.row{display:grid;grid-template-columns:28px 70px minmax(0,1fr) 42px;align-items:center;gap:10px;min-height:66px;border-bottom:1px solid #3c4043}.pick{width:18px;height:18px;accent-color:#8ab4f8}.time{color:#bdc1c6;font-size:15px}.site{display:flex;align-items:center;gap:13px;min-width:0}.siteicon{width:24px;height:24px;border-radius:50%;display:inline-flex;align-items:center;justify-content:center;background:#5f6368;color:#fff;font-family:Segoe UI,system-ui,sans-serif;font-size:12px;font-weight:700;line-height:1;text-align:center;text-transform:uppercase;flex:none}.title{display:block;border:0;background:transparent;color:#e8eaed;font-size:17px;font-weight:650;padding:0;cursor:pointer;text-align:left;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;max-width:100%}.host{color:#bdc1c6;font-size:15px;margin-top:5px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.morewrap{position:relative;text-align:center}.more{border:0;background:transparent;color:#bdc1c6;font-size:24px;cursor:pointer}.menu{display:none;position:absolute;right:0;top:30px;z-index:5;min-width:190px;background:#303134;border:1px solid #5f6368;box-shadow:0 5px 18px rgba(0,0,0,.4);padding:6px;border-radius:6px;text-align:left}.menu.open{display:block}.empty{color:#9aa0a6;font-size:17px})CSS"
        : LR"CSS(html,body{min-height:100%;}body{margin:0;font-family:system-ui,Segoe UI,sans-serif;background:#f8f9fa;color:#202124}.container{max-width:1200px;margin:auto;padding:36px 28px}.card{background:#ffffff;border:1px solid #dadce0;border-radius:16px;padding:28px 26px 36px;box-shadow:0 0 36px rgba(60,64,67,.16)}h1{margin:0 0 22px;font-family:Segoe UI,system-ui,sans-serif;font-size:34px;font-weight:600;letter-spacing:.1px;color:#202124}.search{width:100%;box-sizing:border-box;background:#ffffff;color:#202124;border:1px solid #dadce0;border-radius:26px;padding:13px 20px;font-size:17px;outline:none}.selection{display:flex;align-items:center;gap:12px;padding:16px 2px 8px;color:#202124}.selection.show{display:flex}.selection .when-selected{display:none}.selection.has-selection .when-selected{display:inline-block}.selection button,.menu button{background:transparent;border:0;color:#1967d2;padding:8px 10px;border-radius:6px;cursor:pointer;font-size:14px}.selection button:hover,.menu button:hover{background:#f1f3f4}.row{display:grid;grid-template-columns:28px 70px minmax(0,1fr) 42px;align-items:center;gap:10px;min-height:66px;border-bottom:1px solid #e8eaed}.pick{width:18px;height:18px;accent-color:#1a73e8}.time{color:#5f6368;font-size:15px}.site{display:flex;align-items:center;gap:13px;min-width:0}.siteicon{width:24px;height:24px;border-radius:50%;display:inline-flex;align-items:center;justify-content:center;background:#5f6368;color:#fff;font-family:Segoe UI,system-ui,sans-serif;font-size:12px;font-weight:700;line-height:1;text-align:center;text-transform:uppercase;flex:none}.title{display:block;border:0;background:transparent;color:#202124;font-size:17px;font-weight:650;padding:0;cursor:pointer;text-align:left;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;max-width:100%}.host{color:#5f6368;font-size:15px;margin-top:5px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.morewrap{position:relative;text-align:center}.more{border:0;background:transparent;color:#5f6368;font-size:24px;cursor:pointer}.menu{display:none;position:absolute;right:0;top:30px;z-index:5;min-width:190px;background:#ffffff;border:1px solid #dadce0;box-shadow:0 5px 18px rgba(60,64,67,.25);padding:6px;border-radius:6px;text-align:left}.menu.open{display:block}.empty{color:#5f6368;font-size:17px})CSS";
    std::wstring html = L"<!DOCTYPE html><html><head><meta charset=\"UTF-8\"><title>Browsing History</title><style>";
    html += css;
    html += L"</style></head><body><div class=\"container\"><div class=\"card\"><h1>Browsing History</h1><input id=\"search\" class=\"search\" placeholder=\"Search history\" oninput=\"filterRows()\"><div id=\"selection\" class=\"selection\"><span id=\"count\">0 selected</span><button onclick=\"selectAll()\">Select all</button><button class=\"when-selected\" onclick=\"deselectAll()\">Deselect all</button><button class=\"when-selected\" onclick=\"deleteSelected()\">Delete</button></div><div id=\"rows\">";
    if (g_history.empty()) {
        html += L"<p class=\"empty\">No history.</p>";
    } else {
        for (int n = static_cast<int>(g_history.size()) - 1; n >= 0; --n) {
            const HistoryEntry& entry = g_history[static_cast<size_t>(n)];
            std::wstring host = HistoryHost(entry.uri);
            std::wstring label = HistoryLabel(entry.uri);
            html += L"<div class=\"row\" data-index=\"" + std::to_wstring(n) + L"\" data-search=\"" + HtmlEscape(label + L" " + host + L" " + entry.uri) + L"\"><input type=\"checkbox\" class=\"pick\" onchange=\"syncSelection()\"><div class=\"time\">" + HtmlEscape(entry.time) + L"</div><div class=\"site\"><span class=\"siteicon\">" + HtmlEscape(host.empty() ? L"?" : host.substr(0, 1)) + L"</span><div><button class=\"title\" onclick=\"openEntry(" + std::to_wstring(n) + L")\">" + HtmlEscape(label) + L"</button><div class=\"host\">" + HtmlEscape(entry.uri) + L"</div></div></div><div class=\"morewrap\"><button class=\"more\" onclick=\"toggleMenu(this)\">⋮</button><div class=\"menu\"><button onclick=\"openEntry(" + std::to_wstring(n) + L")\">Open</button><button onclick=\"deleteEntry(" + std::to_wstring(n) + L")\">Delete from history</button></div></div></div>";
        }
    }
    html += LR"HTML(</div></div></div><script>
const send = m => { if (window.chrome && chrome.webview) chrome.webview.postMessage(m); };
function rows(){return [...document.querySelectorAll('.row')];}
function syncSelection(){const picks=document.querySelectorAll('.pick:checked');const selection=document.getElementById('selection');document.getElementById('count').textContent=picks.length+' selected';selection.classList.toggle('show',picks.length>0);selection.classList.toggle('has-selection',picks.length>0);}
function selectAll(){document.querySelectorAll('.pick').forEach(p=>p.checked=true);syncSelection();}
function deselectAll(){document.querySelectorAll('.pick').forEach(p=>p.checked=false);syncSelection();}
function filterRows(){const q=document.getElementById('search').value.toLowerCase();rows().forEach(r=>r.style.display=r.dataset.search.toLowerCase().includes(q)?'grid':'none');}
function indexes(){return rows().filter(r=>r.querySelector('.pick').checked).map(r=>r.dataset.index);}
function openEntry(i){send('open:'+i);}
function deleteEntry(i){send('delete:'+i);}
function deleteSelected(){send('delete-selected:'+indexes().join(','));}
function toggleMenu(btn){document.querySelectorAll('.menu.open').forEach(m=>m.classList.remove('open'));btn.nextElementSibling.classList.toggle('open');}
document.addEventListener('click',e=>{if(!e.target.closest('.morewrap'))document.querySelectorAll('.menu.open').forEach(m=>m.classList.remove('open'));});
syncSelection();
</script></body></html>)HTML";
    html += L"";
    return html;
}

bool ParseHistoryIndex(const std::wstring& value, int& index) {
    try {
        size_t consumed = 0;
        index = std::stoi(value, &consumed);
        return consumed == value.size() && index >= 0 && index < static_cast<int>(g_history.size());
    } catch (...) {
        return false;
    }
}

std::vector<int> ParseHistoryIndices(const std::wstring& value) {
    std::vector<int> indices;
    size_t start = 0;
    while (start < value.size()) {
        size_t end = value.find(L',', start);
        std::wstring part = value.substr(start, end == std::wstring::npos ? std::wstring::npos : end - start);
        int index = -1;
        if (ParseHistoryIndex(part, index)) indices.push_back(index);
        if (end == std::wstring::npos) break;
        start = end + 1;
    }
    std::sort(indices.begin(), indices.end(), std::greater<int>());
    indices.erase(std::unique(indices.begin(), indices.end()), indices.end());
    return indices;
}

bool ParseBookmarkIndex(const std::wstring& value, int& index) {
    try { size_t consumed = 0; index = std::stoi(value, &consumed); return consumed == value.size() && index >= 0 && index < static_cast<int>(g_bookmarks.size()); }
    catch (...) { return false; }
}

void RefreshBookmarkPage() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->historyPage = false; tab->bookmarksPage = true; tab->settingsPage = false; tab->webview->NavigateToString(BookmarksHtml().c_str()); }
void RefreshSettingsPage() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->historyPage = false; tab->bookmarksPage = false; tab->settingsPage = true; tab->webview->NavigateToString(SettingsHtml().c_str()); }
void ApplyCustomTitleBar();

void RefreshHistoryPage() {
    BrowserTab* tab = ActiveTab();
    ICoreWebView2* webview = ActiveWebView();
    if (!tab || !webview) return;
    tab->internalPage = true;
    tab->historyPage = true;
    tab->bookmarksPage = false;
    tab->settingsPage = false;
    std::wstring html = HistoryHtml();
    webview->NavigateToString(html.c_str());
}

void RefreshRuntimeTheme();
void ApplyCustomTitleBar();

bool IsTrustedInternalMessage(BrowserTab* tab, ICoreWebView2WebMessageReceivedEventArgs* args) {
    if (!tab || !args || !(tab->internalPage || tab->historyPage || tab->bookmarksPage || tab->settingsPage || tab->downloadsPage)) return false;
    LPWSTR source = nullptr;
    if (FAILED(args->get_Source(&source)) || !source) return false;
    std::wstring document(source);
    CoTaskMemFree(source);
    return document == L"about:blank";
}

void HandleHistoryMessage(BrowserTab* tab, ICoreWebView2WebMessageReceivedEventArgs* args) {
    if (!tab || !args || !IsTrustedInternalMessage(tab, args)) return;
    LPWSTR raw = nullptr;
    if (FAILED(args->TryGetWebMessageAsString(&raw)) || !raw) return;
    std::wstring message(raw);
        CoTaskMemFree(raw);
    if (message.rfind(L"search:", 0) == 0) {
        std::wstring input = message.substr(7);
        std::wstring uri = NormalizeInput(input);
        if (!uri.empty()) {
            tab->internalPage = false;
            tab->historyPage = false;
            tab->webview->Navigate(uri.c_str());
        }
        return;
    }
    if (message.rfind(L"open:", 0) == 0) {
        int index = -1;
        if (ParseHistoryIndex(message.substr(5), index)) {
            const HistoryEntry entry = g_history[static_cast<size_t>(index)];
            tab->lastUri = entry.uri;
            tab->internalPage = false;
            tab->historyPage = false;
            tab->webview->Navigate(entry.uri.c_str());
            if (tab == ActiveTab()) {
                SetWindowTextW(g_address, entry.uri.c_str());
                SetTabLabel(tab);
            }
        }
        return;
    }


    if (message == L"downloads-clear") { g_downloads.clear(); SavePersistentData(); ShowDownloads(); return; }
    if (message.rfind(L"download-open:",0)==0) { int i=-1; try{i=std::stoi(message.substr(14));}catch(...){ } if(i>=0&&i<(int)g_downloads.size())ShellExecuteW(g_main,L"open",g_downloads[i].record.path.c_str(),nullptr,nullptr,SW_SHOWNORMAL); return; }
    if (message.rfind(L"download-folder:",0)==0) { int i=-1; try{i=std::stoi(message.substr(16));}catch(...){ } if(i>=0&&i<(int)g_downloads.size())ShellExecuteW(g_main,L"open",g_downloads[i].record.path.substr(0,g_downloads[i].record.path.find_last_of(L"\\/")).c_str(),nullptr,nullptr,SW_SHOWNORMAL); return; }
    if (message == L"choose-download-folder") { BROWSEINFOW bi{}; bi.hwndOwner=g_main; bi.lpszTitle=L"Choose the default download folder"; bi.ulFlags=BIF_RETURNONLYFSDIRS|BIF_NEWDIALOGSTYLE; PIDLIST_ABSOLUTE pidl=SHBrowseForFolderW(&bi); if(pidl){wchar_t selected[MAX_PATH]={}; if(SHGetPathFromIDListW(pidl,selected)){g_settings.downloadDirectory=selected; SavePersistentData();} CoTaskMemFree(pidl);} RefreshSettingsPage(); return; }
    if (message == L"clear-data") { ClearBrowsingData(); RefreshHistoryPage(); return; }
    if (message == L"bookmarks-export") { ExportBookmarks(); return; }
    if (message == L"bookmarks-import") { ImportBookmarks(); RefreshBookmarkPage(); return; }
    if (message == L"check-updates") { CheckForUpdates(); return; }
    if (message == L"bookmark-add") {
        if (tab && !tab->privateMode && tab->lastUri.rfind(L"http", 0) == 0) {
            g_bookmarks.push_back({HistoryLabel(tab->lastUri), tab->lastUri});
            SavePersistentData();
        }
        RefreshBookmarkPage();
        return;
    }
    if (message.rfind(L"bookmark-open:", 0) == 0) {
        int index = -1;
        if (ParseBookmarkIndex(message.substr(14), index)) {
            ClearInternalFlags(tab); tab->webview->Navigate(g_bookmarks[static_cast<size_t>(index)].uri.c_str());
        }
        return;
    }
    if (message.rfind(L"bookmark-delete:", 0) == 0) {
        int index = -1;
        if (ParseBookmarkIndex(message.substr(16), index)) g_bookmarks.erase(g_bookmarks.begin() + index);
        SavePersistentData(); RefreshBookmarkPage(); return;
    }
    if (message.rfind(L"settings-search-engine:", 0) == 0) { std::wstring id=message.substr(23); if(FindSearchEngine(id)) { g_settings.searchEngine=id; SavePersistentData(); } RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-custom-search:", 0) == 0) { g_settings.customSearchUrl=message.substr(23); if(!g_settings.customSearchUrl.empty())g_settings.searchEngine=L"custom"; SavePersistentData(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-theme:", 0) == 0) {
        g_settings.theme = message.substr(15); SavePersistentData(); RefreshRuntimeTheme(); RefreshSettingsPage(); return;
    }
    if (message.rfind(L"settings-tabs:", 0) == 0) {
        g_settings.tabsEnabled = message.substr(14) != L"0"; SavePersistentData(); ResizeChildren(); InvalidateTabs(); RefreshSettingsPage(); return;
    }
    if (message.rfind(L"settings-history:", 0) == 0) {
        g_settings.persistHistory = message.substr(17) != L"0"; if (!g_settings.persistHistory) v3data::ClearHistory(g_dataDirectory); else SavePersistentData(); RefreshSettingsPage(); return;
    }
    if (message.rfind(L"settings-restore:",0)==0) { g_settings.restoreSession=message.substr(18)!=L"0"; if (!g_settings.restoreSession) v3data::ClearSession(g_dataDirectory); else SavePersistentData(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-updates:",0)==0) { g_settings.checkUpdates=message.substr(17)!=L"0"; SavePersistentData(); return; }
    if (message.rfind(L"settings-titlebar:", 0) == 0) {
        g_settings.customTitleBar = message.substr(18) != L"0"; SavePersistentData(); ApplyCustomTitleBar(); RefreshSettingsPage(); return;
    }

    if (message.rfind(L"delete-selected:", 0) == 0) {
        std::vector<int> indices = ParseHistoryIndices(message.substr(16));
        for (int index : indices) g_history.erase(g_history.begin() + index);
        SavePersistentData();
        RefreshHistoryPage();
        return;
    }

    if (message.rfind(L"delete:", 0) == 0) {
        int index = -1;
        if (ParseHistoryIndex(message.substr(7), index)) g_history.erase(g_history.begin() + index);
        SavePersistentData();
        RefreshHistoryPage();
    }
}

void NavigateFromAddressBar() {
    BrowserTab* tab = ActiveTab();
    if (!tab || !tab->webview || !g_address) return;
    wchar_t input[0x800] = {};
    GetWindowTextW(g_address, input, static_cast<int>(sizeof(input) / sizeof(input[0])));
    std::wstring uri = NormalizeInput(input);
    if (!uri.empty()) {
        tab->internalPage = false;
        tab->webview->Navigate(uri.c_str());
    }
}

void ShowBookmarks() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; SetWindowTextW(g_address, L""); RefreshBookmarkPage(); }
void ShowSettings() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; SetWindowTextW(g_address, L""); RefreshSettingsPage(); }
void ShowPrivate() { if (g_settings.tabsEnabled) CreateNewTab(L"dashboard", true); else { if (!g_tabs.empty()) { CreateNewTab(L"dashboard", true); if (g_tabs.size() > 1) CloseTab(0); } } }

void ShowDownloads();
void FindInPage();
void UpdateDownloadStates();
void ClearBrowsingData();
void ReopenClosedTab();
void ShowHistory() {
    BrowserTab* tab = ActiveTab();
    ICoreWebView2* webview = ActiveWebView();
    if (!tab || !webview) return;
    SetWindowTextW(g_address, L"");
    tab->internalPage = true;
    tab->historyPage = true;
    tab->bookmarksPage = false;
    tab->settingsPage = false;
    std::wstring html = HistoryHtml();
    webview->NavigateToString(html.c_str());
}

void ShowHome() {
    BrowserTab* tab = ActiveTab();
    if (!tab || !tab->webview) return;
    tab->lastUri = L"about:blank";
    tab->internalPage = true;
    tab->historyPage = false;
    tab->bookmarksPage = false;
    tab->settingsPage = false;
    SetTabLabel(tab);
    SetWindowTextW(g_address, L"");
    SavePersistentData();
    tab->webview->NavigateToString(DashboardHtml());
}

void ResizeChildren() {
    if (!g_main) return;
    RECT rc{}; GetClientRect(g_main, &rc); int width = rc.right - rc.left;
    const int titleHeight = g_settings.customTitleBar ? 32 : 0;
    const int tabHeight = g_settings.tabsEnabled ? 32 : 0;
    ShowWindow(g_tabStrip, g_settings.tabsEnabled ? SW_SHOW : SW_HIDE);
    ShowWindow(g_historyButton, SW_HIDE); ShowWindow(g_bookmarksButton, SW_HIDE); ShowWindow(g_settingsButton, SW_HIDE); ShowWindow(g_privateButton, SW_HIDE); ShowWindow(g_downloadsButton, SW_HIDE); ShowWindow(g_findButton, SW_HIDE);
    const int toolbarY = titleHeight + tabHeight + 4; const int webTop = toolbarY + 34;
    MoveWindow(g_tabStrip, 0, titleHeight, std::max(200, width), 32, TRUE);
    MoveWindow(g_back, 5, toolbarY, 24, 30, TRUE); MoveWindow(g_forward, 33, toolbarY, 24, 30, TRUE); MoveWindow(g_reload, 61, toolbarY, 58, 30, TRUE); MoveWindow(g_home, 124, toolbarY, 48, 30, TRUE);
    const int addressLeft = 177, menuWidth = 36, rightMargin = 5; int menuLeft = std::max(addressLeft + 126, width - menuWidth - rightMargin);
    MoveWindow(g_address, addressLeft, toolbarY, std::max(80, menuLeft - addressLeft - 6), 30, TRUE);
    MoveWindow(g_overflowButton, menuLeft, toolbarY, menuWidth, 30, TRUE);
    for (size_t i = 0; i < g_tabs.size(); ++i) if (g_tabs[i]->controller) { RECT webBounds = rc; webBounds.top = webTop; g_tabs[i]->controller->put_Bounds(webBounds); g_tabs[i]->controller->put_IsVisible(static_cast<int>(i) == g_activeTab ? TRUE : FALSE); }
}

LRESULT CALLBACK AddressProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_KEYDOWN && (GetKeyState(VK_CONTROL)&0x8000)) { bool shift=(GetKeyState(VK_SHIFT)&0x8000)!=0; if(HandleShortcut(static_cast<UINT>(wp), true, shift, false)) return 0; }
    if (msg == WM_KEYDOWN && wp == VK_RETURN) {
        NavigateFromAddressBar();
        return 0;
    }
    return g_oldEditProc ? CallWindowProcW(g_oldEditProc, hwnd, msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
}


void ShowDownloads() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->historyPage = tab->bookmarksPage = tab->settingsPage = false; tab->downloadsPage = true; SetWindowTextW(g_address, L""); tab->webview->NavigateToString(DownloadsHtml().c_str()); }

void FindInPage() { if (!ActiveWebView()) return; const wchar_t* script = LR"JS((function(){var q=window.prompt('Find in page',''); if(q===null||q==='') return 'cancel'; return window.find(q)?'found':'not-found';})())JS"; auto cb = Callback<ICoreWebView2ExecuteScriptCompletedHandler>([](HRESULT, LPCWSTR){ return S_OK; }); ActiveWebView()->ExecuteScript(script, cb.Get()); cb.Get()->Release(); }

void AdjustZoom(double delta) { BrowserTab* tab = ActiveTab(); if (!tab || !tab->controller) return; double z = 1.0; tab->controller->get_ZoomFactor(&z); z = std::max(0.25, std::min(5.0, z + delta)); tab->controller->put_ZoomFactor(z); }
void ResetZoom() { BrowserTab* tab = ActiveTab(); if (tab && tab->controller) tab->controller->put_ZoomFactor(1.0); }

void ReopenClosedTab() { if (!g_settings.tabsEnabled || g_closedTabs.empty()) return; std::wstring uri = g_closedTabs.back(); g_closedTabs.pop_back(); CreateNewTab(uri, false); }

void AddCurrentBookmark() { BrowserTab* tab = ActiveTab(); if (!tab || tab->privateMode || tab->lastUri.rfind(L"http", 0) != 0) return; for (const auto& b : g_bookmarks) if (b.uri == tab->lastUri) return; g_bookmarks.push_back({HistoryLabel(tab->lastUri), tab->lastUri}); SavePersistentData(); }

void ExportBookmarks() {
    OPENFILENAMEW ofn{}; wchar_t path[MAX_PATH] = L"PaneBrowser-bookmarks.html"; ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=g_main; ofn.lpstrFilter=L"HTML bookmarks (*.html)\\0*.html\\0All files (*.*)\\0*.*\\0"; ofn.lpstrFile=path; ofn.nMaxFile=MAX_PATH; ofn.Flags=OFN_OVERWRITEPROMPT|OFN_PATHMUSTEXIST; if (!GetSaveFileNameW(&ofn)) return;
    std::ofstream out(path, std::ios::binary|std::ios::trunc); if (!out) { MessageBoxW(g_main,L"Could not export bookmarks.",L"Pane Browser",MB_OK|MB_ICONERROR); return; }
    out << "<!DOCTYPE NETSCAPE-Bookmark-file-1>\\n<TITLE>Pane Browser Bookmarks</TITLE>\\n<H1>Pane Browser Bookmarks</H1>\\n<DL><p>\\n";
    for (const auto& b : g_bookmarks) out << "<DT><A HREF=\"" << v3data::ToUtf8(b.uri) << "\">" << v3data::ToUtf8(b.title) << "</A>\\n";
    out << "</DL><p>\\n"; out.close();
}

void ImportBookmarks() {
    OPENFILENAMEW ofn{}; wchar_t path[MAX_PATH] = {}; ofn.lStructSize=sizeof(ofn); ofn.hwndOwner=g_main; ofn.lpstrFilter=L"HTML bookmarks (*.html;*.htm)\\0*.html;*.htm\\0All files (*.*)\\0*.*\\0"; ofn.lpstrFile=path; ofn.nMaxFile=MAX_PATH; ofn.Flags=OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST; if (!GetOpenFileNameW(&ofn)) return;
    std::ifstream in(path, std::ios::binary); std::string text((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>()); size_t pos=0, added=0;
    while ((pos=text.find("HREF=\"",pos)) != std::string::npos) { pos += 6; size_t end=text.find('"',pos); if(end==std::string::npos)break; std::wstring uri=v3data::FromUtf8(text.substr(pos,end-pos)); size_t gt=text.find('>',end); size_t lt=gt==std::string::npos?std::string::npos:text.find("</A>",gt); if(gt!=std::string::npos&&lt!=std::string::npos){ std::wstring title=v3data::FromUtf8(text.substr(gt+1,lt-gt-1)); bool exists=false; for(const auto& b:g_bookmarks)if(b.uri==uri)exists=true; if(!exists&&(uri.rfind(L"http://",0)==0||uri.rfind(L"https://",0)==0)){g_bookmarks.push_back({title.empty()?HistoryHost(uri):title,uri});++added;} } pos=end+1; }
    SavePersistentData(); if(added==0)MessageBoxW(g_main,L"No new HTTP(S) bookmarks were found.",L"Pane Browser",MB_OK|MB_ICONINFORMATION); 
}

std::wstring SafeDownloadName(const std::wstring& value) {
    std::wstring name=value; size_t eq=name.find(L"filename="); if(eq!=std::wstring::npos)name=name.substr(eq+9);
    while(!name.empty()&&(name.front()==L' '||name.front()==L'"'||name.front()==L'\''))name.erase(name.begin());
    while(!name.empty()&&(name.back()==L' '||name.back()==L'"'||name.back()==L'\''))name.pop_back();
    size_t slash=name.find_last_of(L"\\/"); if(slash!=std::wstring::npos)name=name.substr(slash+1);
    for(wchar_t& c:name)if(c==L'<'||c==L'>'||c==L':'||c==L'"'||c==L'/'||c==L'\\'||c==L'|'||c==L'?'||c==L'*')c=L'_';
    if (name.empty() || name == L"." || name == L"..") name = L"download.bin";
    return name;
}
std::wstring DownloadNameFromUri(const std::wstring& uri) { size_t end=uri.find_first_of(L"?#"); std::wstring clean=uri.substr(0,end==std::wstring::npos?std::wstring::npos:end); size_t slash=clean.find_last_of(L"/"); return SafeDownloadName(slash==std::wstring::npos?L"download.bin":clean.substr(slash+1)); }
std::wstring UniqueDownloadPath(const std::wstring& folder,const std::wstring& name) { std::wstring path=v3data::Join(folder,name); if(GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES)return path; size_t dot=name.find_last_of(L'.'); std::wstring stem=dot==std::wstring::npos?name:name.substr(0,dot), ext=dot==std::wstring::npos?L"":name.substr(dot); for(int n=1;n<10000;++n){path=v3data::Join(folder,stem+L" ("+std::to_wstring(n)+L")"+ext);if(GetFileAttributesW(path.c_str())==INVALID_FILE_ATTRIBUTES)return path;} return v3data::Join(folder,stem+L" (new)"+ext); }

void UpdateDownloadStates() { bool changed=false; for(auto& d:g_downloads){ if(!d.operation)continue; COREWEBVIEW2_DOWNLOAD_STATE state{}; if(SUCCEEDED(d.operation->get_State(&state))){std::wstring value=state==COREWEBVIEW2_DOWNLOAD_STATE_IN_PROGRESS?L"In progress":state==COREWEBVIEW2_DOWNLOAD_STATE_COMPLETED?L"Completed":state==COREWEBVIEW2_DOWNLOAD_STATE_INTERRUPTED?L"Interrupted":L"Starting"; if(d.record.state!=value){d.record.state=value;changed=true;}} LPWSTR path=nullptr; if(SUCCEEDED(d.operation->get_ResultFilePath(&path))&&path){d.record.path=path;CoTaskMemFree(path);} } if(changed) { SavePersistentData(); if(ActiveTab()&&ActiveTab()->downloadsPage)ShowDownloads(); } }

void ClearBrowsingData() {
    if (MessageBoxW(g_main, L"Clear local History, download records, cookies, cache, and site data? This may sign you out of websites.", L"Pane Browser", MB_YESNO | MB_ICONWARNING) != IDYES) return;
    g_history.clear(); g_downloads.clear();
    v3data::ClearHistory(g_dataDirectory); v3data::SaveDownloads({}, g_dataDirectory); v3data::ClearSession(g_dataDirectory);
    std::vector<ComPtr<ICoreWebView2Profile2>> profiles;
    for (auto& tab : g_tabs) if (tab->webview) {
        ComPtr<ICoreWebView2_13> w13;
        if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_13, reinterpret_cast<void**>(w13.GetAddressOf()))) && w13) {
            ComPtr<ICoreWebView2Profile> profile;
            if (SUCCEEDED(w13->get_Profile(&profile)) && profile) {
                ComPtr<ICoreWebView2Profile2> p2;
                if (SUCCEEDED(profile->QueryInterface(IID_ICoreWebView2Profile2, reinterpret_cast<void**>(p2.GetAddressOf()))) && p2) {
                    bool duplicate = false; for (const auto& existing : profiles) if (existing.Get() == p2.Get()) duplicate = true;
                    if (!duplicate) profiles.push_back(p2);
                }
            }
        }
    }
    if (profiles.empty()) { MessageBoxW(g_main, L"Local records were cleared, but this WebView2 Runtime does not expose profile data clearing.", L"Pane Browser", MB_OK | MB_ICONWARNING); RefreshHistoryPage(); return; }
    auto pending = std::make_shared<int>(static_cast<int>(profiles.size()));
    auto failed = std::make_shared<bool>(false);
    for (const auto& profile : profiles) {
        auto callback = Callback<ICoreWebView2ClearBrowsingDataCompletedHandler>([pending, failed](HRESULT hr) -> HRESULT {
            if (FAILED(hr)) *failed = true;
            --*pending;
            if (*pending == 0) {
                MessageBoxW(g_main, *failed ? L"Some WebView2 browsing data could not be cleared." : L"Browsing history, download history, cookies, cache, and site data were cleared.", L"Pane Browser", MB_OK | (*failed ? MB_ICONWARNING : MB_ICONINFORMATION));
                RefreshHistoryPage();
            }
            return S_OK;
        });
        HRESULT hr = profile->ClearBrowsingDataAll(callback.Get());
        callback.Get()->Release();
        if (FAILED(hr)) { *failed = true; --*pending; }
    }
    if (*pending == 0) { MessageBoxW(g_main, L"WebView2 browsing data could not be cleared.", L"Pane Browser", MB_OK | MB_ICONWARNING); RefreshHistoryPage(); }
}

struct UpdateCheckResult { bool requestSucceeded = false; std::wstring tag; };

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

bool HandleShortcut(UINT vk, bool ctrl, bool shift, bool alt) { if(alt)return false; if(!ctrl && vk==VK_F5){if(ActiveWebView())ActiveWebView()->Reload();return true;} if(!ctrl)return false; if(ctrl&&shift&&vk=='T'){ReopenClosedTab();return true;} if(ctrl&&vk=='L'){SetFocus(g_address);SendMessageW(g_address,EM_SETSEL,0,-1);} else if(ctrl&&vk=='T'&&g_settings.tabsEnabled)CreateNewTab(L"dashboard",false); else if(ctrl&&vk=='W')CloseTab(g_activeTab); else if (ctrl && (vk == 'R' || vk == VK_F5)) { if(ActiveWebView()) ActiveWebView()->Reload(); } else if(ctrl&&vk==VK_TAB){int n=static_cast<int>(g_tabs.size());if(n)SetActiveTab((g_activeTab+(shift?-1:1)+n)%n);} else if(ctrl&&vk=='F')FindInPage(); else if(ctrl&&vk=='D')AddCurrentBookmark(); else if(ctrl&&vk=='J')ShowDownloads(); else if(ctrl&&vk=='H')ShowHistory(); else if(ctrl&&shift&&vk=='P')ShowPrivate(); else if(ctrl&&(vk==VK_OEM_PLUS||vk==VK_ADD))AdjustZoom(0.1); else if(ctrl&&(vk==VK_OEM_MINUS||vk==VK_SUBTRACT))AdjustZoom(-0.1); else if(ctrl&&vk=='0')ResetZoom(); else return false; return true; }

bool HandleGlobalShortcutMessage(const MSG& message) {
    if (message.message != WM_KEYDOWN && message.message != WM_SYSKEYDOWN) return false;
    bool ctrl = (GetKeyState(VK_CONTROL) & 0x8000) != 0;
    bool shift = (GetKeyState(VK_SHIFT) & 0x8000) != 0;
    bool alt = (GetKeyState(VK_MENU) & 0x8000) != 0;
    return HandleShortcut(static_cast<UINT>(message.wParam), ctrl, shift, alt);
}

void ConfigureProfilePrivacy(BrowserTab* tab) {
    if (!tab || !tab->webview) return;
    ComPtr<ICoreWebView2_13> web13;
    if (FAILED(tab->webview->QueryInterface(IID_ICoreWebView2_13, reinterpret_cast<void**>(web13.GetAddressOf()))) || !web13) return;
    ComPtr<ICoreWebView2Profile> profile;
    if (FAILED(web13->get_Profile(&profile)) || !profile) return;
    ComPtr<ICoreWebView2Profile3> profile3;
    if (SUCCEEDED(profile->QueryInterface(IID_ICoreWebView2Profile3, reinterpret_cast<void**>(profile3.GetAddressOf()))) && profile3) {
        // Balanced blocks common cross-site tracking while retaining ordinary first-party cookies and sign-ins.
        profile3->put_PreferredTrackingPreventionLevel(COREWEBVIEW2_TRACKING_PREVENTION_LEVEL_BALANCED);
    }
}

void CreateNewTab(const std::wstring& initialUri, bool privateMode) {
    if (!g_environment || !g_tabStrip) return;
    auto tab = std::make_shared<BrowserTab>();
    tab->privateMode = privateMode;
    int index = static_cast<int>(g_tabs.size());
    g_tabs.push_back(tab);
    SetActiveTab(index);

    auto controllerCallback = Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [tab, initialUri](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
            int currentIndex = FindTab(tab.get());
            if (currentIndex < 0 || FAILED(result) || !controller) return FAILED(result) ? result : S_OK;
            tab->controller = controller;
            HRESULT hr = tab->controller->get_CoreWebView2(&tab->webview);
            if (FAILED(hr) || !tab->webview) return hr;
            ComPtr<ICoreWebView2_13> profileWeb; if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_13, reinterpret_cast<void**>(profileWeb.GetAddressOf()))) && profileWeb) { ComPtr<ICoreWebView2Profile> profile; if (SUCCEEDED(profileWeb->get_Profile(&profile)) && profile && !g_settings.downloadDirectory.empty()) profile->put_DefaultDownloadFolderPath(g_settings.downloadDirectory.c_str()); }
            ConfigureProfilePrivacy(tab.get());
            auto acceleratorCallback = Callback<ICoreWebView2AcceleratorKeyPressedEventHandler>([tab](ICoreWebView2Controller*, ICoreWebView2AcceleratorKeyPressedEventArgs* args)->HRESULT { COREWEBVIEW2_KEY_EVENT_KIND kind{}; UINT key=0; if(SUCCEEDED(args->get_KeyEventKind(&kind))&&SUCCEEDED(args->get_VirtualKey(&key))&&kind==COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN){bool ctrl=(GetKeyState(VK_CONTROL)&0x8000)!=0,shift=(GetKeyState(VK_SHIFT)&0x8000)!=0,alt=(GetKeyState(VK_MENU)&0x8000)!=0; if(HandleShortcut(key,ctrl,shift,alt)) args->put_Handled(TRUE);} return S_OK; });
            EventRegistrationToken acceleratorToken{}; tab->controller->add_AcceleratorKeyPressed(acceleratorCallback.Get(), &acceleratorToken); acceleratorCallback.Get()->Release();

            auto messageCallback = Callback<ICoreWebView2WebMessageReceivedEventHandler>(
                [tab](ICoreWebView2* sender, ICoreWebView2WebMessageReceivedEventArgs* args) -> HRESULT {
                    (void)sender;
                    HandleHistoryMessage(tab.get(), args);
                    return S_OK;
                });
            EventRegistrationToken messageToken{};
            tab->webview->add_WebMessageReceived(messageCallback.Get(), &messageToken);
            messageCallback.Get()->Release();
            auto navigationCallback = Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [tab](ICoreWebView2* sender, ICoreWebView2NavigationCompletedEventArgs* args) -> HRESULT {
                    (void)sender;
                    (void)args;
                    RecordCommittedNavigation(tab.get());
                    return S_OK;
                });
            EventRegistrationToken navigationToken{};
            tab->webview->add_NavigationCompleted(navigationCallback.Get(), &navigationToken);
            navigationCallback.Get()->Release();
            auto newWindowCallback = Callback<ICoreWebView2NewWindowRequestedEventHandler>(
                [tab](ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) -> HRESULT {
                    if (!sender || !args) return E_INVALIDARG;
                    LPWSTR requestedUri = nullptr;
                    HRESULT uriHr = args->get_Uri(&requestedUri);
                    if (SUCCEEDED(uriHr) && requestedUri) {
                        std::wstring uri(requestedUri);
                        CoTaskMemFree(requestedUri);
                        if (!uri.empty()) {
                            ClearInternalFlags(tab.get());
                            sender->Navigate(uri.c_str());
                        }
                    }
                    args->put_Handled(TRUE);
                    return S_OK;
                });
            EventRegistrationToken newWindowToken{};
            tab->webview->add_NewWindowRequested(newWindowCallback.Get(), &newWindowToken);
            newWindowCallback.Get()->Release();
            ComPtr<ICoreWebView2_11> web11;
            if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_11, reinterpret_cast<void**>(web11.GetAddressOf()))) && web11) {
                auto contextCallback = Callback<ICoreWebView2ContextMenuRequestedEventHandler>([tab](ICoreWebView2*, ICoreWebView2ContextMenuRequestedEventArgs* args) -> HRESULT {
                    if (!args) return E_INVALIDARG;
                    POINT point{};
                    if (FAILED(args->get_Location(&point))) GetCursorPos(&point);
                    args->put_Handled(TRUE);
                    ShowNativeContextMenu(point);
                    return S_OK;
                });
                EventRegistrationToken contextToken{};
                web11->add_ContextMenuRequested(contextCallback.Get(), &contextToken);
                contextCallback.Get()->Release();
            }
            ComPtr<ICoreWebView2_4> web4;
            if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_4, reinterpret_cast<void**>(web4.GetAddressOf()))) && web4) {
                auto downloadCallback = Callback<ICoreWebView2DownloadStartingEventHandler>([tab](ICoreWebView2*, ICoreWebView2DownloadStartingEventArgs* args) -> HRESULT {
                    if (!args) return E_INVALIDARG;
                    ComPtr<ICoreWebView2DownloadOperation> op;
                    if (FAILED(args->get_DownloadOperation(&op)) || !op) return S_OK;
                    LPWSTR uri = nullptr;
                    op->get_Uri(&uri);
                    std::wstring u = uri ? uri : L"";
                    if (uri) CoTaskMemFree(uri);
                    std::wstring path;
                    if (!g_settings.downloadDirectory.empty()) {
                        v3data::EnsureDirectory(g_settings.downloadDirectory);
                        LPWSTR disposition = nullptr;
                        op->get_ContentDisposition(&disposition);
                        std::wstring file = disposition ? SafeDownloadName(disposition) : L"";
                        if (disposition) CoTaskMemFree(disposition);
                        if (file.empty() || file == L"download.bin") file = DownloadNameFromUri(u);
                        path = UniqueDownloadPath(g_settings.downloadDirectory, file);
                        args->put_ResultFilePath(path.c_str());
                    }
                    ActiveDownload d;
                    d.operation = op;
                    d.record = {path.empty() ? L"Download" : SafeDownloadName(path.substr(path.find_last_of(L"\\/") + 1)), path, u, L"In progress"};
                    d.privateMode = tab->privateMode;
                    g_downloads.push_back(d);
                    SavePersistentData();
                    return S_OK;
                });
                EventRegistrationToken downloadToken{};
                web4->add_DownloadStarting(downloadCallback.Get(), &downloadToken);
                downloadCallback.Get()->Release();
            }
            ResizeChildren();
            if (initialUri.empty() || initialUri == L"about:blank" || initialUri == L"dashboard") {
                return tab->webview->NavigateToString(DashboardHtml());
            }
            return tab->webview->Navigate(initialUri.c_str());
        });
    HRESULT hr = E_FAIL;
    ComPtr<ICoreWebView2Environment10> env10;
    if (SUCCEEDED(g_environment->QueryInterface(IID_ICoreWebView2Environment10, reinterpret_cast<void**>(env10.GetAddressOf()))) && env10) {
        ComPtr<ICoreWebView2ControllerOptions> options;
        if (SUCCEEDED(env10->CreateCoreWebView2ControllerOptions(&options)) && options) {
            options->put_IsInPrivateModeEnabled(privateMode ? TRUE : FALSE);
            hr = env10->CreateCoreWebView2ControllerWithOptions(g_main, options.Get(), controllerCallback.Get());
        }
    }
    if (FAILED(hr) && !privateMode) hr = g_environment->CreateCoreWebView2Controller(g_main, controllerCallback.Get());
    if (FAILED(hr) && privateMode) { int failedIndex = FindTab(tab.get()); if (failedIndex >= 0) { g_tabs.erase(g_tabs.begin() + failedIndex); g_activeTab = g_tabs.empty() ? -1 : static_cast<int>(g_tabs.size()) - 1; SetActiveTab(g_activeTab); } MessageBoxW(g_main, L"Private browsing is unavailable with this WebView2 Runtime.", L"Pane Browser", MB_OK | MB_ICONWARNING); }
    controllerCallback.Get()->Release();
    if (FAILED(hr)) {
        int failedIndex = FindTab(tab.get());
        if (failedIndex >= 0) {
            g_tabs.erase(g_tabs.begin() + failedIndex);
            g_activeTab = g_tabs.empty() ? -1 : static_cast<int>(g_tabs.size()) - 1;
            SetActiveTab(g_activeTab);
        }
    }
}

bool CheckWebView2Runtime() { LPWSTR version=nullptr; HRESULT hr=GetAvailableCoreWebView2BrowserVersionString(nullptr,&version); if(SUCCEEDED(hr)&&version){CoTaskMemFree(version);return true;} MessageBoxW(g_main,L"Microsoft WebView2 Runtime is required. Install the Evergreen WebView2 Runtime, then start Pane Browser again.",L"Pane Browser",MB_OK|MB_ICONERROR); if(version)CoTaskMemFree(version); return false; }

HRESULT InitializeWebView() {
    std::wstring userDataFolder = GetWebViewUserDataFolder();
    if (!userDataFolder.empty()) CreateDirectoryW(userDataFolder.c_str(), nullptr);
    auto environmentCallback = Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(
        [](HRESULT result, ICoreWebView2Environment* environment) -> HRESULT {
            if (FAILED(result) || !environment) {
                MessageBoxW(g_main, L"Microsoft WebView2 Runtime was not found or could not be initialized.", L"Pane Browser", MB_OK | MB_ICONERROR);
                return result;
            }
            g_environment = environment;
            if (!g_restoreSession.empty()) { if (g_settings.tabsEnabled) { for (const auto& entry : g_restoreSession) CreateNewTab(entry.uri, false); SetActiveTab(g_restoreActive); } else { int restoreIndex = std::max(0, std::min(g_restoreActive, static_cast<int>(g_restoreSession.size()) - 1)); CreateNewTab(g_restoreSession[static_cast<size_t>(restoreIndex)].uri, false); } } else CreateNewTab(L"dashboard");
            return S_OK;
        });
    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(
        nullptr, userDataFolder.empty() ? nullptr : userDataFolder.c_str(), nullptr, environmentCallback.Get());
    environmentCallback.Get()->Release();
    return hr;
}

void RefreshRuntimeTheme() {
    g_darkMode = DetectWindowsDarkMode();
    ApplyWindowTheme(g_main);
    ApplyControlTheme();
    InvalidateTabs();
    HWND controls[] = {g_tabStrip, g_back, g_forward, g_reload, g_home, g_historyButton, g_bookmarksButton, g_settingsButton, g_privateButton, g_downloadsButton, g_findButton, g_overflowButton, g_address};
    for (HWND control : controls) {
        if (control) InvalidateRect(control, nullptr, TRUE);
    }
    for (auto& tab : g_tabs) {
        if (!tab->webview) continue;
        if (tab->historyPage) {
            tab->webview->NavigateToString(HistoryHtml().c_str());
        } else if (tab->bookmarksPage) {
            tab->webview->NavigateToString(BookmarksHtml().c_str());
        } else if (tab->settingsPage) {
            tab->webview->NavigateToString(SettingsHtml().c_str());
        } else if (tab->downloadsPage) {
            tab->webview->NavigateToString(DownloadsHtml().c_str());
        } else if (tab->lastUri == L"about:blank") {
            tab->webview->NavigateToString(DashboardHtml());
        }
    }
    UpdateDownloadStates();
    if (g_main) {
        InvalidateRect(g_main, nullptr, TRUE);
        UpdateWindow(g_main);
    }
}

void ApplyCustomTitleBar() {
    if (!g_main) return;
    LONG_PTR style = GetWindowLongPtrW(g_main, GWL_STYLE);
    if (g_settings.customTitleBar) {
        style &= ~static_cast<LONG_PTR>(WS_CAPTION | WS_SYSMENU);
        style |= WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
    } else {
        style |= WS_CAPTION | WS_SYSMENU;
    }
    SetWindowLongPtrW(g_main, GWL_STYLE, style);
    SetWindowPos(g_main, nullptr, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
    ApplyWindowTheme(g_main);
    ResizeChildren();
    InvalidateRect(g_main, nullptr, TRUE);
}

void ShowOverflowMenu() {
    if (!g_overflowButton) return;
    HMENU menu = CreatePopupMenu(); if (!menu) return;
    AppendMenuW(menu, MF_STRING, kFindId, L"Find in page");
    AppendMenuW(menu, MF_STRING, kDownloadsId, L"Downloads");
    AppendMenuW(menu, MF_STRING, kPrivateId, L"Private tab");
    AppendMenuW(menu, MF_STRING, kSettingsId, L"Settings");
    AppendMenuW(menu, MF_STRING, kBookmarksId, L"Bookmarks");
    AppendMenuW(menu, MF_STRING, kHistoryId, L"History");
    RECT r{}; GetWindowRect(g_overflowButton, &r);
    TrackPopupMenu(menu, TPM_RIGHTALIGN | TPM_BOTTOMALIGN, r.right, r.bottom, 0, g_main, nullptr);
    DestroyMenu(menu);
}

void ShowNativeContextMenu(POINT pt) {
    if (!g_main) return;
    HMENU menu = CreatePopupMenu();
    if (!menu) return;
    AppendMenuW(menu, MF_STRING, kContextNewTab, L"New tab");
    AppendMenuW(menu, MF_STRING, kContextReopen, L"Reopen closed tab");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, kContextFind, L"Find in page");
    AppendMenuW(menu, MF_STRING, kContextZoomIn, L"Zoom in");
    AppendMenuW(menu, MF_STRING, kContextZoomOut, L"Zoom out");
    AppendMenuW(menu, MF_STRING, kContextZoomReset, L"Reset zoom");
    AppendMenuW(menu, MF_SEPARATOR, 0, nullptr);
    AppendMenuW(menu, MF_STRING, kContextHistory, L"History");
    AppendMenuW(menu, MF_STRING, kContextBookmarks, L"Bookmarks");
    AppendMenuW(menu, MF_STRING, kContextDownloads, L"Downloads");
    AppendMenuW(menu, MF_STRING, kContextClearData, L"Clear browsing data");
    if (pt.x < 0 || pt.y < 0) GetCursorPos(&pt);
    TrackPopupMenu(menu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, g_main, nullptr);
    DestroyMenu(menu);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
    case WM_PAINT: {
        if (g_settings.customTitleBar) {
            PAINTSTRUCT ps{}; HDC dc = BeginPaint(hwnd, &ps); RECT r{}; GetClientRect(hwnd, &r);
            RECT title{0, 0, r.right, 32}; HBRUSH b = CreateSolidBrush(g_darkMode ? RGB(28,30,35) : RGB(255,255,255)); FillRect(dc, &title, b); DeleteObject(b);
            SetBkMode(dc, TRANSPARENT); SetTextColor(dc, g_darkMode ? RGB(255,255,255) : RGB(0,0,0));
            RECT text{14, 0, r.right - 150, 32}; DrawTextW(dc, L"Pane Browser", -1, &text, DT_SINGLELINE | DT_VCENTER | DT_LEFT);
            SetTextColor(dc, g_darkMode ? RGB(220,220,220) : RGB(60,60,60));
            RECT buttons{r.right - 138, 0, r.right, 32}; DrawTextW(dc, L"—    □    ×", -1, &buttons, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
            EndPaint(hwnd, &ps); return 0;
        }
        break;
    }
    case WM_NCHITTEST:
        if (g_settings.customTitleBar) { POINT pt{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)}; ScreenToClient(hwnd,&pt); RECT r{};GetClientRect(hwnd,&r); const int b=6; if(pt.y < 32 && pt.x >= r.right - 138) return HTCLIENT; if(pt.y<b&&pt.x<b)return HTTOPLEFT; if(pt.y<b&&pt.x>r.right-b)return HTTOPRIGHT; if(pt.y>r.bottom-b&&pt.x<b)return HTBOTTOMLEFT; if(pt.y>r.bottom-b&&pt.x>r.right-b)return HTBOTTOMRIGHT; if(pt.y<b)return HTTOP; if(pt.y>r.bottom-b)return HTBOTTOM; if(pt.x<b)return HTLEFT; if(pt.x>r.right-b)return HTRIGHT; if(pt.y<32)return HTCAPTION; } break;
    case WM_LBUTTONDOWN:
        if (g_settings.customTitleBar) {
            int x = GET_X_LPARAM(lp), y = GET_Y_LPARAM(lp); RECT r{}; GetClientRect(hwnd, &r);
            if (y < 32 && x >= r.right - 138) { if (x >= r.right - 46) DestroyWindow(hwnd); else if (x >= r.right - 92) ShowWindow(hwnd, IsZoomed(hwnd) ? SW_RESTORE : SW_MAXIMIZE); else ShowWindow(hwnd, SW_MINIMIZE); return 0; }
            if (y < 32) { ReleaseCapture(); SendMessageW(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0); return 0; }
        }
        break;
    case WM_ERASEBKGND: {
        RECT client{};
        GetClientRect(hwnd, &client);
        FillRect(reinterpret_cast<HDC>(wp), &client, ToolbarBackgroundBrush());
        return 1;
    }
    case WM_CTLCOLORBTN:
        SetBkMode(reinterpret_cast<HDC>(wp), TRANSPARENT);
        SetTextColor(reinterpret_cast<HDC>(wp), g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
        return reinterpret_cast<LRESULT>(ToolbarButtonBrush());
    case WM_CTLCOLOREDIT:
        SetBkColor(reinterpret_cast<HDC>(wp), g_darkMode ? RGB(28, 30, 35) : RGB(255, 255, 255));
        SetTextColor(reinterpret_cast<HDC>(wp), g_darkMode ? RGB(255, 255, 255) : RGB(0, 0, 0));
        return reinterpret_cast<LRESULT>(ToolbarBackgroundBrush());
    case WM_DRAWITEM:
        if (reinterpret_cast<DRAWITEMSTRUCT*>(lp) && reinterpret_cast<DRAWITEMSTRUCT*>(lp)->CtlType == ODT_BUTTON) {
            DrawToolbarButton(reinterpret_cast<DRAWITEMSTRUCT*>(lp));
            return TRUE;
        }
        break;
    case WM_SETTINGCHANGE:
    case WM_THEMECHANGED:
    case WM_SYSCOLORCHANGE:
        RefreshRuntimeTheme();
        SetTimer(hwnd, kThemeRefreshTimerId, 150, nullptr);
        return 0;
    case kUpdateResultMessage: {
        std::unique_ptr<UpdateCheckResult> result(reinterpret_cast<UpdateCheckResult*>(lp));
        g_updateCheckRunning.store(false);
        if (!result) return 0;
        if (!result->requestSucceeded) { MessageBoxW(g_main, L"No release information was available.", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        if (result->tag == L"v3.0.0" || result->tag == L"3.0.0") { MessageBoxW(g_main, L"You are using the latest Pane Browser release (v3.0.0).", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        std::wstring prompt = L"Latest GitHub release: " + result->tag + L"\nOpen the releases page to download it?";
        if (MessageBoxW(g_main, prompt.c_str(), L"Pane Browser", MB_YESNO | MB_ICONINFORMATION) == IDYES) ShellExecuteW(g_main, L"open", L"https://github.com/xizar280513/Pane-Browser/releases", nullptr, nullptr, SW_SHOWNORMAL);
        return 0;
    }
    case WM_TIMER:
        if (wp == 0x733) { UpdateDownloadStates(); return 0; }
        if (wp == 0x734) { KillTimer(hwnd, 0x734); CheckForUpdates(); return 0; }
        if (wp == kThemeRefreshTimerId) {
            KillTimer(hwnd, kThemeRefreshTimerId);
            RefreshRuntimeTheme();
            return 0;
        }
        break;
    case WM_CONTEXTMENU: {
        POINT pt{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
        ShowNativeContextMenu(pt);
        return 0;
    }
    case WM_SIZE:
        ResizeChildren();
        return 0;
    case WM_COMMAND:
        switch (LOWORD(wp)) {
        case kBackId: if (ActiveWebView()) ActiveWebView()->GoBack(); return 0;
        case kForwardId: if (ActiveWebView()) ActiveWebView()->GoForward(); return 0;
        case kReloadId: if (ActiveWebView()) ActiveWebView()->Reload(); return 0;
        case kHomeId: ShowHome(); return 0;
        case kHistoryId: ShowHistory(); return 0;
        case kBookmarksId: ShowBookmarks(); return 0;
        case kSettingsId: ShowSettings(); return 0;
        case kPrivateId: ShowPrivate(); return 0;
        case kDownloadsId: ShowDownloads(); return 0;
        case kFindId: FindInPage(); return 0;
        case kOverflowId: ShowOverflowMenu(); return 0;
        case kContextNewTab: if(g_settings.tabsEnabled)CreateNewTab(L"dashboard",false); return 0;
        case kContextReopen: ReopenClosedTab(); return 0;
        case kContextFind: FindInPage(); return 0;
        case kContextZoomIn: AdjustZoom(0.1); return 0;
        case kContextZoomOut: AdjustZoom(-0.1); return 0;
        case kContextZoomReset: ResetZoom(); return 0;
        case kContextHistory: ShowHistory(); return 0;
        case kContextBookmarks: ShowBookmarks(); return 0;
        case kContextDownloads: ShowDownloads(); return 0;
        case kContextClearData: ClearBrowsingData(); return 0;
        }
        break;
    case WM_DESTROY:
        if (g_downloadTimer) KillTimer(hwnd, g_downloadTimer);
        KillTimer(hwnd, 0x734);
        SavePersistentData();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    using SetProcessDpiAwarenessContextFn = BOOL (WINAPI*)(DPI_AWARENESS_CONTEXT);
    HMODULE user32 = GetModuleHandleW(L"user32.dll");
    auto setDpi = user32 ? reinterpret_cast<SetProcessDpiAwarenessContextFn>(GetProcAddress(user32, "SetProcessDpiAwarenessContext")) : nullptr;
    if (!setDpi || !setDpi(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2)) SetProcessDPIAware();
        HRESULT comHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(comHr)) return 1;
    LoadPersistentData();
    g_darkMode = DetectWindowsDarkMode();

    WNDCLASSEXW wc{};
    wc.cbSize = sizeof(wc);
    wc.hInstance = instance;
    wc.lpfnWndProc = WindowProc;
    wc.lpszClassName = L"PaneBrowser";
    wc.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    wc.hIcon = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_PANE_BROWSER), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
    wc.hIconSm = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_PANE_BROWSER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
    wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassExW(&wc);

    WNDCLASSW tabClass{};
    tabClass.hInstance = instance;
    tabClass.lpfnWndProc = TabStripProc;
    tabClass.lpszClassName = L"PaneBrowserTabStrip";
    tabClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    tabClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    RegisterClassW(&tabClass);

    g_main = CreateWindowExW(0, wc.lpszClassName, L"Pane Browser", WS_OVERLAPPEDWINDOW,
                             CW_USEDEFAULT, CW_USEDEFAULT, 1200, 800, nullptr, nullptr, instance, nullptr);
    if (!g_main) {
        CoUninitialize();
        return 1;
    }
        HICON appIconBig = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_PANE_BROWSER), IMAGE_ICON, 32, 32, LR_DEFAULTCOLOR));
    HICON appIconSmall = static_cast<HICON>(LoadImageW(instance, MAKEINTRESOURCEW(IDI_PANE_BROWSER), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR));
    SendMessageW(g_main, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(appIconBig));
    SendMessageW(g_main, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(appIconSmall));
    ApplyWindowTheme(g_main);
    g_tabStrip = CreateWindowExW(0, tabClass.lpszClassName, L"", WS_CHILD | WS_VISIBLE,
                                 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kTabStripId), instance, nullptr);
    g_back = CreateWindowExW(0, L"BUTTON", L"<", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
                             g_main, reinterpret_cast<HMENU>(kBackId), instance, nullptr);
    g_forward = CreateWindowExW(0, L"BUTTON", L">", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
                                g_main, reinterpret_cast<HMENU>(kForwardId), instance, nullptr);
    g_reload = CreateWindowExW(0, L"BUTTON", L"Refresh", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
                               g_main, reinterpret_cast<HMENU>(kReloadId), instance, nullptr);
    g_home = CreateWindowExW(0, L"BUTTON", L"Home", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
                             g_main, reinterpret_cast<HMENU>(kHomeId), instance, nullptr);
    g_historyButton = CreateWindowExW(0, L"BUTTON", L"History", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0,
                                      g_main, reinterpret_cast<HMENU>(kHistoryId), instance, nullptr);
    g_bookmarksButton = CreateWindowExW(0, L"BUTTON", L"Bookmarks", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0,
                                      g_main, reinterpret_cast<HMENU>(kBookmarksId), instance, nullptr);
    g_settingsButton = CreateWindowExW(0, L"BUTTON", L"Settings", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0,
                                      g_main, reinterpret_cast<HMENU>(kSettingsId), instance, nullptr);
    g_privateButton = CreateWindowExW(0, L"BUTTON", L"Private", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0,
                                      g_main, reinterpret_cast<HMENU>(kPrivateId), instance, nullptr);
    g_downloadsButton = CreateWindowExW(0, L"BUTTON", L"Downloads", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kDownloadsId), instance, nullptr);
    g_findButton = CreateWindowExW(0, L"BUTTON", L"Find", WS_CHILD | BS_OWNERDRAW, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kFindId), instance, nullptr);
    g_address = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                                0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kAddressId), instance, nullptr);
    g_overflowButton = CreateWindowExW(0, L"BUTTON", L"⋮", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kOverflowId), instance, nullptr);
    g_downloadTimer = SetTimer(g_main, 0x733, 750, nullptr);
    if (g_settings.checkUpdates) SetTimer(g_main, 0x734, 3000, nullptr);
    g_oldEditProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
        g_address, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(AddressProc)));
        ApplyControlTheme();
    ApplyCustomTitleBar();
    ShowWindow(g_main, show);
    ResizeChildren();
    UpdateWindow(g_main);

    if (!CheckWebView2Runtime()) { DestroyWindow(g_main); CoUninitialize(); return 1; }
    HRESULT hr = InitializeWebView();
    if (FAILED(hr)) { MessageBoxW(g_main, L"WebView2 initialization failed.", L"Pane Browser", MB_OK | MB_ICONERROR); DestroyWindow(g_main); CoUninitialize(); return 1; }

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (HandleGlobalShortcutMessage(msg)) continue;
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    for (auto& tab : g_tabs) {
        tab->webview.Reset();
        tab->controller.Reset();
    }
    g_tabs.clear();
    g_environment.Reset();
    CoUninitialize();
    return static_cast<int>(msg.wParam);
}
