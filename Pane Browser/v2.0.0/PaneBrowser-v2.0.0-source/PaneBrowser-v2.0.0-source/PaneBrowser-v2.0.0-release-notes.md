# Pane Browser v2.0.0 — Build report

## Implemented features

Pane Browser v2.0.0 adds file-based persistent History, file-based Bookmarks, WebView2 private-mode tabs, an optional custom client-area title bar, a Settings page, optional tab support, and selectable System/Light/Dark UI themes. The v1 navigation, Startpage search fallback, tab close/new-tab controls, actual final URL recording, internal Home/History pages, and embedded Pane Browser icon are retained.

## Distribution outputs

`PaneBrowser-v2.0.0-portable.exe` is a single-file self-extracting launcher. It embeds `PaneBrowserCore.exe` and `WebView2Loader.dll`, unpacks them to a process-specific temporary folder, runs the browser, waits for exit, and removes the temporary files.

`PaneBrowser-v2.0.0-setup.exe` is a single-file installer launcher. It embeds the same core and loader, installs them under `%LOCALAPPDATA%\\Pane Browser`, and creates Desktop and Start Menu shortcuts pointing to the installed core executable.

The Microsoft WebView2 Runtime remains a required system dependency for both outputs. It is not bundled into the single-file launchers.

## Source package

`PaneBrowser-v2.0.0-source.zip` includes the updated C++ source, persistence helper, private-profile integration, settings/bookmarks/history pages, resource files, portable launcher, installer launcher, build script, icon, README, `.gitignore`, and MIT License.

## Verification

- Core, portable launcher, and installer each compile as PE32+ Windows x64 GUI executables.
- Portable and installer executables contain a `.rsrc` resource directory with embedded payload resources.
- The source passes MinGW-w64 syntax compilation. Remaining warnings are limited to the WebView2 SDK pragma and dynamic Windows API function-pointer casts.
- Final WebView2 rendering, private profile behavior, shortcut creation, title-bar hit testing, and Windows shell behavior require testing on a real Windows machine with WebView2 Runtime installed.

## Known scope notes

The file-based records are intentionally simple UTF-8-safe escaped line files. Private mode is implemented using WebView2 controller options and does not add private navigations to the normal History file. The custom title bar is optional and can be enabled from Settings; the default remains the native Windows title bar for compatibility.
