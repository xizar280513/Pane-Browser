# Pane Browser

Pane Browser is an independent native Windows browser shell built with C++17, Win32, and Microsoft WebView2. The window chrome and tab strip are native controls, while web content is rendered by the WebView2 runtime.

## Version 3.0.0

Pane Browser v3.0.0 keeps the v1/v2 navigation model and adds a more complete daily-browsing workflow.

### Features

- Direct URL navigation and Startpage search fallback.
- Back, Forward, Refresh, Home, History, Bookmarks, Downloads, Settings, and Private controls.
- Multi-tab browsing with close buttons, a new-tab button, an optional tab strip, and reopen-closed-tab support.
- File-based persistent browsing history with atomic replacement and startup initialization of the data directory.
- Session restore for normal HTTP(S) tabs. Private tabs are never saved for session restore.
- Bookmarks with add, open, delete, and HTML import/export.
- Private tabs backed by WebView2 private controller options. Private navigation is excluded from normal History.
- Download manager with a Downloads page, a configurable download folder, state tracking, safe filenames, collision avoidance, open-file, and open-folder actions.
- Find in page through `Ctrl+F` or the Find button.
- Global keyboard shortcuts for navigation, tabs, bookmarks, downloads, History, private mode, and zoom. They are dispatched from the main Windows message loop and do not require clicking the web page first.
- Context menu with tab, find, zoom, History, Bookmarks, Downloads, navigation, and data actions, including a WebView2 page-content context-menu integration.
- Clear browsing data for local History/download records and WebView2 profile data when the installed runtime exposes the profile API.
- Per-tab zoom controls with `Ctrl++`, `Ctrl+-`, and `Ctrl+0`.
- Settings for System/Light/Dark theme, tabs, persistent History, session restore, optional update checks, custom title bar, and search-engine selection.
- A three-dot overflow menu at the right of the address bar contains Find in page, Downloads, Private tab, Settings, Bookmarks, and History. Back, Forward, Refresh, and Home remain on the left.
- Search options include Google, Bing, DuckDuckGo, Brave Search, Startpage, Mojeek, Swisscows, Yahoo, Baidu, Naver, Yandex, Ecosia, Qwant, Seznam, Ask, AOL, Kagi, MetaGer, SearXNG, You.com, Perplexity, Yep, Sogou, 360 Search, Rambler, Daum, Goo, Excite, Dogpile, Gibiru, WolframAlpha, and a Custom URL mode.
- Runtime detection with a clear message when Microsoft WebView2 Runtime is unavailable.
- Optional GitHub release update checker, enabled from Settings or available as a Settings action.
- Per-user installer with upgrade behavior, Desktop and Start Menu shortcuts, and `/uninstall` support. User data is kept during uninstall.
- Single-file portable and installer launchers that embed the browser core and `WebView2Loader.dll`.
- Privacy-oriented profile handling: normal cookies/site data remain available for convenience, private tabs stay isolated, private download records are not persisted, History uses local badges instead of an external favicon service, and Balanced tracking prevention is requested where supported by the installed WebView2 Runtime.
- WebView2 native commands are accepted only from the internal `about:blank` pages used by Pane Browser; external websites cannot use the internal web-message command channel.

## Keyboard shortcuts

| Shortcut | Action |
|---|---|
| `Ctrl+L` | Focus the address bar |
| `Ctrl+T` | Open a new tab when tabs are enabled, regardless of current focus |
| `Ctrl+W` | Close the active tab, regardless of current focus |
| `Ctrl+Shift+T` | Restore the most recently closed normal tab |
| `Ctrl+Tab` | Activate the next tab |
| `Ctrl+Shift+Tab` | Activate the previous tab |
| `Ctrl+R` or `F5` | Refresh the active page |
| `Ctrl+F` | Find in page |
| `Ctrl+D` | Add the current page to Bookmarks |
| `Ctrl+J` | Open Downloads |
| `Ctrl+H` | Open History |
| `Ctrl+Shift+P` | Open a private tab |
| `Ctrl++` / `Ctrl+-` | Zoom in or out |
| `Ctrl+0` | Reset zoom |

## Local data

Normal application data is stored under:

```text
%LOCALAPPDATA%\Pane Browser\
├── history.pbh
├── bookmarks.pbb
├── settings.pbs
├── session.pbs
├── downloads.pbd
└── WebView2\
```

The selected search engine is used for text entered in the address bar; custom URLs may contain `{query}` or `%s`. The `history.pbh` file is created when persistent History is enabled and a normal HTTP(S) navigation is committed. v3 initializes the data directory during startup, before WebView2 is created, so the v2 startup-path defect is addressed. History is capped at 10,000 entries. All record files use escaped UTF-8 fields. Writes use a process-specific temporary file followed by `MoveFileExW` replacement to reduce the chance of a partially written record file, and invalid file-vs-directory paths are rejected.

Private tabs are excluded from normal History and from session restore. Private download records are not persisted. Normal cookies and site data are intentionally retained in the normal WebView2 profile so websites can remember sign-in. Clearing browsing data asks for confirmation, waits for WebView2 completion callbacks, and reports partial failures instead of claiming success unconditionally. Disabling persistent History or session restore removes the corresponding stale local record file.

## Distribution

### Portable

`PaneBrowser-v3.0.0-portable.exe` is one self-extracting portable executable. It contains the browser core and `WebView2Loader.dll`, extracts them into a unique temporary directory at runtime, launches the browser, waits for exit, retries cleanup to allow WebView2 child processes to release files, and reports cleanup only as best-effort.

### Installer

`PaneBrowser-v3.0.0-setup.exe` is one per-user installer executable. It installs the browser under `%LOCALAPPDATA%\Pane Browser`, updates existing files when Pane Browser is closed, and creates Desktop and Start Menu shortcuts. The Start Menu also receives an uninstall shortcut. The installer accepts `/uninstall` and preserves user History and Bookmarks.

Neither launcher bundles the Microsoft WebView2 Runtime. Windows x64 systems must have the Evergreen WebView2 Runtime installed. The runtime can be obtained from the official [Microsoft WebView2 download page](https://developer.microsoft.com/microsoft-edge/webview2/).

## Building from source

The provided `build_v3.sh` script expects MinGW-w64, GNU windres, and the included WebView2 SDK layout. From the project directory:

```bash
./build_v3.sh
```

The build creates these generated files:

```text
PaneBrowserCore.exe
PaneBrowser-Portable.exe
PaneBrowser-Setup.exe
WebView2Loader.dll
```

`PaneBrowserCore.exe` is the internal browser executable used by the two launchers. For normal distribution, provide the portable launcher or setup launcher instead of the core executable alone.

## Project files

| File | Purpose |
|---|---|
| `PaneBrowser.cpp` | Native window, WebView2 controllers, tabs, internal pages, History, Bookmarks, Downloads, Settings, shortcuts, and update checker |
| `v3_data.h` | Atomic local data files, session records, settings, and path helpers |
| `single_portable_launcher.cpp` | Single-file portable self-extractor |
| `single_installer.cpp` | Per-user installer, upgrade path, shortcut creator, and uninstaller mode |
| `build_v3.sh` | Reproducible MinGW-w64 build script |
| `EventToken.h` | WebView2 event token definitions required by the included SDK setup |
| `PaneBrowser.rc` | Application icon resources |
| `portable_launcher.rc` | Embedded portable payload resources |
| `installer.rc` | Embedded installer payload resources |
| `PaneBrowser.ico` | Multi-resolution Windows application icon |
| `resource_v2.h` | Resource identifiers shared by the launchers |
| `LICENSE` | MIT License |

## Verification status

The v3 source passes MinGW-w64 syntax and strict warning checks after excluding the expected WebView2 SDK pragma and dynamic Windows API function-pointer casts. The three generated executables are PE32+ Windows x64 GUI files and the launcher resources contain the core and loader payloads. Update checking runs asynchronously so network timeouts do not block the browser UI.

Final WebView2 rendering, downloads, private mode, session restore, custom title-bar hit testing, Windows shortcut creation, runtime detection, and installer upgrade/uninstall behavior must still be exercised on a real Windows x64 system with WebView2 Runtime installed. The project does not claim that Linux cross-compilation replaces that runtime test.

## License

Pane Browser is distributed under the MIT License. See `LICENSE`.
