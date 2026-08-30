# Pane Browser v5.0.0 MiniEdition

Pane Browser MiniEdition is a deliberately minimal independent native C++17/Win32/WebView2 browser shell. It is not the Regular Pane Browser v5 product and it does not produce a Regular portable build or an installer.

## Output

The build creates only:

```text
PaneBrowser-v5.0.0-MiniEdition-portable.exe
```

The final executable is a single-file launcher. It embeds the minimal browser core and the official `WebView2Loader.dll` payload. At runtime it extracts both files only to a temporary folder, runs the browser, waits for it to close, and removes the extracted files. No loader DLL is required beside the final executable. The loader is kept as an embedded runtime payload rather than using the experimental MinGW static-loader shim, because startup reliability is more important than the last few kilobytes.

## Intentionally included

MiniEdition provides one browser tab, an address bar, Back, Forward, Refresh, Home, direct URL navigation, and Startpage search for ordinary text queries. It supports native browser fullscreen with `F11` and handles WebView2 `ContainsFullScreenElementChanged` so HTML video fullscreen can expand the WebView and hide the browser chrome. Press `F11` again to restore the previous window state. Its Home page and native controls follow the Windows Light/Dark application theme at startup and respond to system theme changes while the app is running. It uses the installed Microsoft Edge WebView2 Runtime for rendering.

## Intentionally excluded

MiniEdition has no History UI, no History file, no persistent History, no Bookmarks, no Incognito/private mode, no profile manager, no downloads manager, no settings page, no theme toggle, no tab strip, no multi-tab support, no session restore, no workspace features, no extensions, no installer, and no ordinary `PaneBrowser-v5.0.0-portable.exe` or `PaneBrowser-v5.0.0-setup.exe` output.

## Privacy model

The MiniEdition browser creates a randomly named temporary WebView2 User Data Folder for each run. It does not use the normal Pane Browser `%LOCALAPPDATA%\\Pane Browser` data root and does not intentionally save app-owned History, Bookmarks, settings, session records, or profiles. On normal exit, the core releases its WebView2 objects and removes the temporary browser-data folder with retry logic. This is a best-effort local privacy design, not a guarantee against data retained by websites, the operating system, crash services, DNS infrastructure, or the WebView2 Runtime itself.

Files downloaded by websites are not exposed through a MiniEdition download manager. Any browser-initiated file created by a website or operating system outside the temporary WebView2 folder is outside this app's cleanup scope.

## Requirements

Windows x64 with the Microsoft Edge WebView2 Runtime installed is required. The Runtime is an external prerequisite and is not embedded into the application.

## Build

A MinGW-w64 x64 toolchain and GNU `windres` are required. The source package includes the minimum WebView2 SDK headers, the official x64 `WebView2Loader.dll`, and its import library. The final executable embeds the loader and does not need a separate loader DLL beside it.

```bash
./build_v5_mini.sh
./audit_mini_v5.sh
```

The build script removes intermediate core, object, and resource files and intentionally removes any files named `PaneBrowser-v5.0.0-portable.exe` or `PaneBrowser-v5.0.0-setup.exe` from the project root. The only final executable is the MiniEdition portable file.

## System theme behavior

MiniEdition reads the Windows application-theme preference and applies it to the native window, navigation controls, address bar, and Home page. A Windows theme change is received through the native window message path and applied without adding a theme toggle or persistent settings file.

## Remaining limitations

Runtime behavior has not been tested on Windows inside the current Linux build environment. Authentication behavior, Startpage availability, WebView2 Runtime compatibility, video fullscreen, F11 fullscreen, temporary-folder cleanup, DPI behavior, and navigation edge cases require Windows x64 testing. The project has its own independent source tree and implementation.
