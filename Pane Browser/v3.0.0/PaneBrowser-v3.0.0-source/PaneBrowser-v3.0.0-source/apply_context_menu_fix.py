from pathlib import Path
p=Path('/home/ubuntu/pane-browser-v3/PaneBrowser.cpp')
s=p.read_text()
marker='template <typename Fn>\nclass CallbackHolder<ICoreWebView2NewWindowRequestedEventHandler, Fn>'
insert='''template <typename Fn>
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
class CallbackHolder<ICoreWebView2NewWindowRequestedEventHandler, Fn>'''
if marker not in s: raise SystemExit('context callback marker missing')
s=s.replace(marker,insert,1)
# Forward declaration for shared menu builder.
needle='void ShowOverflowMenu();\n'
if needle not in s: raise SystemExit('overflow declaration missing')
s=s.replace(needle,needle+'void ShowNativeContextMenu(POINT point);\n',1)
# Add WebView2 context-menu registration after the NewWindow handler registration.
needle='''            EventRegistrationToken newWindowToken{};
            tab->webview->add_NewWindowRequested(newWindowCallback.Get(), &newWindowToken);
            newWindowCallback.Get()->Release();
            ComPtr<ICoreWebView2_4> web4;'''
insert='''            EventRegistrationToken newWindowToken{};
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
            ComPtr<ICoreWebView2_4> web4;'''
if needle not in s: raise SystemExit('new window registration block missing')
s=s.replace(needle,insert,1)
# Add shared menu function before WindowProc.
marker='LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {'
function='''void ShowNativeContextMenu(POINT pt) {
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

'''
if marker not in s: raise SystemExit('window proc marker missing')
s=s.replace(marker,function+marker,1)
# Replace host-only duplicated menu creation with shared builder.
old='''    case WM_CONTEXTMENU: {
        HMENU menu=CreatePopupMenu(); AppendMenuW(menu,MF_STRING,kContextNewTab,L"New tab"); AppendMenuW(menu,MF_STRING,kContextReopen,L"Reopen closed tab"); AppendMenuW(menu,MF_SEPARATOR,0,nullptr); AppendMenuW(menu,MF_STRING,kContextFind,L"Find in page"); AppendMenuW(menu,MF_STRING,kContextZoomIn,L"Zoom in"); AppendMenuW(menu,MF_STRING,kContextZoomOut,L"Zoom out"); AppendMenuW(menu,MF_STRING,kContextZoomReset,L"Reset zoom"); AppendMenuW(menu,MF_SEPARATOR,0,nullptr); AppendMenuW(menu,MF_STRING,kContextHistory,L"History"); AppendMenuW(menu,MF_STRING,kContextBookmarks,L"Bookmarks"); AppendMenuW(menu,MF_STRING,kContextDownloads,L"Downloads"); AppendMenuW(menu,MF_STRING,kContextClearData,L"Clear browsing data"); POINT pt{GET_X_LPARAM(lp),GET_Y_LPARAM(lp)}; if(pt.x==-1){GetCursorPos(&pt);} TrackPopupMenu(menu,TPM_RIGHTBUTTON,pt.x,pt.y,0,hwnd,nullptr); DestroyMenu(menu); return 0;
    }'''
new='''    case WM_CONTEXTMENU: {
        POINT pt{GET_X_LPARAM(lp), GET_Y_LPARAM(lp)};
        ShowNativeContextMenu(pt);
        return 0;
    }'''
if old not in s: raise SystemExit('old context menu block missing')
s=s.replace(old,new,1)
p.write_text(s)
print('context menu fix applied')
