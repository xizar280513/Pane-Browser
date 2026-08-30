# Pane Browser v1.0.0

## Distribution refinement

This package preserves the Pane Browser v1.0.0 browser core and introduces a single-file portable distribution.

### Portable executable

`PaneBrowser-v1.0.0-portable.exe` embeds the v1.0.0 browser core and `WebView2Loader.dll`. The launcher extracts them into a temporary directory only while the browser is running, starts the core application, waits for it to exit, and performs best-effort cleanup.

Users no longer need to download a ZIP and manually keep `WebView2Loader.dll` beside the browser launcher.

### Preserved v1 behavior

The browser core remains the v1.0.0 baseline with native Win32 controls, Microsoft WebView2 rendering, Startpage search fallback, navigation controls, multi-tab browsing, session-only History, Windows theme adaptation, and the Pane Browser icon.

Persistent History, bookmarks, private mode, session restore, Downloads, Settings, and other later capabilities remain part of the v2.0.0 and v3.0.0 release lines. They are not added to this v1 package.

### Requirement

Microsoft WebView2 Runtime must be installed on the target Windows x64 system. The launcher embeds `WebView2Loader.dll`, but it does not bundle the WebView2 Runtime.

### Verification status

The core and launcher are cross-compiled as PE32+ Windows x64 GUI executables. Runtime rendering, WebView2 startup, and temporary-folder cleanup should be verified on a real Windows x64 device before public distribution.
