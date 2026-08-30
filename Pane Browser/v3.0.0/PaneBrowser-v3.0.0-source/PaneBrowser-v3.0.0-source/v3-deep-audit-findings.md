# v3.0.0 deep audit — working findings

## Baseline

The latest v3.0.0 source is `/home/ubuntu/pane-browser-v3/PaneBrowser.cpp` with `v3_data.h`, build script, two single-file launchers, WebView2 SDK headers/loader, and a source ZIP. Current artifacts identify as PE32+ Windows x64 GUI files.

## Confirmed positive findings

- Normal WebView2 data is directed to `%LOCALAPPDATA%\\Pane Browser\\WebView2`, which supports persistent cookies/site data.
- Private tabs use `CreateCoreWebView2ControllerWithOptions` with `IsInPrivateModeEnabled=TRUE`; private controller creation failure does not fall back silently to normal mode.
- Application History, session restore, closed-tab restore, and persistent downloads exclude private tabs/records.
- Profile privacy helper requests Balanced tracking prevention through the profile interface where available.
- Strict syntax/build checks have passed in earlier runs; only known external SDK/dynamic-API warnings remain.
- Single-file launchers contain large `.rsrc` sections and embedded PE signatures.

## Potential risks to verify deeply

- `DataDirectory` treats `ERROR_ALREADY_EXISTS` from `CreateDirectoryW` as success even when the path may be a file; later writes can silently fail.
- `EnsureDirectory` has the same file-vs-directory ambiguity for custom download paths.
- `WriteLines` returns false on failed flush/move but callers generally ignore the result, so data-loss warnings are not surfaced.
- COM callback `QueryInterface` implementations ignore `riid` and return their event interface for every requested IID; this is nonconforming COM behavior and may create runtime fragility.
- Event registration tokens are not retained/removable, although controller/webview destruction eventually releases the event sources; this is a lifecycle cleanup risk rather than an immediate leak.
- Portable cleanup is best effort and may fail while WebView2 child processes still hold files. Microsoft recommends waiting for browser processes to exit before deleting UDFs.
- Installer uninstall script deletes `PaneBrowser.exe`, loader, setup executable, and the script but intentionally leaves user data; this is useful for privacy preservation but may leave an empty folder if WebView2 files remain.
- Installer shortcut creation results are ignored; failure can be silent on unusual Windows profiles or redirected folders.
- Context menu handling is attached to the host window; WebView2 web content may receive its own context menu, so native context-menu coverage may be incomplete.
- No live Windows/WebView2 runtime is present in this environment; actual cookies, private isolation, shortcut routing, title-bar behavior, and installer operations remain unverified.

No source changes were made as part of the audit phase after the user's request.
