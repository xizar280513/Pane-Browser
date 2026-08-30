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
#include <map>
#include <shellapi.h>
#include <commdlg.h>
#include <wininet.h>
#include "resource.h"
#include "v3_data.h"
#include "v4_data.h"
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
class CallbackHolder<ICoreWebView2WindowCloseRequestedEventHandler, Fn> : public ICoreWebView2WindowCloseRequestedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2WindowCloseRequestedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2WindowCloseRequestedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, IUnknown* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2ProcessFailedEventHandler, Fn> : public ICoreWebView2ProcessFailedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2ProcessFailedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2ProcessFailedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2ProcessFailedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2BrowserProcessExitedEventHandler, Fn> : public ICoreWebView2BrowserProcessExitedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2BrowserProcessExitedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2BrowserProcessExitedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2Environment* sender, ICoreWebView2BrowserProcessExitedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2TrySuspendCompletedHandler, Fn> : public ICoreWebView2TrySuspendCompletedHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2TrySuspendCompletedHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2TrySuspendCompletedHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(HRESULT errorCode, BOOL result) override { return fn_(errorCode, result); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2PermissionRequestedEventHandler, Fn> : public ICoreWebView2PermissionRequestedEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2PermissionRequestedEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2PermissionRequestedEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2PermissionRequestedEventArgs* args) override { return fn_(sender, args); }
};

template <typename Fn>
class CallbackHolder<ICoreWebView2NavigationStartingEventHandler, Fn> : public ICoreWebView2NavigationStartingEventHandler {
    volatile LONG refs_ = 1; Fn fn_;
public:
    virtual ~CallbackHolder() = default;
    explicit CallbackHolder(Fn fn) : fn_(std::move(fn)) {}
    HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, void** out) override { if (!out) return E_POINTER; if (riid != IID_IUnknown && riid != IID_ICoreWebView2NavigationStartingEventHandler) { *out = nullptr; return E_NOINTERFACE; } *out = static_cast<ICoreWebView2NavigationStartingEventHandler*>(this); AddRef(); return S_OK; }
    ULONG STDMETHODCALLTYPE AddRef() override { return InterlockedIncrement(&refs_); }
    ULONG STDMETHODCALLTYPE Release() override { ULONG r = InterlockedDecrement(&refs_); if (!r) delete this; return r; }
    HRESULT STDMETHODCALLTYPE Invoke(ICoreWebView2* sender, ICoreWebView2NavigationStartingEventArgs* args) override { return fn_(sender, args); }
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
constexpr int kContextCommandPalette = 0x520B;
constexpr int kContextSiteData = 0x520C;
constexpr int kContextWorkspace = 0x520D;
constexpr int kContextNotes = 0x520E;
constexpr int kContextReading = 0x520F;
constexpr int kContextSplit = 0x5210;
constexpr int kAddressId = 0x6a;
constexpr int kTabStripId = 0x70;
constexpr int kTabWidth = 174;
constexpr UINT kThemeRefreshTimerId = 0x5A7;
constexpr UINT kUpdateResultMessage = WM_APP + 0x31;
constexpr UINT kRecoverWebViewsMessage = WM_APP + 0x42;
constexpr UINT kRecoveryTimerId = 0x744;

struct BrowserTab {
    ComPtr<ICoreWebView2Controller> controller;
    ComPtr<ICoreWebView2> webview;
    std::wstring lastUri = L"about:blank";
    std::wstring profileId = L"personal";
    std::wstring profileName = L"Personal";
    std::wstring workspaceId = L"default";
    std::wstring groupId;
    unsigned long long generation = 1;
    bool internalPage = true;
    bool historyPage = false;
    bool bookmarksPage = false;
    bool settingsPage = false;
    bool downloadsPage = false;
    bool privateMode = false;
    bool guestMode = false;
    bool sleeping = false;
    bool crashed = false;
    bool authenticationWindow = false;
    bool commandPalettePage = false;
    bool notesPage = false;
    bool siteDataPage = false;
    bool workspacePage = false;
    bool readingMode = false;
    ULONGLONG lastActiveTick = 0;
};

struct RecoveryDescriptor {
    std::wstring uri;
    std::wstring profileId;
    std::wstring profileName;
    std::wstring workspaceId;
    std::wstring groupId;
    bool privateMode = false;
    bool authenticationWindow = false;
};

struct RestoredTabMetadata {
    std::wstring uri;
    std::wstring workspaceId;
    std::wstring groupId;
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
std::vector<v4data::ProfileInfo> g_profiles;
v4data::ProfileInfo g_activeProfile{L"personal", L"Personal", L"normal", false};
std::wstring g_profileDataDirectory;
std::wstring g_profileSwitchTarget;
std::wstring g_profileDeleteTarget;
bool g_profileDeletePending = false;
bool g_deleteAllInProgress = false;
unsigned long long g_profileGeneration = 1;
std::atomic_bool g_recoveryScheduled{false};
std::atomic_bool g_recoveryInProgress{false};
std::vector<RecoveryDescriptor> g_recoveryDescriptors;
int g_recoveryActiveNormal = 0;
std::atomic_bool g_recoveryReady{false};
std::wstring g_quickNote;
bool g_splitView = false;
int g_splitSecondTab = -1;
std::vector<RestoredTabMetadata> g_restoreTabMetadata;
bool g_restoringSession = false;
size_t g_restoreCursor = 0;

struct SitePolicy {
    bool scriptsEnabled = true;
    bool scriptDialogsEnabled = true;
};

std::map<std::wstring, SitePolicy> g_sitePolicies;
std::map<std::wstring, COREWEBVIEW2_PERMISSION_STATE> g_permissionPolicies;



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

v4data::ProfileInfo* FindProfileInfo(const std::wstring& id) {
    for (auto& profile : g_profiles) if (_wcsicmp(profile.id.c_str(), id.c_str()) == 0) return &profile;
    return nullptr;
}

void LoadProfileContext() {
    v4data::LoadProfiles(g_profiles);
    std::wstring wanted = v4data::LoadActiveProfile();
    v4data::ProfileInfo* found = FindProfileInfo(wanted);
    if (!found) found = FindProfileInfo(L"personal");
    if (!found) {
        g_profiles.insert(g_profiles.begin(), {L"personal", L"Personal", L"normal", false});
        found = &g_profiles.front();
    }
    g_activeProfile = *found;
    if (g_activeProfile.kind == L"private") g_activeProfile = {L"personal", L"Personal", L"normal", false};
    v4data::EnsureProfileLayout(g_activeProfile);
    g_profileDataDirectory = v4data::ProfileDataDirectory(g_activeProfile);
    g_dataDirectory = g_profileDataDirectory;
    v4data::SaveActiveProfile(g_activeProfile.id);
}

std::wstring WebViewProfileName(const BrowserTab& tab) {
    if (tab.privateMode) return tab.profileId == L"personal" ? L"" : v4data::SanitizeId(tab.profileId);
    if (tab.profileId == L"personal") return L""; // preserve the v3 default profile and cookies
    return v4data::SanitizeId(tab.profileId);
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

std::wstring OriginFromUri(const std::wstring& uri) {
    size_t scheme = uri.find(L"://");
    if (scheme == std::wstring::npos) return L"";
    size_t start = scheme + 3;
    size_t end = uri.find_first_of(L"/?#", start);
    std::wstring origin = uri.substr(0, end == std::wstring::npos ? std::wstring::npos : end);
    for (wchar_t& c : origin) c = static_cast<wchar_t>(towlower(c));
    return origin;
}

std::wstring PermissionPolicyKey(const std::wstring& origin, COREWEBVIEW2_PERMISSION_KIND kind) {
    return origin + L"|" + std::to_wstring(static_cast<int>(kind));
}

std::wstring PermissionKindLabel(COREWEBVIEW2_PERMISSION_KIND kind) {
    switch (kind) {
        case COREWEBVIEW2_PERMISSION_KIND_CAMERA: return L"Camera";
        case COREWEBVIEW2_PERMISSION_KIND_MICROPHONE: return L"Microphone";
        case COREWEBVIEW2_PERMISSION_KIND_GEOLOCATION: return L"Location";
        case COREWEBVIEW2_PERMISSION_KIND_NOTIFICATIONS: return L"Notifications";
        case COREWEBVIEW2_PERMISSION_KIND_CLIPBOARD_READ: return L"Clipboard read";
        case COREWEBVIEW2_PERMISSION_KIND_AUTOPLAY: return L"Autoplay";
        default: return L"Other permission";
    }
}

SitePolicy GetSitePolicy(const std::wstring& origin) {
    auto it = g_sitePolicies.find(origin);
    return it == g_sitePolicies.end() ? SitePolicy{} : it->second;
}

void ApplySitePolicy(BrowserTab* tab) {
    if (!tab || !tab->webview || tab->privateMode) return;
    std::wstring origin = OriginFromUri(tab->lastUri);
    if (origin.empty()) return;
    SitePolicy policy = GetSitePolicy(origin);
    ComPtr<ICoreWebView2Settings> settings;
    if (SUCCEEDED(tab->webview->get_Settings(&settings)) && settings) {
        settings->put_IsScriptEnabled(policy.scriptsEnabled ? TRUE : FALSE);
        settings->put_AreDefaultScriptDialogsEnabled(policy.scriptDialogsEnabled ? TRUE : FALSE);
    }
}

void SaveSitePolicies() {
    if (g_profileDataDirectory.empty()) return;
    std::vector<std::string> siteLines;
    for (const auto& pair : g_sitePolicies) siteLines.push_back(v4data::Encode(pair.first) + "\t" + (pair.second.scriptsEnabled ? "1" : "0") + "\t" + (pair.second.scriptDialogsEnabled ? "1" : "0"));
    v4data::WriteLines(v4data::Join(g_profileDataDirectory, L"site-policies.pbp"), siteLines);
    std::vector<std::string> permissionLines;
    for (const auto& pair : g_permissionPolicies) permissionLines.push_back(v4data::Encode(pair.first) + "\t" + std::to_string(static_cast<int>(pair.second)));
    v4data::WriteLines(v4data::Join(g_profileDataDirectory, L"permissions.pbp"), permissionLines);
}

void LoadSitePolicies() {
    g_sitePolicies.clear();
    g_permissionPolicies.clear();
    if (g_profileDataDirectory.empty()) return;
    for (const auto& line : v4data::ReadLines(v4data::Join(g_profileDataDirectory, L"site-policies.pbp"))) {
        size_t a = line.find('\t'), b = line.find('\t', a == std::string::npos ? a : a + 1);
        if (a == std::string::npos || b == std::string::npos) continue;
        std::wstring origin = v4data::Decode(line.substr(0, a));
        g_sitePolicies[origin] = {line.substr(a + 1, b - a - 1) != "0", line.substr(b + 1) != "0"};
    }
    for (const auto& line : v4data::ReadLines(v4data::Join(g_profileDataDirectory, L"permissions.pbp"))) {
        size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        try { g_permissionPolicies[v4data::Decode(line.substr(0, tab))] = static_cast<COREWEBVIEW2_PERMISSION_STATE>(std::stoi(line.substr(tab + 1))); } catch (...) {}
    }
}

void SaveQuickNote();
void LoadQuickNote();
void RefreshSettingsPage();

void SavePersistentData() {
    if (g_profileDataDirectory.empty()) LoadProfileContext();
    g_dataDirectory = g_profileDataDirectory;
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
    std::vector<std::string> sessionMetadata;
    for (const auto& tab : g_tabs) if (tab && !tab->privateMode && (tab->lastUri.rfind(L"http://", 0) == 0 || tab->lastUri.rfind(L"https://", 0) == 0)) sessionMetadata.push_back(v4data::Encode(tab->lastUri) + "\t" + v4data::Encode(tab->workspaceId) + "\t" + v4data::Encode(tab->groupId));
    ok = v4data::WriteLines(v4data::Join(g_profileDataDirectory, L"session-meta.pbv"), sessionMetadata) && ok;
    SaveQuickNote();
    SaveSitePolicies();
    if (!ok) OutputDebugStringW(L"Pane Browser: one or more local data files could not be written.\n");
}

void SaveQuickNote() {
    if (!g_profileDataDirectory.empty()) v4data::WriteLines(v4data::Join(g_profileDataDirectory, L"quick-note.pbn"), {v4data::Encode(g_quickNote)});
}

void LoadQuickNote() {
    g_quickNote.clear();
    auto lines = v4data::ReadLines(v4data::Join(g_profileDataDirectory, L"quick-note.pbn"));
    if (!lines.empty()) g_quickNote = v4data::Decode(lines.front());
}

void LoadPersistentData() {
    LoadProfileContext();
    LoadSitePolicies();
    LoadQuickNote();
    v3data::LoadSettings(g_settings, g_dataDirectory);
    g_history.clear();
    std::vector<v3data::HistoryEntry> history;
    if (g_settings.persistHistory) { v3data::LoadHistory(history, g_dataDirectory); for (const auto& e : history) g_history.push_back({e.uri, e.time}); } else { v3data::ClearHistory(g_dataDirectory); }
    v3data::LoadBookmarks(g_bookmarks, g_dataDirectory);
    g_downloads.clear(); std::vector<v3data::DownloadRecord> savedDownloads; v3data::LoadDownloads(savedDownloads, g_dataDirectory); for (const auto& d : savedDownloads) g_downloads.push_back({ComPtr<ICoreWebView2DownloadOperation>(), d, false});
    g_restoreSession.clear(); g_restoreActive = 0; g_restoreTabMetadata.clear();
    if (g_settings.restoreSession) {
        v3data::LoadSession(g_restoreSession, g_restoreActive, g_dataDirectory);
        for (const auto& line : v4data::ReadLines(v4data::Join(g_profileDataDirectory, L"session-meta.pbv"))) { size_t a = line.find('\t'), b = line.find('\t', a == std::string::npos ? a : a + 1); if (a == std::string::npos || b == std::string::npos) continue; g_restoreTabMetadata.push_back({v4data::Decode(line.substr(0, a)), v4data::Decode(line.substr(a + 1, b - a - 1)), v4data::Decode(line.substr(b + 1))}); }
    } else v3data::ClearSession(g_dataDirectory);
}

std::wstring GetWebViewUserDataFolder() {
    std::wstring webviewFolder = v4data::WebViewUserDataFolder();
    v4data::EnsureDirectory(webviewFolder);
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
    std::wstring prefix;
    if (tab.privateMode) prefix += L"Private - ";
    if (tab.sleeping) prefix += L"Sleeping - ";
    if (tab.crashed) prefix += L"Crashed - ";
    if (tab.lastUri.empty() || tab.lastUri == L"about:blank") return prefix.empty() ? L"New Tab" : prefix + L"New Tab";
    std::wstring title = prefix + tab.lastUri;
    if (title.size() > 22) title = title.substr(0, 19) + L"...";
    return title;
}

bool GetTabRect(int index, RECT& rect) {
    if (index < 0 || index >= static_cast<int>(g_tabs.size())) return false;
    if (g_settings.tabsEnabled && g_settings.verticalTabs) {
        rect.left = 4;
        rect.top = 3 + index * 32;
        rect.right = 216;
        rect.bottom = rect.top + 30;
    } else {
        rect.left = index * kTabWidth + 4;
        rect.top = 3;
        rect.right = rect.left + kTabWidth - 8;
        rect.bottom = 30;
    }
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
void ShowHistory();
void ShowHome();
void ShowPrivate();
void ShowNotes();
void ShowSiteData();
void ShowWorkspace();
void DeleteActiveProfile();
void DeleteAllData();
int RunDeferredRootDeletion();
void ToggleReadingMode();
void ToggleSplitView();
void BackupProfile();
void RestoreProfileBackup();
void SetActiveTab(int index);
void ShowDownloads();
void FindInPage();
void UpdateDownloadStates();
void ClearBrowsingData();
void SaveQuickNote();
void LoadQuickNote();
void ReopenClosedTab();
void ExportBookmarks();
void ImportBookmarks();
void CheckForUpdates();
void ShowOverflowMenu();
void ShowNativeContextMenu(POINT point);
void ConfigureProfilePrivacy(BrowserTab* tab);
void HandleManagedNewWindowRequest(BrowserTab* opener, ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args);
void ScheduleWebViewRecovery();
void RecoverAllWebViews();
HRESULT InitializeWebView();
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
        int plusLeft = g_settings.verticalTabs ? 6 : static_cast<int>(g_tabs.size()) * kTabWidth + 6;
        RECT plusRect = g_settings.verticalTabs ? RECT{6, static_cast<int>(g_tabs.size()) * 32 + 2, 42, static_cast<int>(g_tabs.size()) * 32 + 31} : RECT{plusLeft, 2, plusLeft + 36, 31};
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
        int plusLeft = g_settings.verticalTabs ? 6 : static_cast<int>(g_tabs.size()) * kTabWidth + 6;
        if (g_settings.tabsEnabled && ((g_settings.verticalTabs && x >= 0 && x < 48 && y >= static_cast<int>(g_tabs.size()) * 32) || (!g_settings.verticalTabs && x >= plusLeft && x < plusLeft + 42 && y >= 0 && y < 32))) CreateNewTab(L"dashboard", false);
        return 0;
    }
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

void ResumeTab(BrowserTab* tab) {
    if (!tab || !tab->sleeping || !tab->webview) return;
    ComPtr<ICoreWebView2_3> web3;
    if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_3, reinterpret_cast<void**>(web3.GetAddressOf()))) && web3) {
        web3->Resume();
        tab->sleeping = false;
        tab->lastActiveTick = GetTickCount64();
    }
}

void SleepInactiveTabs() {
    if (!g_settings.tabSleeping || g_settings.sleepMinutes < 1) return;
    ULONGLONG now = GetTickCount64();
    ULONGLONG limit = static_cast<ULONGLONG>(g_settings.sleepMinutes) * 60ULL * 1000ULL;
    for (size_t i = 0; i < g_tabs.size(); ++i) {
        BrowserTab* tab = g_tabs[i].get();
        if (!tab || static_cast<int>(i) == g_activeTab || tab->privateMode || tab->authenticationWindow || tab->sleeping || !tab->webview || !tab->lastActiveTick || now - tab->lastActiveTick < limit) continue;
        ComPtr<ICoreWebView2_3> web3;
        if (SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_3, reinterpret_cast<void**>(web3.GetAddressOf()))) && web3) {
            auto suspendCallback = Callback<ICoreWebView2TrySuspendCompletedHandler>([tab](HRESULT errorCode, BOOL result) -> HRESULT {
                if (SUCCEEDED(errorCode) && result) { tab->sleeping = true; InvalidateTabs(); }
                return S_OK;
            });
            web3->TrySuspend(suspendCallback.Get());
            suspendCallback.Get()->Release();
        }
    }
}

void SetActiveTab(int index) {
    if (index < 0 || index >= static_cast<int>(g_tabs.size())) return;
    g_activeTab = index;
    ResumeTab(g_tabs[static_cast<size_t>(index)].get());
    g_tabs[static_cast<size_t>(index)]->lastActiveTick = GetTickCount64();
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
    if (g_splitView) {
        if (index == g_activeTab || index == g_splitSecondTab) { g_splitView = false; g_splitSecondTab = -1; }
        else if (index < g_splitSecondTab) --g_splitSecondTab;
    }
    if (g_tabs[index]->controller) g_tabs[index]->controller->Close();
    g_tabs[index]->webview.Reset();
    g_tabs[index]->controller.Reset();
    g_tabs.erase(g_tabs.begin() + index);
    if (g_activeTab > index) --g_activeTab;
    else if (g_activeTab >= static_cast<int>(g_tabs.size())) g_activeTab = static_cast<int>(g_tabs.size()) - 1;
    SetActiveTab(g_activeTab);
    SavePersistentData();
}

bool SwitchActiveProfile(const std::wstring& id) {
    v4data::ProfileInfo* target = FindProfileInfo(id);
    if (!target || target->kind == L"private" || target->temporary || _wcsicmp(target->id.c_str(), g_activeProfile.id.c_str()) == 0) return false;
    if (!v4data::EnsureProfileLayout(*target)) return false;
    SavePersistentData();
    for (auto& tab : g_tabs) {
        ++tab->generation;
        if (tab->controller) tab->controller->Close();
        if (tab->webview) tab->webview.Reset();
        if (tab->controller) tab->controller.Reset();
    }
    g_tabs.clear();
    g_activeTab = -1;
    g_closedTabs.clear();
    g_activeProfile = *target;
    g_profileDataDirectory = v4data::ProfileDataDirectory(g_activeProfile);
    g_dataDirectory = g_profileDataDirectory;
    v4data::SaveActiveProfile(g_activeProfile.id);
    LoadPersistentData();
    if (g_environment) CreateNewTab(L"dashboard", false);
    return true;
}

bool g_profileDeleteWebDataSucceeded = true;

void FinalizeProfileDeletion() {
    if (!g_profileDeletePending || g_profileDeleteTarget.empty()) return;
    const std::wstring targetId = g_profileDeleteTarget;
    v4data::ProfileInfo* target = FindProfileInfo(targetId);
    if (!target || _wcsicmp(target->id.c_str(), L"personal") == 0) {
        g_profileDeletePending = false;
        g_profileDeleteTarget.clear();
        return;
    }
    const std::wstring targetDataDirectory = v4data::ProfileDataDirectory(*target);
    for (auto& tab : g_tabs) {
        ++tab->generation;
        if (tab->controller) tab->controller->Close();
        if (tab->webview) tab->webview.Reset();
        if (tab->controller) tab->controller.Reset();
    }
    g_tabs.clear();
    g_activeTab = -1;
    g_closedTabs.clear();
    bool appDataRemoved = v4data::DeleteDirectoryTree(targetDataDirectory);
    g_profiles.erase(std::remove_if(g_profiles.begin(), g_profiles.end(), [&](const v4data::ProfileInfo& profile) { return _wcsicmp(profile.id.c_str(), targetId.c_str()) == 0; }), g_profiles.end());
    v4data::SaveProfiles(g_profiles);
    v4data::ProfileInfo* personal = FindProfileInfo(L"personal");
    g_activeProfile = personal ? *personal : v4data::ProfileInfo{L"personal", L"Personal", L"normal", false};
    v4data::EnsureProfileLayout(g_activeProfile);
    g_profileDataDirectory = v4data::ProfileDataDirectory(g_activeProfile);
    g_dataDirectory = g_profileDataDirectory;
    v4data::SaveActiveProfile(g_activeProfile.id);
    LoadPersistentData();
    g_profileDeletePending = false;
    g_profileDeleteTarget.clear();
    if (g_environment) CreateNewTab(L"dashboard", false);
    std::wstring message = appDataRemoved && g_profileDeleteWebDataSucceeded
        ? L"The profile and its local browser data were deleted."
        : L"The profile app data was deleted, but some WebView2 profile data could not be cleared.";
    MessageBoxW(g_main, message.c_str(), L"Pane Browser", MB_OK | ((appDataRemoved && g_profileDeleteWebDataSucceeded) ? MB_ICONINFORMATION : MB_ICONWARNING));
}

void DeleteActiveProfile() {
    if (g_profileDeletePending) return;
    if (_wcsicmp(g_activeProfile.id.c_str(), L"personal") == 0) {
        MessageBoxW(g_main, L"The Personal profile is protected and cannot be deleted.", L"Pane Browser", MB_OK | MB_ICONINFORMATION);
        return;
    }
    if (MessageBoxW(g_main, L"Delete the active profile and its local History, Bookmarks, Downloads, Notes, Settings, session data, permissions, and WebView2 site data? This cannot be undone.", L"Delete Profile", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) != IDYES) return;
    BrowserTab* tab = ActiveTab();
    ComPtr<ICoreWebView2_13> profileWeb;
    ComPtr<ICoreWebView2Profile> profile;
    ComPtr<ICoreWebView2Profile8> profile8;
    if (!tab || !tab->webview || FAILED(tab->webview->QueryInterface(IID_ICoreWebView2_13, reinterpret_cast<void**>(profileWeb.GetAddressOf()))) || !profileWeb || FAILED(profileWeb->get_Profile(&profile)) || !profile || FAILED(profile->QueryInterface(IID_ICoreWebView2Profile8, reinterpret_cast<void**>(profile8.GetAddressOf()))) || !profile8) {
        MessageBoxW(g_main, L"This WebView2 Runtime does not support safe profile deletion. Nothing was deleted.", L"Delete Profile", MB_OK | MB_ICONWARNING);
        return;
    }
    g_profileDeleteTarget = g_activeProfile.id;
    g_profileDeletePending = true;
    g_profileDeleteWebDataSucceeded = false;
    for (auto& openTab : g_tabs) {
        ++openTab->generation;
        if (openTab->controller) openTab->controller->Close();
        if (openTab->webview) openTab->webview.Reset();
        if (openTab->controller) openTab->controller.Reset();
    }
    g_tabs.clear();
    g_activeTab = -1;
    g_closedTabs.clear();
    HRESULT hr = profile8->Delete();
    profile8.Reset();
    profile.Reset();
    profileWeb.Reset();
    if (FAILED(hr)) {
        g_profileDeletePending = false;
        g_profileDeleteTarget.clear();
        if (g_environment) CreateNewTab(L"dashboard", false);
        MessageBoxW(g_main, L"WebView2 could not delete this profile because it is still in use or unavailable. Nothing was removed from Pane Browser data.", L"Delete Profile", MB_OK | MB_ICONWARNING);
        return;
    }
    g_profileDeleteWebDataSucceeded = true;
    FinalizeProfileDeletion();
}

bool IsApprovedPaneDataRoot(const std::wstring& root) {
    if (root.empty()) return false;
    wchar_t local[MAX_PATH] = {};
    std::wstring localRoot;
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, local)) && local[0]) localRoot = v4data::Join(local, L"Pane Browser");
    std::wstring fallbackRoot = v4data::Join(v3data::ModuleDirectory(), L"PaneBrowser.Data");
    return (!localRoot.empty() && _wcsicmp(root.c_str(), localRoot.c_str()) == 0) || _wcsicmp(root.c_str(), fallbackRoot.c_str()) == 0;
}

int RunDeferredRootDeletion() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv) return 2;
    bool valid = argc == 4 && _wcsicmp(argv[1], L"--pane-delete-all") == 0;
    DWORD parentPid = 0;
    std::wstring root;
    if (valid) {
        try { parentPid = static_cast<DWORD>(std::stoul(argv[2])); } catch (...) { valid = false; }
        root = argv[3];
        valid = valid && parentPid != 0 && IsApprovedPaneDataRoot(root);
    }
    if (valid) {
        HANDLE parent = OpenProcess(SYNCHRONIZE, FALSE, parentPid);
        if (parent) { WaitForSingleObject(parent, INFINITE); CloseHandle(parent); valid = v4data::DeleteDirectoryTree(root); }
        else valid = false;
    }
    LocalFree(argv);
    return valid ? 0 : 2;
}

bool LaunchDeferredRootDeletion() {
    std::wstring root = v4data::BaseDirectory();
    if (!IsApprovedPaneDataRoot(root)) return false;
    wchar_t modulePath[32768] = {};
    DWORD length = GetModuleFileNameW(nullptr, modulePath, _countof(modulePath));
    if (!length || length >= _countof(modulePath)) return false;
    std::wstring command = std::wstring(L"\"") + std::wstring(modulePath, length) + L"\" --pane-delete-all " + std::to_wstring(GetCurrentProcessId()) + L" \"" + root + L"\"";
    std::vector<wchar_t> commandBuffer(command.begin(), command.end());
    commandBuffer.push_back(L'\0');
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    PROCESS_INFORMATION process{};
    BOOL launched = CreateProcessW(modulePath, commandBuffer.data(), nullptr, nullptr, FALSE, CREATE_NO_WINDOW, nullptr, nullptr, &startup, &process);
    if (!launched) return false;
    CloseHandle(process.hThread);
    CloseHandle(process.hProcess);
    return true;
}

void DeleteAllData() {
    if (g_deleteAllInProgress) return;
    std::wstring root = v4data::BaseDirectory();
    if (!IsApprovedPaneDataRoot(root)) {
        MessageBoxW(g_main, L"Pane Browser could not verify its local data folder. Nothing was deleted.", L"Delete All Data", MB_OK | MB_ICONWARNING);
        return;
    }
    std::wstring warning = L"Permanently delete ALL Pane Browser data?\\n\\nThis removes the entire folder:\\n" + root + L"\\n\\nIt includes every profile, History, Bookmarks, Downloads records, Settings, sessions, permissions, cookies, cache, WebView2 website data, and local sign-in state. This cannot be recovered, restored, or backed up by Pane Browser. Files saved outside this folder, such as downloads in a custom external directory, are not deleted. Pane Browser will close immediately.";
    if (MessageBoxW(g_main, warning.c_str(), L"Delete All Data", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) != IDYES) return;
    if (MessageBoxW(g_main, L"Final confirmation: permanently erase the entire Pane Browser data folder now? There is no undo and no recovery inside Pane Browser.", L"Delete All Data", MB_YESNO | MB_ICONWARNING | MB_DEFBUTTON2) != IDYES) return;
    if (!LaunchDeferredRootDeletion()) {
        MessageBoxW(g_main, L"Pane Browser could not start the safe cleanup process. Nothing was deleted.", L"Delete All Data", MB_OK | MB_ICONERROR);
        return;
    }
    g_deleteAllInProgress = true;
    for (auto& tab : g_tabs) {
        ++tab->generation;
        if (tab->controller) tab->controller->Close();
        if (tab->webview) tab->webview.Reset();
        if (tab->controller) tab->controller.Reset();
    }
    g_tabs.clear();
    g_activeTab = -1;
    g_closedTabs.clear();
    g_downloads.clear();
    g_environment.Reset();
    DestroyWindow(g_main);
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
    tab->commandPalettePage = false;
    tab->notesPage = false;
    tab->siteDataPage = false;
    tab->workspacePage = false;
}

bool IsAuthenticationUri(const std::wstring& uri) {
    std::wstring value = uri;
    for (wchar_t& c : value) c = static_cast<wchar_t>(towlower(c));
    const wchar_t* markers[] = {L"/oauth", L"/authorize", L"/authorization", L"/signin", L"/login", L"/saml", L"/oidc", L"/auth/", L"accounts.google.", L"login.microsoftonline."};
    for (const wchar_t* marker : markers) if (value.find(marker) != std::wstring::npos) return true;
    return false;
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
    ApplySitePolicy(tab);
    if (tab->authenticationWindow) {
        if (tab == ActiveTab()) {
            SetWindowTextW(g_address, L"Authentication window");
            SetTabLabel(tab);
        }
        return;
    }
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

std::wstring NotesHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    return L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:900px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px}h1{margin-top:0}textarea{width:100%;height:360px;box-sizing:border-box;padding:14px;font:16px Segoe UI;border-radius:8px;background:" + bg + L";color:" + text + L";border:1px solid #777}button{padding:9px 13px;margin-top:12px;border:0;border-radius:6px;cursor:pointer}</style></head><body><main class='wrap'><section class='card'><h1>Quick Notes</h1><p>Notes are stored locally in the active profile and are not uploaded by Pane Browser.</p><textarea id='note'>" + HtmlEscape(g_quickNote) + L"</textarea><br><button onclick=\"send('notes-save:'+document.getElementById('note').value)\">Save note</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
}

std::wstring SiteDataHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    BrowserTab* tab = ActiveTab();
    std::wstring origin = tab ? OriginFromUri(tab->lastUri) : L"";
    SitePolicy policy = GetSitePolicy(origin);
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:900px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px}h1{margin-top:0}.row{display:flex;justify-content:space-between;align-items:center;padding:12px 0;border-bottom:1px solid #777}button{padding:9px 13px;margin:6px 6px 0 0;border:0;border-radius:6px;cursor:pointer}</style></head><body><main class='wrap'><section class='card'><h1>Site Data Center</h1><p><strong>Profile:</strong> " + HtmlEscape(g_activeProfile.name) + L"</p><p><strong>Current origin:</strong> " + HtmlEscape(origin.empty() ? L"No web origin" : origin) + L"</p><p>Cookies, DOM storage, cache, permissions, and other web data belong to the active WebView2 profile. Cookie values and authentication tokens are never displayed or exported by this page.</p>";
    if (!origin.empty() && tab && !tab->privateMode) {
        html += std::wstring(L"<h2>Per-site controls</h2><div class='row'><span>JavaScript</span><span><button onclick=\"send('site-scripts:1')\">Allow</button><button onclick=\"send('site-scripts:0')\">Block</button> — currently ") + (policy.scriptsEnabled ? L"allowed" : L"blocked") + L"</span></div><div class='row'><span>Script dialogs</span><span><button onclick=\"send('site-dialogs:1')\">Allow</button><button onclick=\"send('site-dialogs:0')\">Block</button> — currently " + (policy.scriptDialogsEnabled ? L"allowed" : L"blocked") + L"</span></div><h2>Permission policy</h2>";
        const COREWEBVIEW2_PERMISSION_KIND kinds[] = {COREWEBVIEW2_PERMISSION_KIND_CAMERA, COREWEBVIEW2_PERMISSION_KIND_MICROPHONE, COREWEBVIEW2_PERMISSION_KIND_GEOLOCATION, COREWEBVIEW2_PERMISSION_KIND_NOTIFICATIONS, COREWEBVIEW2_PERMISSION_KIND_CLIPBOARD_READ};
        for (auto kind : kinds) {
            std::wstring key = PermissionPolicyKey(origin, kind); auto it = g_permissionPolicies.find(key); std::wstring state = it == g_permissionPolicies.end() ? L"default" : (it->second == COREWEBVIEW2_PERMISSION_STATE_ALLOW ? L"allowed" : L"blocked");
            html += L"<div class='row'><span>" + PermissionKindLabel(kind) + L"</span><span><button onclick=\"send('permission:" + std::to_wstring(static_cast<int>(kind)) + L":1')\">Allow</button><button onclick=\"send('permission:" + std::to_wstring(static_cast<int>(kind)) + L":2')\">Block</button> — currently " + state + L"</span></div>";
        }
    } else html += L"<p>Open a normal web page to configure per-site policies. Private tabs do not persist site policies.</p>";
    html += L"<h2>Data actions</h2><button onclick=\"send('clear-data')\">Clear WebView2 site data and app records</button><button onclick=\"send('settings')\">Open profile settings</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
    return html;
}

std::wstring WorkspaceHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:1050px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px}h1{margin-top:0}.row{padding:10px 0;border-bottom:1px solid #777}button{padding:9px 13px;margin:6px 6px 0 0;border:0;border-radius:6px;cursor:pointer}</style></head><body><main class='wrap'><section class='card'><h1>Workspace</h1><input id='tabSearch' aria-label='Search tabs' placeholder='Search tabs' oninput=\"const q=this.value.toLowerCase();document.querySelectorAll('.row').forEach(r=>r.style.display=r.textContent.toLowerCase().includes(q)?'block':'none')\" style='box-sizing:border-box;width:100%;padding:11px;margin:0 0 12px;border-radius:8px;border:1px solid #777;font:inherit'><p>Active profile: <strong>" + HtmlEscape(g_activeProfile.name) + L"</strong>. Normal tabs can be grouped and restored; private tabs are excluded.</p>";
    for (size_t i = 0; i < g_tabs.size(); ++i) {
        const auto& tab = g_tabs[i];
        html += L"<div class='row'><strong>" + HtmlEscape(TabTitle(*tab)) + L"</strong> — workspace " + HtmlEscape(tab->workspaceId) + L" — group " + HtmlEscape(tab->groupId.empty() ? L"Ungrouped" : tab->groupId) + (tab->sleeping ? L" — Sleeping" : L"") + L"<br><button onclick=\"send('tab-group:" + std::to_wstring(i) + L":Research')\">Put in Research group</button><button onclick=\"send('tab-group:" + std::to_wstring(i) + L":Personal')\">Put in Personal group</button></div>";
    }
    html += L"<button onclick=\"send('workspace-new-tab')\">New workspace tab</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
    return html;
}

void ShowNotes() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->notesPage = true; tab->historyPage = tab->bookmarksPage = tab->settingsPage = tab->downloadsPage = tab->commandPalettePage = tab->siteDataPage = tab->workspacePage = false; SetWindowTextW(g_address, L"Quick Notes"); tab->webview->NavigateToString(NotesHtml().c_str()); }
void ShowSiteData() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->siteDataPage = true; tab->historyPage = tab->bookmarksPage = tab->settingsPage = tab->downloadsPage = tab->commandPalettePage = tab->notesPage = tab->workspacePage = false; SetWindowTextW(g_address, L"Site Data Center"); tab->webview->NavigateToString(SiteDataHtml().c_str()); }
void ShowWorkspace() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->workspacePage = true; tab->historyPage = tab->bookmarksPage = tab->settingsPage = tab->downloadsPage = tab->commandPalettePage = tab->notesPage = tab->siteDataPage = false; SetWindowTextW(g_address, L"Workspace"); tab->webview->NavigateToString(WorkspaceHtml().c_str()); }

void ToggleReadingMode() {
    BrowserTab* tab = ActiveTab();
    if (!tab || !tab->webview || tab->internalPage || tab->privateMode) return;
    tab->readingMode = !tab->readingMode;
    const wchar_t* script = tab->readingMode ? LR"JS((()=>{document.documentElement.dataset.paneReading='1';document.body.style.maxWidth='860px';document.body.style.margin='0 auto';document.body.style.fontSize='1.12em';document.body.style.lineHeight='1.65';})())JS" : LR"JS((()=>{document.documentElement.dataset.paneReading='0';document.body.style.maxWidth='';document.body.style.margin='';document.body.style.fontSize='';document.body.style.lineHeight='';})())JS";
    auto cb = Callback<ICoreWebView2ExecuteScriptCompletedHandler>([](HRESULT, LPCWSTR) { return S_OK; });
    tab->webview->ExecuteScript(script, cb.Get()); cb.Get()->Release();
}

void ToggleSplitView() {
    if (!g_settings.tabsEnabled) return;
    if (g_splitView) { g_splitView = false; g_splitSecondTab = -1; ResizeChildren(); return; }
    if (g_tabs.size() < 2) { CreateNewTab(L"dashboard", false); }
    if (g_tabs.size() >= 2) { g_splitView = true; g_splitSecondTab = (g_activeTab + 1) % static_cast<int>(g_tabs.size()); ResizeChildren(); }
}

void BackupProfile() {
    BROWSEINFOW bi{}; bi.hwndOwner = g_main; bi.lpszTitle = L"Choose a folder for the local Pane Browser backup"; bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi); if (!pidl) return;
    wchar_t selected[MAX_PATH] = {}; bool ok = SHGetPathFromIDListW(pidl, selected) != FALSE; CoTaskMemFree(pidl); if (!ok) return;
    std::wstring destination = v4data::Join(selected, L"PaneBrowser-v4-backup");
    if (!v4data::EnsureDirectory(destination)) return;
    const wchar_t* files[] = {L"history.pbh", L"bookmarks.pbb", L"settings.pbs", L"session.pbs", L"session-meta.pbv", L"downloads.pbd", L"quick-note.pbn"};
    bool copied = true;
    for (const wchar_t* file : files) { std::wstring source = v4data::Join(g_dataDirectory, file); std::wstring target = v4data::Join(destination, file); if (GetFileAttributesW(source.c_str()) != INVALID_FILE_ATTRIBUTES && !CopyFileW(source.c_str(), target.c_str(), FALSE)) copied = false; }
    MessageBoxW(g_main, copied ? L"App-owned profile data was backed up. Cookies, tokens, and WebView2 UDF data were not exported." : L"Some profile files could not be backed up.", L"Pane Browser", MB_OK | (copied ? MB_ICONINFORMATION : MB_ICONWARNING));
}

void RestoreProfileBackup() {
    BROWSEINFOW bi{}; bi.hwndOwner = g_main; bi.lpszTitle = L"Choose a Pane Browser app-data backup folder"; bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
    PIDLIST_ABSOLUTE pidl = SHBrowseForFolderW(&bi); if (!pidl) return;
    wchar_t selected[MAX_PATH] = {}; bool ok = SHGetPathFromIDListW(pidl, selected) != FALSE; CoTaskMemFree(pidl); if (!ok) return;
    if (MessageBoxW(g_main, L"Restore app-owned History, Bookmarks, Settings, Session, Downloads, and Notes into the active profile? WebView2 cookies and login data will not be imported.", L"Pane Browser", MB_YESNO | MB_ICONWARNING) != IDYES) return;
    const wchar_t* files[] = {L"history.pbh", L"bookmarks.pbb", L"settings.pbs", L"session.pbs", L"session-meta.pbv", L"downloads.pbd", L"quick-note.pbn"};
    for (const wchar_t* file : files) { std::wstring source = v4data::Join(selected, file); std::wstring target = v4data::Join(g_dataDirectory, file); if (GetFileAttributesW(source.c_str()) != INVALID_FILE_ATTRIBUTES) CopyFileW(source.c_str(), target.c_str(), FALSE); }
    LoadPersistentData();
    RefreshSettingsPage();
}

std::wstring ProfileOptionsHtml() {
    std::wstring html;
    for (const auto& profile : g_profiles) {
        if (profile.kind == L"private" || profile.temporary) continue;
        html += L"<option value='" + HtmlEscape(profile.id) + L"'" + (_wcsicmp(profile.id.c_str(), g_activeProfile.id.c_str()) == 0 ? L" selected" : L"") + L">" + HtmlEscape(profile.name) + L"</option>";
    }
    return html;
}

std::wstring SettingsHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:900px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px;box-shadow:0 4px 24px rgba(0,0,0,.15)}h1{font-size:34px;margin:0 0 24px}.setting{display:flex;justify-content:space-between;align-items:center;padding:17px 0;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}select,input{font:inherit;padding:8px;border-radius:6px}button{font:inherit;padding:9px 13px;margin:10px 6px 0 0;border:0;border-radius:6px;cursor:pointer}.danger{background:#b3261e;color:#fff}.shortcuts{width:100%;border-collapse:collapse;margin-top:12px}.shortcuts th,.shortcuts td{text-align:left;padding:7px;border-bottom:1px solid " + std::wstring(g_darkMode ? L"#3c4043" : L"#e8eaed") + L"}</style></head><body><main class='wrap'><section class='card'><h1>Settings</h1>";
    html += L"<div class='setting'><span>Active profile</span><select onchange=\"send('profile-switch:'+this.value)\">" + ProfileOptionsHtml() + L"</select></div><div class='setting'><span>Create profile</span><input id='newProfile' style='width:40%' placeholder='Work or School'><button onclick=\"send('profile-create:'+document.getElementById('newProfile').value)\">Create</button><button onclick=\"send('guest-session')\">Open Guest Session</button></div><div class='setting'><span>Delete profile</span>" + (_wcsicmp(g_activeProfile.id.c_str(), L"personal") == 0 ? L"<button disabled title='The Personal profile is protected'>Personal profile is protected</button>" : L"<button onclick=\"send('profile-delete:" + HtmlEscape(g_activeProfile.id) + L"')\">Delete Profile</button>") + L"</div><div class='setting'><span>Search engine</span><select onchange=\"send('settings-search-engine:'+this.value)\">" + SearchEngineOptionsHtml() + L"</select></div><div class='setting'><span>Custom search URL</span><input style='width:52%' value='" + HtmlEscape(g_settings.customSearchUrl) + L"' placeholder='https://example.com/search?q={query}' onchange=\"send('settings-custom-search:'+this.value)\"></div>";
    html += std::wstring(L"<div class='setting'><span>Theme</span><select onchange=\"send('settings-theme:'+this.value)\"><option value='system'") + (g_settings.theme == L"system" ? L" selected" : L"") + L">System</option><option value='light'" + (g_settings.theme == L"light" ? L" selected" : L"") + L">Light</option><option value='dark'" + (g_settings.theme == L"dark" ? L" selected" : L"") + L">Dark</option></select></div>";
    html += std::wstring(L"<div class='setting'><span>Enable tabs</span><input type='checkbox' ") + (g_settings.tabsEnabled ? L"checked" : L"") + L" onchange=\"send('settings-tabs:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Persistent history</span><input type='checkbox' ") + (g_settings.persistHistory ? L"checked" : L"") + L" onchange=\"send('settings-history:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Restore previous session</span><input type='checkbox' ") + (g_settings.restoreSession ? L"checked" : L"") + L" onchange=\"send('settings-restore:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Check for updates on startup</span><input type='checkbox' ") + (g_settings.checkUpdates ? L"checked" : L"") + L" onchange=\"send('settings-updates:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Custom title bar</span><input type='checkbox' ") + (g_settings.customTitleBar ? L"checked" : L"") + L" onchange=\"send('settings-titlebar:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Vertical tabs</span><input type='checkbox' ") + (g_settings.verticalTabs ? L"checked" : L"") + L" onchange=\"send('settings-vertical-tabs:'+(this.checked?'1':'0'))\"></div>";
    html += std::wstring(L"<div class='setting'><span>Tab sleeping</span><input type='checkbox' ") + (g_settings.tabSleeping ? L"checked" : L"") + L" onchange=\"send('settings-sleeping:'+(this.checked?'1':'0'))\"></div>";
    html += L"<div class='setting'><span>Sleep after minutes</span><input type='number' min='1' max='120' value='" + std::to_wstring(g_settings.sleepMinutes) + L"' onchange=\"send('settings-sleep-minutes:'+this.value)\"></div>";
    html += std::wstring(L"<div class='setting'><span>Privacy mode</span><select onchange=\"send('settings-privacy:'+this.value)\"><option value='balanced'") + (g_settings.privacyMode == L"balanced" ? L" selected" : L"") + L">Balanced</option><option value='strict'" + (g_settings.privacyMode == L"strict" ? L" selected" : L"") + L">Strict</option><option value='custom'" + (g_settings.privacyMode == L"custom" ? L" selected" : L"") + L">Custom</option></select></div>";
    html += L"<h2>Privacy and site data</h2><p>Active profile: <strong>" + HtmlEscape(g_activeProfile.name) + L"</strong>. Normal cookies and site data are kept locally so trusted websites can remember your sign-in. Private tabs use an isolated profile, are excluded from History and session restore, and their download records are not persisted. Pane Browser does not upload its own History to a Pane Browser server.</p><p>Authentication compatibility is provider-dependent. Redirect and popup flows are preserved where WebView2 and the provider allow embedded authentication; some providers may require a system-browser flow.</p>";
    html += LR"HTML(<h2>Keyboard shortcuts</h2><table class='shortcuts'><tr><th>Shortcut</th><th>Action</th></tr><tr><td>Ctrl+L</td><td>Focus address bar</td></tr><tr><td>Ctrl+T</td><td>Open a new tab</td></tr><tr><td>Ctrl+W</td><td>Close the active tab</td></tr><tr><td>Ctrl+H</td><td>Open History</td></tr><tr><td>Ctrl+Shift+T</td><td>Restore the most recently closed normal tab</td></tr><tr><td>Ctrl+Tab</td><td>Next tab</td></tr><tr><td>Ctrl+Shift+Tab</td><td>Previous tab</td></tr><tr><td>Ctrl+R / F5</td><td>Reload page</td></tr><tr><td>Ctrl+F</td><td>Find in page</td></tr><tr><td>Ctrl+D</td><td>Add bookmark</td></tr><tr><td>Ctrl+J</td><td>Downloads</td></tr><tr><td>Ctrl+Shift+P</td><td>Open private tab</td></tr><tr><td>Ctrl++ / Ctrl+-</td><td>Zoom in / out</td></tr><tr><td>Ctrl+0</td><td>Reset zoom</td></tr></table>)HTML";
    html += L"<button onclick=\"send('choose-download-folder')\">Choose download folder</button><button onclick=\"send('clear-data')\">Clear browsing data</button><button onclick=\"send('bookmarks-export')\">Export bookmarks</button><button onclick=\"send('bookmarks-import')\">Import bookmarks</button><button onclick=\"send('check-updates')\">Check for updates</button><button class='danger' onclick=\"send('delete-all-data')\">Delete All Data</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
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
function visibleRows(){return rows().filter(r=>r.style.display!=='none');}
function syncSelection(){const picks=document.querySelectorAll('.pick:checked');const selection=document.getElementById('selection');document.getElementById('count').textContent=picks.length+' selected';selection.classList.toggle('show',picks.length>0);selection.classList.toggle('has-selection',picks.length>0);}
function selectAll(){visibleRows().forEach(r=>{const pick=r.querySelector('.pick');if(pick)pick.checked=true;});syncSelection();}
function deselectAll(){visibleRows().forEach(r=>{const pick=r.querySelector('.pick');if(pick)pick.checked=false;});syncSelection();}
function filterRows(){const q=document.getElementById('search').value.toLowerCase();rows().forEach(r=>{const visible=r.dataset.search.toLowerCase().includes(q);r.style.display=visible?'grid':'none';if(!visible){const pick=r.querySelector('.pick');if(pick)pick.checked=false;}});syncSelection();}
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

std::wstring CommandPaletteHtml() {
    const wchar_t* bg = g_darkMode ? L"#202124" : L"#f8f9fa";
    const wchar_t* card = g_darkMode ? L"#292a2d" : L"#ffffff";
    const wchar_t* text = g_darkMode ? L"#f1f3f4" : L"#202124";
    std::wstring html = L"<!doctype html><html><head><meta charset='utf-8'><style>body{margin:0;background:" + std::wstring(bg) + L";color:" + text + L";font:16px Segoe UI,system-ui,sans-serif}.wrap{max-width:760px;margin:auto;padding:36px}.card{background:" + card + L";padding:28px;border-radius:16px;box-shadow:0 4px 24px rgba(0,0,0,.15)}h1{margin:0 0 18px;font-size:32px}button{display:block;width:100%;text-align:left;padding:12px 14px;margin:7px 0;border:0;border-radius:8px;cursor:pointer;background:" + std::wstring(g_darkMode ? L"#3c4043" : L"#f1f3f4") + L";color:" + text + L"}</style></head><body><main class='wrap'><section class='card'><h1>Command Palette</h1><input id='q' aria-label='Filter commands' placeholder='Filter commands' oninput=\"const q=this.value.toLowerCase();document.querySelectorAll('button').forEach(b=>b.style.display=b.textContent.toLowerCase().includes(q)?'block':'none')\" style='box-sizing:border-box;width:100%;padding:11px;margin:0 0 10px;border-radius:8px;border:1px solid #777;font:inherit'><button onclick=\"send('palette-new-tab')\">New tab</button><button onclick=\"send('palette-history')\">History</button><button onclick=\"send('palette-bookmarks')\">Bookmarks</button><button onclick=\"send('palette-downloads')\">Downloads</button><button onclick=\"send('palette-settings')\">Settings and profiles</button><button onclick=\"send('palette-private')\">New private tab</button><button onclick=\"send('palette-recover')\">Recover WebView tabs</button><button onclick=\"send('palette-notes')\">Quick Notes</button><button onclick=\"send('palette-site-data')\">Site Data Center</button><button onclick=\"send('palette-workspace')\">Workspace and tab groups</button><button onclick=\"send('palette-reading')\">Toggle Reading Mode</button><button onclick=\"send('palette-split')\">Toggle Split View</button><button onclick=\"send('palette-backup')\">Backup profile data</button><button onclick=\"send('palette-restore')\">Restore profile data</button><button onclick=\"send('palette-home')\">Home</button></section></main><script>const send=m=>window.chrome&&chrome.webview&&chrome.webview.postMessage(m)</script></body></html>";
    return html;
}

void ShowCommandPalette() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->historyPage = tab->bookmarksPage = tab->settingsPage = tab->downloadsPage = false; tab->commandPalettePage = true; SetWindowTextW(g_address, L"Command Palette"); tab->webview->NavigateToString(CommandPaletteHtml().c_str()); }

void RefreshBookmarkPage() { BrowserTab* tab = ActiveTab(); if (!tab || !tab->webview) return; tab->internalPage = true; tab->historyPage = false; tab->bookmarksPage = true; tab->settingsPage = false; tab->commandPalettePage = false; tab->webview->NavigateToString(BookmarksHtml().c_str()); }
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
    if (!tab || !args || !(tab->internalPage || tab->historyPage || tab->bookmarksPage || tab->settingsPage || tab->downloadsPage || tab->commandPalettePage || tab->notesPage || tab->siteDataPage || tab->workspacePage)) return false;
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
    if (message == L"palette-new-tab") { if (g_settings.tabsEnabled) CreateNewTab(L"dashboard", false); return; }
    if (message == L"palette-history") { ShowHistory(); return; }
    if (message == L"palette-bookmarks") { ShowBookmarks(); return; }
    if (message == L"palette-downloads") { ShowDownloads(); return; }
    if (message == L"palette-settings") { ShowSettings(); return; }
    if (message == L"palette-private") { ShowPrivate(); return; }
    if (message == L"palette-recover") { ScheduleWebViewRecovery(); return; }
    if (message == L"palette-notes") { ShowNotes(); return; }
    if (message == L"palette-site-data") { ShowSiteData(); return; }
    if (message == L"palette-workspace") { ShowWorkspace(); return; }
    if (message == L"palette-reading") { ToggleReadingMode(); return; }
    if (message == L"palette-split") { ToggleSplitView(); return; }
    if (message == L"palette-backup") { BackupProfile(); return; }
    if (message == L"palette-restore") { RestoreProfileBackup(); return; }
    if (message.rfind(L"notes-save:", 0) == 0) { g_quickNote = message.substr(11); SaveQuickNote(); ShowNotes(); return; }
    if (message == L"site-data") { ShowSiteData(); return; }
    if (message.rfind(L"site-scripts:", 0) == 0 || message.rfind(L"site-dialogs:", 0) == 0) {
        std::wstring origin = tab ? OriginFromUri(tab->lastUri) : L"";
        if (!origin.empty() && tab && !tab->privateMode) {
            SitePolicy& policy = g_sitePolicies[origin];
            bool enabled = message.substr(message.find(L':') + 1) != L"0";
            if (message.rfind(L"site-scripts:", 0) == 0) policy.scriptsEnabled = enabled; else policy.scriptDialogsEnabled = enabled;
            ApplySitePolicy(tab); SavePersistentData();
        }
        ShowSiteData(); return;
    }
    if (message.rfind(L"permission:", 0) == 0) {
        std::wstring origin = tab ? OriginFromUri(tab->lastUri) : L"";
        constexpr size_t permissionPrefixLength = 11; // "permission:"
        size_t first = message.find(L':', permissionPrefixLength), second = message.find(L':', first == std::wstring::npos ? first : first + 1);
        if (!origin.empty() && first != std::wstring::npos && second != std::wstring::npos && tab && !tab->privateMode) {
            try { int kindValue = std::stoi(message.substr(permissionPrefixLength, first - permissionPrefixLength)); int stateValue = std::stoi(message.substr(second + 1)); if (stateValue == 1 || stateValue == 2) { g_permissionPolicies[PermissionPolicyKey(origin, static_cast<COREWEBVIEW2_PERMISSION_KIND>(kindValue))] = stateValue == 1 ? COREWEBVIEW2_PERMISSION_STATE_ALLOW : COREWEBVIEW2_PERMISSION_STATE_DENY; SavePersistentData(); } } catch (...) {}
        }
        ShowSiteData(); return;
    }
    if (message == L"guest-session") { ShowPrivate(); return; }
    if (message == L"settings") { ShowSettings(); return; }
    if (message == L"workspace-new-tab") { if (g_settings.tabsEnabled) CreateNewTab(L"dashboard", false); return; }
    if (message.rfind(L"tab-group:", 0) == 0) { constexpr size_t prefix = 10; size_t first = message.find(L':', prefix); if (first != std::wstring::npos) { try { int index = std::stoi(message.substr(prefix, first - prefix)); if (index >= 0 && index < static_cast<int>(g_tabs.size()) && !g_tabs[static_cast<size_t>(index)]->privateMode) { g_tabs[static_cast<size_t>(index)]->groupId = message.substr(first + 1); SavePersistentData(); } } catch (...) {} } ShowWorkspace(); return; }
    if (message == L"palette-home") { ShowHome(); return; }
    if (message.rfind(L"profile-switch:", 0) == 0) {
        std::wstring target = message.substr(15);
        if (v4data::IsSafeProfileId(target) && SwitchActiveProfile(target)) RefreshSettingsPage();
        return;
    }
    if (message.rfind(L"profile-delete:", 0) == 0) {
        std::wstring target = message.substr(15);
        if (v4data::IsSafeProfileId(target) && _wcsicmp(target.c_str(), g_activeProfile.id.c_str()) == 0 && _wcsicmp(target.c_str(), L"personal") != 0) DeleteActiveProfile();
        return;
    }
    if (message == L"delete-all-data") { DeleteAllData(); return; }
    if (message.rfind(L"profile-create:", 0) == 0) {
        std::wstring name = message.substr(15);
        if (name.size() > 48) name.resize(48);
        std::wstring id = v4data::SanitizeId(name);
        if (!name.empty() && _wcsicmp(id.c_str(), L"personal") != 0 && !FindProfileInfo(id)) {
            v4data::ProfileInfo created{id, name, L"normal", false};
            if (v4data::EnsureProfileLayout(created)) { g_profiles.push_back(created); v4data::SaveProfiles(g_profiles); }
        }
        RefreshSettingsPage();
        return;
    }
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
    if (message.rfind(L"settings-restore:",0)==0) { g_settings.restoreSession=message.substr(17)!=L"0"; if (!g_settings.restoreSession) v3data::ClearSession(g_dataDirectory); else SavePersistentData(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-updates:",0)==0) { g_settings.checkUpdates=message.substr(17)!=L"0"; SavePersistentData(); return; }
    if (message.rfind(L"settings-titlebar:", 0) == 0) {
        g_settings.customTitleBar = message.substr(18) != L"0"; SavePersistentData(); ApplyCustomTitleBar(); RefreshSettingsPage(); return;
    }
    if (message.rfind(L"settings-vertical-tabs:", 0) == 0) { g_settings.verticalTabs = message.substr(23) != L"0"; SavePersistentData(); ResizeChildren(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-sleeping:", 0) == 0) { g_settings.tabSleeping = message.substr(18) != L"0"; SavePersistentData(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-sleep-minutes:", 0) == 0) { try { g_settings.sleepMinutes = std::max(1, std::min(120, std::stoi(message.substr(22)))); } catch (...) {} SavePersistentData(); RefreshSettingsPage(); return; }
    if (message.rfind(L"settings-privacy:", 0) == 0) { g_settings.privacyMode = message.substr(17); if (g_settings.privacyMode != L"balanced" && g_settings.privacyMode != L"strict" && g_settings.privacyMode != L"custom") g_settings.privacyMode = L"balanced"; SavePersistentData(); for (auto& item : g_tabs) ConfigureProfilePrivacy(item.get()); RefreshSettingsPage(); return; }

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

std::wstring SmartResolveInput(const std::wstring& raw) {
    size_t first = raw.find_first_not_of(L" \t\r\n"), last = raw.find_last_not_of(L" \t\r\n");
    if (first == std::wstring::npos) return L"";
    std::wstring value = raw.substr(first, last - first + 1);
    std::wstring lowered = value;
    for (wchar_t& c : lowered) c = static_cast<wchar_t>(towlower(c));
    for (const auto& bookmark : g_bookmarks) {
        std::wstring title = bookmark.title, uri = bookmark.uri;
        for (wchar_t& c : title) c = static_cast<wchar_t>(towlower(c));
        for (wchar_t& c : uri) c = static_cast<wchar_t>(towlower(c));
        if (title == lowered || uri == lowered || OriginFromUri(bookmark.uri) == lowered) return bookmark.uri;
    }
    for (auto it = g_history.rbegin(); it != g_history.rend(); ++it) {
        if (HistoryHost(it->uri) == value || it->uri == value) return it->uri;
    }
    return NormalizeInput(value);
}

void NavigateFromAddressBar() {
    BrowserTab* tab = ActiveTab();
    if (!tab || !tab->webview || !g_address) return;
    wchar_t input[0x800] = {};
    GetWindowTextW(g_address, input, static_cast<int>(sizeof(input) / sizeof(input[0])));
    std::wstring uri = SmartResolveInput(input);
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
    const bool vertical = g_settings.tabsEnabled && g_settings.verticalTabs;
    const int tabHeight = g_settings.tabsEnabled && !vertical ? 32 : 0;
    const int tabWidth = vertical ? 220 : 0;
    ShowWindow(g_tabStrip, g_settings.tabsEnabled ? SW_SHOW : SW_HIDE);
    ShowWindow(g_historyButton, SW_HIDE); ShowWindow(g_bookmarksButton, SW_HIDE); ShowWindow(g_settingsButton, SW_HIDE); ShowWindow(g_privateButton, SW_HIDE); ShowWindow(g_downloadsButton, SW_HIDE); ShowWindow(g_findButton, SW_HIDE);
    const int toolbarY = titleHeight + tabHeight + 4; const int webTop = toolbarY + 34;
    MoveWindow(g_tabStrip, 0, titleHeight, vertical ? tabWidth : std::max(200, width), vertical ? std::max(200, static_cast<int>(rc.bottom - titleHeight)) : 32, TRUE);
    const int toolbarX = tabWidth;
    MoveWindow(g_back, toolbarX + 5, toolbarY, 24, 30, TRUE); MoveWindow(g_forward, toolbarX + 33, toolbarY, 24, 30, TRUE); MoveWindow(g_reload, toolbarX + 61, toolbarY, 58, 30, TRUE); MoveWindow(g_home, toolbarX + 124, toolbarY, 48, 30, TRUE);
    const int addressLeft = toolbarX + 177, menuWidth = 36, rightMargin = 5; int menuLeft = std::max(addressLeft + 126, width - menuWidth - rightMargin);
    MoveWindow(g_address, addressLeft, toolbarY, std::max(80, menuLeft - addressLeft - 6), 30, TRUE);
    MoveWindow(g_overflowButton, menuLeft, toolbarY, menuWidth, 30, TRUE);
    for (size_t i = 0; i < g_tabs.size(); ++i) if (g_tabs[i]->controller) {
        RECT webBounds = rc; webBounds.left = tabWidth; webBounds.top = webTop;
        bool visible = static_cast<int>(i) == g_activeTab;
        if (g_splitView && g_splitSecondTab >= 0 && g_splitSecondTab < static_cast<int>(g_tabs.size()) && g_splitSecondTab != g_activeTab) {
            int splitLeft = tabWidth + (rc.right - tabWidth) / 2;
            if (static_cast<int>(i) == g_activeTab) webBounds.right = splitLeft;
            else if (static_cast<int>(i) == g_splitSecondTab) { webBounds.left = splitLeft; visible = true; }
            else visible = false;
        }
        g_tabs[i]->controller->put_Bounds(webBounds);
        g_tabs[i]->controller->put_IsVisible(visible ? TRUE : FALSE);
    }
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
    g_sitePolicies.clear(); g_permissionPolicies.clear();
    v3data::ClearHistory(g_dataDirectory); v3data::SaveDownloads({}, g_dataDirectory); v3data::ClearSession(g_dataDirectory); SaveSitePolicies();
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
    HINTERNET internet = InternetOpenW(L"PaneBrowser/4.0", INTERNET_OPEN_TYPE_PRECONFIG, nullptr, nullptr, INTERNET_FLAG_NO_CACHE_WRITE);
    if (internet) {
        DWORD timeout = 5000;
        InternetSetOptionW(internet, INTERNET_OPTION_CONNECT_TIMEOUT, &timeout, sizeof(timeout));
        InternetSetOptionW(internet, INTERNET_OPTION_RECEIVE_TIMEOUT, &timeout, sizeof(timeout));
        const wchar_t* headers = L"User-Agent: PaneBrowser/4.0\r\nAccept: application/vnd.github+json\r\n";
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

bool HandleShortcut(UINT vk, bool ctrl, bool shift, bool alt) { if(alt)return false; if(!ctrl && vk==VK_F5){if(ActiveWebView())ActiveWebView()->Reload();return true;} if(!ctrl)return false; if(ctrl&&shift&&vk==VK_SPACE){ShowCommandPalette();return true;} if(ctrl&&shift&&vk=='T'){ReopenClosedTab();return true;} if(ctrl&&vk=='L'){SetFocus(g_address);SendMessageW(g_address,EM_SETSEL,0,-1);} else if(ctrl&&vk=='T'&&g_settings.tabsEnabled)CreateNewTab(L"dashboard",false); else if(ctrl&&vk=='W')CloseTab(g_activeTab); else if (ctrl && (vk == 'R' || vk == VK_F5)) { if(ActiveWebView()) ActiveWebView()->Reload(); } else if(ctrl&&vk==VK_TAB){int n=static_cast<int>(g_tabs.size());if(n)SetActiveTab((g_activeTab+(shift?-1:1)+n)%n);} else if(ctrl&&vk=='F')FindInPage(); else if(ctrl&&vk=='D')AddCurrentBookmark(); else if(ctrl&&vk=='J')ShowDownloads(); else if(ctrl&&vk=='H')ShowHistory(); else if(ctrl&&shift&&vk=='P')ShowPrivate(); else if(ctrl&&(vk==VK_OEM_PLUS||vk==VK_ADD))AdjustZoom(0.1); else if(ctrl&&(vk==VK_OEM_MINUS||vk==VK_SUBTRACT))AdjustZoom(-0.1); else if(ctrl&&vk=='0')ResetZoom(); else return false; return true; }

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
        COREWEBVIEW2_TRACKING_PREVENTION_LEVEL level = COREWEBVIEW2_TRACKING_PREVENTION_LEVEL_BALANCED;
        if (g_settings.privacyMode == L"strict") level = COREWEBVIEW2_TRACKING_PREVENTION_LEVEL_STRICT;
        // Custom currently keeps the balanced safety floor; per-site overrides are layered separately.
        profile3->put_PreferredTrackingPreventionLevel(level);
    }
}

bool IsSensitivePermissionKind(COREWEBVIEW2_PERMISSION_KIND kind) {
    return kind == COREWEBVIEW2_PERMISSION_KIND_CAMERA || kind == COREWEBVIEW2_PERMISSION_KIND_MICROPHONE || kind == COREWEBVIEW2_PERMISSION_KIND_GEOLOCATION || kind == COREWEBVIEW2_PERMISSION_KIND_NOTIFICATIONS || kind == COREWEBVIEW2_PERMISSION_KIND_CLIPBOARD_READ;
}

void ScheduleWebViewRecovery() {
    bool expected = false;
    if (g_recoveryScheduled.compare_exchange_strong(expected, true) && g_main) PostMessageW(g_main, kRecoverWebViewsMessage, 0, 0);
}

void FinishWebViewRecovery() {
    if (!g_recoveryInProgress.exchange(false)) return;
    g_recoveryReady.store(false);
    if (g_main) KillTimer(g_main, kRecoveryTimerId);
    std::vector<RecoveryDescriptor> pending;
    pending.swap(g_recoveryDescriptors);
    g_restoreSession.clear();
    g_restoreTabMetadata.clear();
    g_restoreActive = std::max(0, g_recoveryActiveNormal);
    for (const auto& descriptor : pending) {
        if (descriptor.privateMode || descriptor.authenticationWindow) continue;
        std::wstring uri = descriptor.uri.empty() ? L"dashboard" : descriptor.uri;
        if (uri.rfind(L"http://", 0) == 0 || uri.rfind(L"https://", 0) == 0) {
            g_restoreSession.push_back({uri, false});
            g_restoreTabMetadata.push_back({uri, descriptor.workspaceId, descriptor.groupId});
        }
    }
    g_restoreCursor = 0;
    g_restoringSession = false;
    if (g_environment) g_environment.Reset();
    if (g_restoreSession.empty()) g_restoreActive = 0;
    InitializeWebView();
}

void RecoverAllWebViews() {
    g_recoveryScheduled.store(false);
    if (g_recoveryInProgress.exchange(true)) return;
    g_recoveryDescriptors.clear();
    g_recoveryActiveNormal = 0;
    int normalIndex = 0;
    for (const auto& tab : g_tabs) {
        if (!tab || tab->privateMode || tab->authenticationWindow) continue;
        RecoveryDescriptor descriptor;
        descriptor.uri = tab->lastUri;
        descriptor.profileId = tab->profileId;
        descriptor.profileName = tab->profileName;
        descriptor.workspaceId = tab->workspaceId;
        descriptor.groupId = tab->groupId;
        if (FindTab(tab.get()) == g_activeTab) g_recoveryActiveNormal = normalIndex;
        ++normalIndex;
        g_recoveryDescriptors.push_back(std::move(descriptor));
    }
    SavePersistentData();
    for (auto& tab : g_tabs) {
        ++tab->generation;
        if (tab->controller) tab->controller->Close();
        tab->webview.Reset();
        tab->controller.Reset();
    }
    g_tabs.clear();
    g_activeTab = -1;
    g_recoveryReady.store(false);
    if (g_environment) g_environment.Reset();
    if (g_main) SetTimer(g_main, kRecoveryTimerId, 2000, nullptr);
}

void HandleManagedNewWindowRequest(BrowserTab* opener, ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) {
    if (!opener || !sender || !args) return;
    LPWSTR rawUri = nullptr;
    if (FAILED(args->get_Uri(&rawUri)) || !rawUri) { args->put_Handled(TRUE); return; }
    std::wstring requestedUri(rawUri);
    CoTaskMemFree(rawUri);
    BOOL userInitiated = FALSE;
    args->get_IsUserInitiated(&userInitiated);
    bool blankPopup = requestedUri.empty() || requestedUri == L"about:blank";
    if (!blankPopup && !userInitiated && !IsAuthenticationUri(requestedUri)) {
        args->put_Handled(TRUE);
        return;
    }
    ComPtr<ICoreWebView2Deferral> deferral;
    if (FAILED(args->GetDeferral(&deferral)) || !deferral) { args->put_Handled(TRUE); return; }
    ComPtr<ICoreWebView2NewWindowRequestedEventArgs> argsHold = args;
    auto popup = std::make_shared<BrowserTab>();
    popup->profileId = opener->profileId;
    popup->profileName = opener->profileName;
    popup->workspaceId = opener->workspaceId;
    popup->groupId = opener->groupId;
    popup->privateMode = opener->privateMode;
    popup->guestMode = opener->guestMode;
    popup->authenticationWindow = blankPopup || IsAuthenticationUri(requestedUri);
    popup->internalPage = false;
    popup->lastUri = requestedUri;
    popup->generation = ++g_profileGeneration;
    g_tabs.push_back(popup);
    SetActiveTab(static_cast<int>(g_tabs.size()) - 1);

    auto discardPopup = [popup]() {
        int index = FindTab(popup.get());
        if (index >= 0) {
            g_tabs.erase(g_tabs.begin() + index);
            if (g_activeTab >= static_cast<int>(g_tabs.size())) g_activeTab = static_cast<int>(g_tabs.size()) - 1;
            SetActiveTab(g_activeTab);
        }
    };
    auto callback = Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(
        [popup, argsHold, deferral, requestedUri, discardPopup](HRESULT result, ICoreWebView2Controller* controller) -> HRESULT {
            if (FAILED(result) || !controller || FindTab(popup.get()) < 0) {
                argsHold->put_Handled(TRUE);
                deferral->Complete();
                discardPopup();
                return FAILED(result) ? result : E_FAIL;
            }
            popup->controller = controller;
            if (FAILED(popup->controller->get_CoreWebView2(&popup->webview)) || !popup->webview) {
                argsHold->put_Handled(TRUE);
                deferral->Complete();
                discardPopup();
                return E_FAIL;
            }
            ConfigureProfilePrivacy(popup.get());
            auto navigationStartingCallback = Callback<ICoreWebView2NavigationStartingEventHandler>(
                [popup](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs* navArgs) -> HRESULT {
                    if (!navArgs || popup->privateMode) return S_OK;
                    LPWSTR navRawUri = nullptr;
                    if (FAILED(navArgs->get_Uri(&navRawUri)) || !navRawUri) return S_OK;
                    std::wstring origin = OriginFromUri(navRawUri);
                    CoTaskMemFree(navRawUri);
                    SitePolicy policy = GetSitePolicy(origin);
                    ComPtr<ICoreWebView2Settings> settings;
                    if (!origin.empty() && popup->webview && SUCCEEDED(popup->webview->get_Settings(&settings)) && settings) {
                        settings->put_IsScriptEnabled(policy.scriptsEnabled ? TRUE : FALSE);
                        settings->put_AreDefaultScriptDialogsEnabled(policy.scriptDialogsEnabled ? TRUE : FALSE);
                    }
                    return S_OK;
                });
            EventRegistrationToken navigationStartingToken{};
            popup->webview->add_NavigationStarting(navigationStartingCallback.Get(), &navigationStartingToken);
            navigationStartingCallback.Get()->Release();
            auto navigationCallback = Callback<ICoreWebView2NavigationCompletedEventHandler>(
                [popup](ICoreWebView2*, ICoreWebView2NavigationCompletedEventArgs*) -> HRESULT {
                    RecordCommittedNavigation(popup.get());
                    return S_OK;
                });
            EventRegistrationToken navigationToken{};
            popup->webview->add_NavigationCompleted(navigationCallback.Get(), &navigationToken);
            navigationCallback.Get()->Release();
            auto closeCallback = Callback<ICoreWebView2WindowCloseRequestedEventHandler>(
                [popup](ICoreWebView2*, IUnknown*) -> HRESULT {
                    int index = FindTab(popup.get());
                    if (index >= 0) CloseTab(index);
                    return S_OK;
                });
            EventRegistrationToken closeToken{};
            popup->webview->add_WindowCloseRequested(closeCallback.Get(), &closeToken);
            closeCallback.Get()->Release();
            HRESULT setWindow = argsHold->put_NewWindow(popup->webview.Get());
            if (FAILED(setWindow)) {
                argsHold->put_Handled(TRUE);
                discardPopup();
                deferral->Complete();
                return setWindow;
            }
            argsHold->put_Handled(TRUE);
            deferral->Complete();
            ResizeChildren();
            SetWindowTextW(g_address, popup->authenticationWindow ? L"Authentication window" : requestedUri.c_str());
            return S_OK;
        });
    HRESULT hr = E_FAIL;
    ComPtr<ICoreWebView2Environment10> env10;
    if (SUCCEEDED(g_environment->QueryInterface(IID_ICoreWebView2Environment10, reinterpret_cast<void**>(env10.GetAddressOf()))) && env10) {
        ComPtr<ICoreWebView2ControllerOptions> options;
        if (SUCCEEDED(env10->CreateCoreWebView2ControllerOptions(&options)) && options) {
            std::wstring profileName = WebViewProfileName(*popup);
            if (!profileName.empty()) options->put_ProfileName(profileName.c_str());
            options->put_IsInPrivateModeEnabled(popup->privateMode ? TRUE : FALSE);
            hr = env10->CreateCoreWebView2ControllerWithOptions(g_main, options.Get(), callback.Get());
        }
    }
    if (FAILED(hr) && !popup->privateMode && popup->profileId == L"personal") hr = g_environment->CreateCoreWebView2Controller(g_main, callback.Get());
    if (FAILED(hr)) {
        argsHold->put_Handled(TRUE);
        deferral->Complete();
        discardPopup();
    }
    callback.Get()->Release();
}

void CreateNewTab(const std::wstring& initialUri, bool privateMode) {
    if (!g_environment || !g_tabStrip) return;
    auto tab = std::make_shared<BrowserTab>();
    tab->privateMode = privateMode;
    tab->profileId = g_activeProfile.id;
    tab->profileName = g_activeProfile.name;
    tab->guestMode = g_activeProfile.kind == L"guest";
    tab->generation = ++g_profileGeneration;
    tab->lastActiveTick = GetTickCount64();
    if (g_restoringSession && !privateMode && g_restoreCursor < g_restoreTabMetadata.size()) {
        const auto& metadata = g_restoreTabMetadata[g_restoreCursor++];
        if (metadata.uri == initialUri || metadata.uri.empty()) { tab->workspaceId = metadata.workspaceId; tab->groupId = metadata.groupId; }
    }
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
            auto navigationStartingCallback = Callback<ICoreWebView2NavigationStartingEventHandler>(
                [tab](ICoreWebView2*, ICoreWebView2NavigationStartingEventArgs* args) -> HRESULT {
                    if (!args || tab->privateMode) return S_OK;
                    LPWSTR rawUri = nullptr;
                    if (FAILED(args->get_Uri(&rawUri)) || !rawUri) return S_OK;
                    std::wstring origin = OriginFromUri(rawUri);
                    CoTaskMemFree(rawUri);
                    SitePolicy policy = GetSitePolicy(origin);
                    ComPtr<ICoreWebView2Settings> settings;
                    if (!origin.empty() && SUCCEEDED(tab->webview->get_Settings(&settings)) && settings) {
                        settings->put_IsScriptEnabled(policy.scriptsEnabled ? TRUE : FALSE);
                        settings->put_AreDefaultScriptDialogsEnabled(policy.scriptDialogsEnabled ? TRUE : FALSE);
                    }
                    return S_OK;
                });
            EventRegistrationToken navigationStartingToken{};
            tab->webview->add_NavigationStarting(navigationStartingCallback.Get(), &navigationStartingToken);
            navigationStartingCallback.Get()->Release();
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
            auto permissionCallback = Callback<ICoreWebView2PermissionRequestedEventHandler>(
                [tab](ICoreWebView2*, ICoreWebView2PermissionRequestedEventArgs* args) -> HRESULT {
                    if (!args) return E_INVALIDARG;
                    COREWEBVIEW2_PERMISSION_KIND kind{};
                    args->get_PermissionKind(&kind);
                    LPWSTR rawUri = nullptr;
                    args->get_Uri(&rawUri);
                    std::wstring origin = rawUri ? OriginFromUri(rawUri) : OriginFromUri(tab->lastUri);
                    if (rawUri) CoTaskMemFree(rawUri);
                    std::wstring key = PermissionPolicyKey(origin, kind);
                    auto policy = g_permissionPolicies.find(key);
                    if (tab->privateMode) args->put_State(COREWEBVIEW2_PERMISSION_STATE_DENY);
                    else if (policy != g_permissionPolicies.end()) args->put_State(policy->second);
                    else if (IsSensitivePermissionKind(kind)) args->put_State(COREWEBVIEW2_PERMISSION_STATE_DENY);
                    else args->put_State(COREWEBVIEW2_PERMISSION_STATE_DEFAULT);
                    return S_OK;
                });
            EventRegistrationToken permissionToken{};
            tab->webview->add_PermissionRequested(permissionCallback.Get(), &permissionToken);
            permissionCallback.Get()->Release();
            auto processFailedCallback = Callback<ICoreWebView2ProcessFailedEventHandler>(
                [tab](ICoreWebView2*, ICoreWebView2ProcessFailedEventArgs* args) -> HRESULT {
                    if (!args) return E_INVALIDARG;
                    COREWEBVIEW2_PROCESS_FAILED_KIND kind{};
                    args->get_ProcessFailedKind(&kind);
                    tab->crashed = true;
                    if (kind == COREWEBVIEW2_PROCESS_FAILED_KIND_BROWSER_PROCESS_EXITED || kind == COREWEBVIEW2_PROCESS_FAILED_KIND_RENDER_PROCESS_EXITED || kind == COREWEBVIEW2_PROCESS_FAILED_KIND_RENDER_PROCESS_UNRESPONSIVE) ScheduleWebViewRecovery();
                    return S_OK;
                });
            EventRegistrationToken processFailedToken{};
            tab->webview->add_ProcessFailed(processFailedCallback.Get(), &processFailedToken);
            processFailedCallback.Get()->Release();
            auto newWindowCallback = Callback<ICoreWebView2NewWindowRequestedEventHandler>(
                [tab](ICoreWebView2* sender, ICoreWebView2NewWindowRequestedEventArgs* args) -> HRESULT {
                    HandleManagedNewWindowRequest(tab.get(), sender, args);
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
            std::wstring profileName = WebViewProfileName(*tab);
            if (!profileName.empty()) options->put_ProfileName(profileName.c_str());
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
            ComPtr<ICoreWebView2Environment5> env5;
            if (SUCCEEDED(environment->QueryInterface(IID_ICoreWebView2Environment5, reinterpret_cast<void**>(env5.GetAddressOf()))) && env5) {
                auto browserExitCallback = Callback<ICoreWebView2BrowserProcessExitedEventHandler>([](ICoreWebView2Environment*, ICoreWebView2BrowserProcessExitedEventArgs*) -> HRESULT {
                    if (g_recoveryInProgress.load() && g_main) {
                        g_recoveryReady.store(true);
                        PostMessageW(g_main, kRecoverWebViewsMessage, 1, 0);
                    }
                    return S_OK;
                });
                EventRegistrationToken browserExitToken{};
                env5->add_BrowserProcessExited(browserExitCallback.Get(), &browserExitToken);
                browserExitCallback.Get()->Release();
            }
            if (!g_restoreSession.empty()) { g_restoreCursor = 0; g_restoringSession = true; if (g_settings.tabsEnabled) { for (const auto& entry : g_restoreSession) CreateNewTab(entry.uri, false); SetActiveTab(g_restoreActive); } else { int restoreIndex = std::max(0, std::min(g_restoreActive, static_cast<int>(g_restoreSession.size()) - 1)); CreateNewTab(g_restoreSession[static_cast<size_t>(restoreIndex)].uri, false); } g_restoringSession = false; } else CreateNewTab(L"dashboard");
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
    AppendMenuW(menu, MF_STRING, kContextCommandPalette, L"Command Palette");
    AppendMenuW(menu, MF_STRING, kContextSiteData, L"Site Data Center");
    AppendMenuW(menu, MF_STRING, kContextWorkspace, L"Workspace and tab groups");
    AppendMenuW(menu, MF_STRING, kContextNotes, L"Quick Notes");
    AppendMenuW(menu, MF_STRING, kContextReading, L"Toggle Reading Mode");
    AppendMenuW(menu, MF_STRING, kContextSplit, L"Toggle Split View");
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
    case kRecoverWebViewsMessage:
        if (wp == 0) RecoverAllWebViews();
        else if (g_recoveryReady.load()) FinishWebViewRecovery();
        return 0;
    case kUpdateResultMessage: {
        std::unique_ptr<UpdateCheckResult> result(reinterpret_cast<UpdateCheckResult*>(lp));
        g_updateCheckRunning.store(false);
        if (!result) return 0;
        if (!result->requestSucceeded) { MessageBoxW(g_main, L"No release information was available.", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        if (result->tag == L"v4.0.0" || result->tag == L"4.0.0") { MessageBoxW(g_main, L"You are using the latest Pane Browser release (v4.0.0).", L"Pane Browser", MB_OK | MB_ICONINFORMATION); return 0; }
        std::wstring prompt = L"Latest GitHub release: " + result->tag + L"\nOpen the releases page to download it?";
        if (MessageBoxW(g_main, prompt.c_str(), L"Pane Browser", MB_YESNO | MB_ICONINFORMATION) == IDYES) ShellExecuteW(g_main, L"open", L"https://github.com/xizar280513/Pane-Browser/releases", nullptr, nullptr, SW_SHOWNORMAL);
        return 0;
    }
    case WM_TIMER:
        if (wp == 0x733) { UpdateDownloadStates(); return 0; }
        if (wp == 0x745) { SleepInactiveTabs(); return 0; }
        if (wp == 0x734) { KillTimer(hwnd, 0x734); CheckForUpdates(); return 0; }
        if (wp == kRecoveryTimerId) {
            KillTimer(hwnd, kRecoveryTimerId);
            if (g_recoveryInProgress.load()) FinishWebViewRecovery();
            return 0;
        }
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
        case kContextCommandPalette: ShowCommandPalette(); return 0;
        case kContextSiteData: ShowSiteData(); return 0;
        case kContextWorkspace: ShowWorkspace(); return 0;
        case kContextNotes: ShowNotes(); return 0;
        case kContextReading: ToggleReadingMode(); return 0;
        case kContextSplit: ToggleSplitView(); return 0;
        case kContextBookmarks: ShowBookmarks(); return 0;
        case kContextDownloads: ShowDownloads(); return 0;
        case kContextClearData: ClearBrowsingData(); return 0;
        }
        break;
    case WM_DESTROY:
        if (g_downloadTimer) KillTimer(hwnd, g_downloadTimer);
        KillTimer(hwnd, 0x734);
        KillTimer(hwnd, 0x745);
        if (!g_deleteAllInProgress) SavePersistentData();
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(hwnd, msg, wp, lp);
}

} // namespace

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    if (wcsstr(GetCommandLineW(), L"--pane-delete-all") != nullptr) return RunDeferredRootDeletion();
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
    SetTimer(g_main, 0x745, 60000, nullptr);
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
