# Pane Browser v1.0.0

Pane Browser v1.0.0 is an independent lightweight Windows browser shell built with native Win32 controls, C++17, and Microsoft WebView2.

This package preserves the v1.0.0 browser behavior while improving the way it is distributed and maintained. The browser core remains the same v1 application baseline; the new portable launcher embeds the core executable and `WebView2Loader.dll` into one downloadable Windows executable.

## Core purpose

The first Pane Browser release focuses on a direct, uncomplicated browsing window for Windows. It provides the essential controls without requiring a large application framework:

- Direct URL navigation and Startpage search fallback.
- Back, Forward, Refresh, and Home controls.
- Multiple tabs with a new-tab button and tab close buttons.
- Session-only browsing History with full committed URLs.
- History selection, deletion, Select all, and Deselect all actions.
- Light and Dark appearance following the Windows theme.
- A native Pane Browser application icon.
- Links requesting a new window are handled in the existing browser window rather than opening an additional application window.

## Portable distribution

`PaneBrowser-v1.0.0-portable.exe` is the recommended v1.0.0 download. It is a single self-extracting executable containing:

| Embedded component | Purpose |
|---|---|
| `PaneBrowser-v1.0.0-core.exe` | The v1.0.0 browser application built from `PaneBrowser.cpp`. |
| `WebView2Loader.dll` | The native loader used to connect the application to Microsoft WebView2. |

At runtime, the launcher extracts these files into a unique temporary directory, starts the browser with that directory as its working directory, waits for the browser to close, and attempts to remove the temporary files. Users do not need to place `WebView2Loader.dll` beside the downloaded portable launcher.

The portable launcher does **not** bundle the Microsoft WebView2 Runtime. Windows must have the Evergreen WebView2 Runtime installed. It can be obtained from the official [Microsoft WebView2 download page](https://developer.microsoft.com/microsoft-edge/webview2/).

## Local behavior

The original v1.0.0 build keeps browsing History for the current session. It does not provide the persistent History, bookmarks, private tabs, session restore, Downloads manager, or Settings system introduced in later Pane Browser releases. Those features belong to v2.0.0 and v3.0.0 and are intentionally not backported here so that the v1.0.0 behavior remains stable.

The browser uses Microsoft WebView2 for web content. When the core is run directly, its v1 profile is located beside the core executable according to the original v1 path-based behavior. When the single-file launcher is used, the unchanged core runs from a temporary extraction directory; its WebView2 profile is therefore temporary and is removed with that directory after exit. As a result, cookies, cache, login state, and other site data from a launcher session should not be expected to survive the next launch. This is a packaging consequence of the single-file requirement, not a new privacy or data-management feature.

## Source layout

The source package is organized around the files required to inspect or reproduce the v1.0.0 build:

| File or directory | Purpose |
|---|---|
| `PaneBrowser.cpp` | Native v1.0.0 browser implementation. |
| `PaneBrowser.rc` | Application icon resource definition. |
| `PaneBrowser.ico` | Pane Browser application icon. |
| `resource.h` | Application resource identifiers. |
| `single_portable_launcher_v1.cpp` | Single-file portable launcher source. |
| `portable_v1.rc` | Launcher payload resource definition. |
| `resource_v1.h` | Launcher resource identifiers. |
| `build_v1_portable.sh` | Reproducible MinGW-w64 build script. |
| `sdk/webview2/build/native` | Minimum WebView2 SDK files required by the build. |
| `LICENSE` | Project license supplied with the v1 source package. |
| `RELEASE_NOTES-v1.0.0.md` | Release information for this package. |

## Reproduce the build

The build script expects an x64 MinGW-w64 compiler, GNU `windres`, and the included WebView2 SDK layout. From the project directory, run:

```bash
./build_v1_portable.sh
```

The build produces the intermediate core and loader under `build/`, and the final launcher under `dist/`:

```text
build/PaneBrowser-v1.0.0-core.exe
build/WebView2Loader.dll
dist/PaneBrowser-v1.0.0-portable.exe
```

`build/PaneBrowser-v1.0.0-core.exe` is the browser core and requires `build/WebView2Loader.dll` beside it when run directly. For normal distribution, use `dist/PaneBrowser-v1.0.0-portable.exe` instead.

## Version relationship

Pane Browser v1.0.0 is the initial release line. Later versions add functionality rather than replacing the purpose of this baseline:

| Version | Direction |
|---|---|
| **v1.0.0** | Essential native browser shell and session History. |
| **v2.0.0** | Local persistent data, bookmarks, private mode, Settings, themes, optional tabs, and single-file distribution. |
| **v3.0.0** | Session restore, Downloads, Find in page, shortcuts, context menus, zoom, installer improvements, and stability/privacy refinements. |

## Project status

This package is intended to preserve the v1.0.0 behavior while making the download experience simpler. Static compilation and PE structure checks can be performed in the cross-compilation environment; final WebView2 rendering and portable cleanup should be exercised on a real Windows x64 system with WebView2 Runtime installed.

Pane Browser is an independent project and is not affiliated with Microsoft, Google, Startpage, or any other service mentioned in this document.

## License

Pane Browser is distributed under the license included in [`LICENSE`](LICENSE).
