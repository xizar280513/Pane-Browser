from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=p.read_text()
# Add a correctly typed completed-handler holder.
marker='template <typename Fn>\nclass CallbackHolder<ICoreWebView2AcceleratorKeyPressedEventHandler, Fn>'
insert='''template <typename Fn>
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
class CallbackHolder<ICoreWebView2AcceleratorKeyPressedEventHandler, Fn>'''
if marker not in s: raise SystemExit('clear callback insertion marker missing')
s=s.replace(marker,insert,1)
old='''void ClearBrowsingData() { g_history.clear(); g_downloads.clear(); v3data::ClearHistory(g_dataDirectory); v3data::SaveDownloads({},g_dataDirectory); v3data::ClearSession(g_dataDirectory); for(auto& tab:g_tabs) if(tab->webview){ComPtr<ICoreWebView2_13> w13; if(SUCCEEDED(tab->webview->QueryInterface(IID_ICoreWebView2_13, reinterpret_cast<void**>(w13.GetAddressOf())))){ComPtr<ICoreWebView2Profile> profile; if(SUCCEEDED(w13->get_Profile(&profile))&&profile){ComPtr<ICoreWebView2Profile2> p2; if(SUCCEEDED(profile->QueryInterface(IID_ICoreWebView2Profile2, reinterpret_cast<void**>(p2.GetAddressOf()))))p2->ClearBrowsingDataAll(nullptr);}}} MessageBoxW(g_main,L"Browsing history, download history, cookies, cache, and site data were requested to be cleared.",L"Pane Browser",MB_OK|MB_ICONINFORMATION); }'''
new='''void ClearBrowsingData() {
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
}'''
if old not in s: raise SystemExit('clear data function missing')
s=s.replace(old,new,1)
p.write_text(s)
print('clear data fix applied')
