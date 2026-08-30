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
#include <cctype>
#include "resource.h"
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
        (void)riid;
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
        (void)riid;
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
        (void)riid;
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
class CallbackHolder<ICoreWebView2NewWindowRequestedEventHandler, Fn>
    : public ICoreWebView2NewWindowRequestedEventHandler {
    volatile LONG refs_ = 1;
    Fn fn_;
public:
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    virtual ~CallbackHolder() = default;
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override {
        if (!out) return E_POINTER;
        (void)riid;
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
        (void)riid;
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
constexpr int kAddressId = 0x6a;
constexpr int kTabStripId = 0x70;
constexpr int kTabWidth = 174;
constexpr UINT kThemeRefreshTimerId = 0x5A7;

struct BrowserTab {
    ComPtr<ICoreWebView2Controller> controller;
    ComPtr<ICoreWebView2> webview;
    std::wstring lastUri = L"about:blank";
    bool internalPage = true;
    bool historyPage = false;
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
HWND g_address = nullptr;
WNDPROC g_oldEditProc = nullptr;
ComPtr<ICoreWebView2Environment> g_environment;
std::vector<std::shared_ptr<BrowserTab>> g_tabs;
std::vector<HistoryEntry> g_history;
int g_activeTab = -1;
bool g_darkMode = false;

const wchar_t* kDashboardHtmlDark = LR"HTML(<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Pane Browser</title><style>
html,body{height:100%;}body{margin:0;background:#202124;color:#f1f3f4;font-family:Segoe UI,system-ui,sans-serif}.shell{height:100%;display:flex;align-items:center;justify-content:center;padding:24px;box-sizing:border-box}.content{width:min(640px,100%);display:flex;flex-direction:column;align-items:center;gap:24px}h1{margin:0;font-size:30px;font-weight:600;letter-spacing:.1px;color:#f1f3f4}.search{width:100%;height:66px;display:flex;align-items:center;background:#303134;border:1px solid #5f6368;border-radius:4px;box-shadow:0 2px 6px rgba(0,0,0,.35);box-sizing:border-box}.search input{flex:1;min-width:0;height:100%;padding:0 20px;border:0;outline:0;background:transparent;color:#f1f3f4;font:inherit;font-size:18px}.search input::placeholder{color:#9aa0a6}.search input[type=search]::-webkit-search-cancel-button{-webkit-appearance:none;display:none}.search button{width:62px;height:100%;display:grid;place-items:center;border:0;background:transparent;color:#bdc1c6;cursor:pointer}.search button:hover{color:#f1f3f4}.search svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2.5;stroke-linecap:round}
</style></head><body><main class="shell"><section class="content"><h1>Pane Browser</h1><form class="search" onsubmit="submitSearch();return false;"><input id="q" type="search" autocomplete="off" autofocus><button type="submit" aria-label="Search"><svg viewBox="0 0 24 24"><circle cx="10.8" cy="10.8" r="6.8"></circle><path d="M16 16l5 5"></path></svg></button></form></section></main><script>function submitSearch(){const q=document.getElementById('q').value.trim();if(!q)return;const uri='https://www.startpage.com/sp/search?query='+encodeURIComponent(q);if(window.chrome&&chrome.webview)chrome.webview.postMessage('search:'+q);else window.location.href=uri;}</script></body></html>)HTML";

const wchar_t* kDashboardHtmlLight = LR"HTML(<!DOCTYPE html><html><head><meta charset="UTF-8"><title>Pane Browser</title><style>
html,body{height:100%;}body{margin:0;background:#ffffff;color:#202124;font-family:Segoe UI,system-ui,sans-serif}.shell{height:100%;display:flex;align-items:center;justify-content:center;padding:24px;box-sizing:border-box}.content{width:min(640px,100%);display:flex;flex-direction:column;align-items:center;gap:24px}h1{margin:0;font-size:30px;font-weight:600;letter-spacing:.1px;color:#202124}.search{width:100%;height:66px;display:flex;align-items:center;background:#ffffff;border:1px solid #dadce0;border-radius:4px;box-shadow:0 2px 6px rgba(60,64,67,.18);box-sizing:border-box}.search input{flex:1;min-width:0;height:100%;padding:0 20px;border:0;outline:0;background:transparent;color:#202124;font:inherit;font-size:18px}.search input::placeholder{color:#9aa0a6}.search input[type=search]::-webkit-search-cancel-button{-webkit-appearance:none;display:none}.search button{width:62px;height:100%;display:grid;place-items:center;border:0;background:transparent;color:#5f6368;cursor:pointer}.search button:hover{color:#202124}.search svg{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:2.5;stroke-linecap:round}
</style></head><body><main class="shell"><section class="content"><h1>Pane Browser</h1><form class="search" onsubmit="submitSearch();return false;"><input id="q" type="search" autocomplete="off" autofocus><button type="submit" aria-label="Search"><svg viewBox="0 0 24 24"><circle cx="10.8" cy="10.8" r="6.8"></circle><path d="M16 16l5 5"></path></svg></button></form></section></main><script>function submitSearch(){const q=document.getElementById('q').value.trim();if(!q)return;const uri='https://www.startpage.com/sp/search?query='+encodeURIComponent(q);if(window.chrome&&chrome.webview)chrome.webview.postMessage('search:'+q);else window.location.href=uri;}</script></body></html>)HTML";

bool DetectWindowsDarkMode() {
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
        HWND controls[] = {g_tabStrip, g_back, g_forward, g_reload, g_home, g_historyButton, g_address};
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

std::wstring GetWebViewUserDataFolder() {
    wchar_t modulePath[32768] = {};
    DWORD length = GetModuleFileNameW(nullptr, modulePath, static_cast<DWORD>(sizeof(modulePath) / sizeof(modulePath[0])));
    if (!length || length >= sizeof(modulePath) / sizeof(modulePath[0])) return L"";
    std::wstring fullPath(modulePath, length);
    size_t slash = fullPath.find_last_of(L"\\/");
    std::wstring directory = slash == std::wstring::npos ? L"." : fullPath.substr(0, slash + 1);
    std::wstring executableName = slash == std::wstring::npos ? fullPath : fullPath.substr(slash + 1);
    return directory + executableName + L".WebView2";
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

std::wstring NormalizeInput(const std::wstring& input) {
    if (input.empty()) return L"";
    if (input.rfind(L"http://", 0) == 0 || input.rfind(L"https://", 0) == 0) return input;
    if (input.find(L'.') != std::wstring::npos) return L"https://" + input;
    return L"https://www.startpage.com/sp/search?query=" + UrlEncode(input);
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
    if (tab.lastUri.empty() || tab.lastUri == L"about:blank") return L"New Tab";
    std::wstring title = tab.lastUri;
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
void CreateNewTab(const std::wstring& initialUri);
void CloseTab(int index);
void SetActiveTab(int index);

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
        if (x >= plusLeft && x < plusLeft + 42 && y >= 0 && y < 32) CreateNewTab(L"dashboard");
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
        tab->lastUri = L"about:blank";
        tab->internalPage = true;
        tab->historyPage = false;
        if (tab->webview) tab->webview->NavigateToString(DashboardHtml());
        if (g_address) SetWindowTextW(g_address, L"");
        SetTabLabel(tab);
        SetActiveTab(0);
        return;
    }
    g_tabs[index]->webview.Reset();
    g_tabs[index]->controller.Reset();
    g_tabs.erase(g_tabs.begin() + index);
    if (g_activeTab > index) --g_activeTab;
    else if (g_activeTab >= static_cast<int>(g_tabs.size())) g_activeTab = static_cast<int>(g_tabs.size()) - 1;
    SetActiveTab(g_activeTab);
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
    tab->lastUri = uri;
    if (!tab->internalPage) {
        g_history.push_back({uri, CurrentTimeLabel()});
    }
    if (tab == ActiveTab()) {
        SetWindowTextW(g_address, uri.c_str());
        SetTabLabel(tab);
    }
}

std::wstring HistoryHtml() {
    const wchar_t* css = g_darkMode
        ? LR"CSS(html,body{min-height:100%;}body{margin:0;font-family:system-ui,Segoe UI,sans-serif;background:#202124;color:#e8eaed}.container{max-width:1200px;margin:auto;padding:36px 28px}.card{background:#292a2d;border-radius:16px;padding:28px 26px 36px;box-shadow:0 0 36px rgba(0,0,0,.35)}h1{margin:0 0 22px;font-family:Segoe UI,system-ui,sans-serif;font-size:34px;font-weight:600;letter-spacing:.1px;color:#f1f3f4}.search{width:100%;box-sizing:border-box;background:#303134;color:#e8eaed;border:1px solid #5f6368;border-radius:26px;padding:13px 20px;font-size:17px;outline:none}.selection{display:flex;align-items:center;gap:12px;padding:16px 2px 8px;color:#e8eaed}.selection.show{display:flex}.selection .when-selected{display:none}.selection.has-selection .when-selected{display:inline-block}.selection button,.menu button{background:transparent;border:0;color:#8ab4f8;padding:8px 10px;border-radius:6px;cursor:pointer;font-size:14px}.selection button:hover,.menu button:hover{background:#3c4043}.row{display:grid;grid-template-columns:28px 70px minmax(0,1fr) 42px;align-items:center;gap:10px;min-height:66px;border-bottom:1px solid #3c4043}.pick{width:18px;height:18px;accent-color:#8ab4f8}.time{color:#bdc1c6;font-size:15px}.site{display:flex;align-items:center;gap:13px;min-width:0}.site img{width:24px;height:24px;border-radius:4px;object-fit:contain}.title{display:block;border:0;background:transparent;color:#e8eaed;font-size:17px;font-weight:650;padding:0;cursor:pointer;text-align:left;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;max-width:100%}.host{color:#bdc1c6;font-size:15px;margin-top:5px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.morewrap{position:relative;text-align:center}.more{border:0;background:transparent;color:#bdc1c6;font-size:24px;cursor:pointer}.menu{display:none;position:absolute;right:0;top:30px;z-index:5;min-width:190px;background:#303134;border:1px solid #5f6368;box-shadow:0 5px 18px rgba(0,0,0,.4);padding:6px;border-radius:6px;text-align:left}.menu.open{display:block}.empty{color:#9aa0a6;font-size:17px})CSS"
        : LR"CSS(html,body{min-height:100%;}body{margin:0;font-family:system-ui,Segoe UI,sans-serif;background:#f8f9fa;color:#202124}.container{max-width:1200px;margin:auto;padding:36px 28px}.card{background:#ffffff;border:1px solid #dadce0;border-radius:16px;padding:28px 26px 36px;box-shadow:0 0 36px rgba(60,64,67,.16)}h1{margin:0 0 22px;font-family:Segoe UI,system-ui,sans-serif;font-size:34px;font-weight:600;letter-spacing:.1px;color:#202124}.search{width:100%;box-sizing:border-box;background:#ffffff;color:#202124;border:1px solid #dadce0;border-radius:26px;padding:13px 20px;font-size:17px;outline:none}.selection{display:flex;align-items:center;gap:12px;padding:16px 2px 8px;color:#202124}.selection.show{display:flex}.selection .when-selected{display:none}.selection.has-selection .when-selected{display:inline-block}.selection button,.menu button{background:transparent;border:0;color:#1967d2;padding:8px 10px;border-radius:6px;cursor:pointer;font-size:14px}.selection button:hover,.menu button:hover{background:#f1f3f4}.row{display:grid;grid-template-columns:28px 70px minmax(0,1fr) 42px;align-items:center;gap:10px;min-height:66px;border-bottom:1px solid #e8eaed}.pick{width:18px;height:18px;accent-color:#1a73e8}.time{color:#5f6368;font-size:15px}.site{display:flex;align-items:center;gap:13px;min-width:0}.site img{width:24px;height:24px;border-radius:4px;object-fit:contain}.title{display:block;border:0;background:transparent;color:#202124;font-size:17px;font-weight:650;padding:0;cursor:pointer;text-align:left;white-space:nowrap;overflow:hidden;text-overflow:ellipsis;max-width:100%}.host{color:#5f6368;font-size:15px;margin-top:5px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.morewrap{position:relative;text-align:center}.more{border:0;background:transparent;color:#5f6368;font-size:24px;cursor:pointer}.menu{display:none;position:absolute;right:0;top:30px;z-index:5;min-width:190px;background:#ffffff;border:1px solid #dadce0;box-shadow:0 5px 18px rgba(60,64,67,.25);padding:6px;border-radius:6px;text-align:left}.menu.open{display:block}.empty{color:#5f6368;font-size:17px})CSS";
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
            html += L"<div class=\"row\" data-index=\"" + std::to_wstring(n) + L"\" data-search=\"" + HtmlEscape(label + L" " + host + L" " + entry.uri) + L"\"><input type=\"checkbox\" class=\"pick\" onchange=\"syncSelection()\"><div class=\"time\">" + HtmlEscape(entry.time) + L"</div><div class=\"site\"><img src=\"https://www.google.com/s2/favicons?domain=" + HtmlEscape(host) + L"&sz=32\" onerror=\"this.style.display='none'\"><div><button class=\"title\" onclick=\"openEntry(" + std::to_wstring(n) + L")\">" + HtmlEscape(label) + L"</button><div class=\"host\">" + HtmlEscape(entry.uri) + L"</div></div></div><div class=\"morewrap\"><button class=\"more\" onclick=\"toggleMenu(this)\">⋮</button><div class=\"menu\"><button onclick=\"openEntry(" + std::to_wstring(n) + L")\">Open</button><button onclick=\"deleteEntry(" + std::to_wstring(n) + L")\">Delete from history</button></div></div></div>";
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

void RefreshHistoryPage() {
    BrowserTab* tab = ActiveTab();
    ICoreWebView2* webview = ActiveWebView();
    if (!tab || !webview) return;
    tab->internalPage = true;
    tab->historyPage = true;
    std::wstring html = HistoryHtml();
    webview->NavigateToString(html.c_str());
}

void HandleHistoryMessage(BrowserTab* tab, ICoreWebView2WebMessageReceivedEventArgs* args) {
    if (!tab || !args) return;
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


    if (message.rfind(L"delete-selected:", 0) == 0) {
        std::vector<int> indices = ParseHistoryIndices(message.substr(16));
        for (int index : indices) g_history.erase(g_history.begin() + index);
        RefreshHistoryPage();
        return;
    }

    if (message.rfind(L"delete:", 0) == 0) {
        int index = -1;
        if (ParseHistoryIndex(message.substr(7), index)) g_history.erase(g_history.begin() + index);
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

void ShowHistory() {
    BrowserTab* tab = ActiveTab();
    ICoreWebView2* webview = ActiveWebView();
    if (!tab || !webview) return;
    SetWindowTextW(g_address, L"");
    tab->internalPage = true;
    tab->historyPage = true;
    std::wstring html = HistoryHtml();
    webview->NavigateToString(html.c_str());
}

void ShowHome() {
    BrowserTab* tab = ActiveTab();
    if (!tab || !tab->webview) return;
    tab->lastUri = L"about:blank";
    tab->internalPage = true;
    tab->historyPage = false;
    SetTabLabel(tab);
    SetWindowTextW(g_address, L"");
    tab->webview->NavigateToString(DashboardHtml());
}

void ResizeChildren() {
    if (!g_main) return;
    RECT rc{};
    GetClientRect(g_main, &rc);
    int width = rc.right - rc.left;
    MoveWindow(g_tabStrip, 0, 0, std::max(200, width), 32, TRUE);
    MoveWindow(g_back, 5, 36, 24, 30, TRUE);
    MoveWindow(g_forward, 33, 36, 24, 30, TRUE);
    MoveWindow(g_reload, 61, 36, 58, 30, TRUE);
    MoveWindow(g_home, 124, 36, 48, 30, TRUE);
    MoveWindow(g_historyButton, 177, 36, 60, 30, TRUE);
    MoveWindow(g_address, 242, 36, std::max<LONG>(120, width - 245), 30, TRUE);
    for (size_t i = 0; i < g_tabs.size(); ++i) {
        if (g_tabs[i]->controller) {
            RECT webBounds = rc;
            webBounds.top = 70;
            g_tabs[i]->controller->put_Bounds(webBounds);
            g_tabs[i]->controller->put_IsVisible(static_cast<int>(i) == g_activeTab ? TRUE : FALSE);
        }
    }
}

LRESULT CALLBACK AddressProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    if (msg == WM_KEYDOWN && wp == VK_RETURN) {
        NavigateFromAddressBar();
        return 0;
    }
    return g_oldEditProc ? CallWindowProcW(g_oldEditProc, hwnd, msg, wp, lp) : DefWindowProcW(hwnd, msg, wp, lp);
}

void CreateNewTab(const std::wstring& initialUri) {
    if (!g_environment || !g_tabStrip) return;
    auto tab = std::make_shared<BrowserTab>();
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
                            tab->internalPage = false;
                            tab->historyPage = false;
                            sender->Navigate(uri.c_str());
                        }
                    }
                    args->put_Handled(TRUE);
                    return S_OK;
                });
            EventRegistrationToken newWindowToken{};
            tab->webview->add_NewWindowRequested(newWindowCallback.Get(), &newWindowToken);
            newWindowCallback.Get()->Release();
            ResizeChildren();
            if (initialUri.empty() || initialUri == L"about:blank" || initialUri == L"dashboard") {
                return tab->webview->NavigateToString(DashboardHtml());
            }
            return tab->webview->Navigate(initialUri.c_str());
        });
    HRESULT hr = g_environment->CreateCoreWebView2Controller(g_main, controllerCallback.Get());
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
            CreateNewTab(L"dashboard");
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
    HWND controls[] = {g_tabStrip, g_back, g_forward, g_reload, g_home, g_historyButton, g_address};
    for (HWND control : controls) {
        if (control) InvalidateRect(control, nullptr, TRUE);
    }
    for (auto& tab : g_tabs) {
        if (!tab->webview) continue;
        if (tab->historyPage) {
            tab->webview->NavigateToString(HistoryHtml().c_str());
        } else if (tab->lastUri == L"about:blank") {
            tab->webview->NavigateToString(DashboardHtml());
        }
    }
    if (g_main) {
        InvalidateRect(g_main, nullptr, TRUE);
        UpdateWindow(g_main);
    }
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {
    switch (msg) {
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
    case WM_TIMER:
        if (wp == kThemeRefreshTimerId) {
            KillTimer(hwnd, kThemeRefreshTimerId);
            RefreshRuntimeTheme();
            return 0;
        }
        break;
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
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    SetProcessDPIAware();
        HRESULT comHr = CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
    if (FAILED(comHr)) return 1;
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
    g_historyButton = CreateWindowExW(0, L"BUTTON", L"History", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0,
                                      g_main, reinterpret_cast<HMENU>(kHistoryId), instance, nullptr);
    g_address = CreateWindowExW(WS_EX_CLIENTEDGE, L"EDIT", L"", WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL,
                                0, 0, 0, 0, g_main, reinterpret_cast<HMENU>(kAddressId), instance, nullptr);
    g_oldEditProc = reinterpret_cast<WNDPROC>(SetWindowLongPtrW(
        g_address, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(AddressProc)));
    ApplyControlTheme();

    ShowWindow(g_main, show);
    ResizeChildren();
    UpdateWindow(g_main);

    HRESULT hr = InitializeWebView();
    if (FAILED(hr)) MessageBoxW(g_main, L"WebView2 initialization failed.", L"Pane Browser", MB_OK | MB_ICONERROR);

    MSG msg{};
    while (GetMessageW(&msg, nullptr, 0, 0) > 0) {
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
