# Pane Browser v2.0.0

Pane Browser is a compact Windows browser shell built with native C++/Win32 and Microsoft WebView2. Version 2.0.0 expands the original browsing window with local data storage, bookmarks, private tabs, configurable appearance, settings, and single-file distribution options.

The implementation in this repository is independent. It does not extract or reuse source code from another browser project.

## Highlights

Pane Browser v2.0.0 includes direct URL navigation, Startpage search fallback, Back/Forward/Refresh/Home controls, a tab strip with close and new-tab controls, actual final-URL tracking, and WebView2 handling for links that request another window.

The new release adds persistent history stored under the user's local application data directory, a Bookmarks page with add/open/delete actions, a Private button backed by WebView2's private controller profile, and a Settings page. Settings can switch between System, Light, and Dark appearance, enable or disable tabs, turn persistent history on or off, and enable the optional custom title-bar layout.

## Local data

Normal browsing data is stored by WebView2 under the Pane Browser application-data directory. Pane Browser stores its lightweight application records in the same product area:

```text
%LOCALAPPDATA%\Pane Browser\
├── history.pbh
├── bookmarks.pbb
├── settings.pbs
└── WebView2\
```

History and bookmarks use UTF-8-safe, escaped line records rather than a database, which keeps the distribution small and makes the files easy to back up. Private tabs use a separate WebView2 private profile and do not add visited pages to the normal History file. Private profile data is created in a temporary process-specific directory.

## Settings and interface

The Settings page provides the following options:

| Setting | Behavior |
|---|---|
| Theme | Follows Windows, or forces Light/Dark for Pane Browser chrome and internal pages |
| Enable tabs | Shows or hides the tab strip and disables the new-tab workflow when off |
| Persistent history | Controls whether normal History is loaded from and saved to disk |
| Custom title bar | Uses an optional client-area title bar with custom minimize, maximize, and close controls |

The native toolbar, Home page, History page, Bookmarks page, and Settings page refresh their colors when the Windows theme changes while the application is running.

## Distribution

### Single portable executable

`PaneBrowser-Portable.exe` is the recommended no-install download. It embeds the browser core and `WebView2Loader.dll` as resources, extracts them to a temporary runtime directory when launched, starts the core browser, waits for it to close, and removes the temporary files. The user downloads and runs one EXE; no companion DLL is required beside it.

The Microsoft WebView2 Runtime is still required on the Windows computer. The portable single EXE does not bundle Microsoft's WebView2 Runtime itself.

### Single installer executable

`PaneBrowser-Setup.exe` is a self-contained installer launcher. It extracts the browser core and WebView2 loader to `%LOCALAPPDATA%\Pane Browser`, creates a Desktop shortcut and a Start Menu shortcut, and points both shortcuts to the installed core executable. The installer also requires the Microsoft WebView2 Runtime to be present on the computer.

## Build requirements

The build environment requires:

- Windows x64 target support through MinGW-w64.
- The Microsoft WebView2 SDK under `sdk/webview2`.
- The WebView2 loader import library and DLL from the SDK's x64 directory.
- A Windows resource compiler such as MinGW-w64 `windres`.

Build the core browser and the two v2.0.0 single-EXE outputs with:

```bash
./build_v2.sh
```

The script produces:

```text
PaneBrowserCore.exe
PaneBrowser-Portable.exe
PaneBrowser-Setup.exe
```

`PaneBrowserCore.exe` is the unpacked development/runtime binary. End users should normally receive `PaneBrowser-Portable.exe` or `PaneBrowser-Setup.exe` instead.

## Source layout

| File | Role |
|---|---|
| `PaneBrowser.cpp` | Native browser window, WebView2 controllers, tabs, History, Bookmarks, Settings, private mode, and theme logic |
| `v2_data.h` | File-based history, bookmark, settings, and profile-path helpers |
| `single_portable_launcher.cpp` | Self-extracting portable launcher |
| `single_installer.cpp` | Self-extracting installer and shortcut creator |
| `build_v2.sh` | Core, resource, portable, and installer build script |
| `PaneBrowser.rc` | Core application icon resource |
| `portable_launcher.rc` | Portable launcher resources and embedded payloads |
| `installer.rc` | Installer resources and embedded payloads |
| `PaneBrowser.ico` | Multi-resolution Windows application icon |
| `resource_v2.h` | Resource identifiers for v2 packaging |

## Privacy and limitations

Pane Browser does not add its own telemetry, advertising, or background synchronization service. Normal websites still make their own network requests and may collect data according to their own policies. Private mode prevents Pane Browser from adding those visits to normal file-based History and uses WebView2 private mode, but it should not be treated as anonymity software.

The v2.0.0 builds are cross-compiled and statically checked in the Linux development environment. Final WebView2 rendering, Windows shell shortcuts, title-bar hit testing, private-profile behavior, and WebView2 Runtime compatibility must be verified on a Windows system.

## License

Pane Browser is released under the MIT License. See [LICENSE](LICENSE).
