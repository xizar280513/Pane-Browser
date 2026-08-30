# Pane Browser v3.0.0 — QA report

## Scope

This audit covers the v3.0.0 source derived from the v2 baseline. It reviews the requested productivity features, persistent-history defect, private mode, installer behavior, portability, privacy hardening, and likely hidden failure points.

## Feature coverage

| Area | Static finding |
|---|---|
| Session restore | Normal HTTP(S) tabs are serialized to `session.pbs`; private tabs are excluded; active normal-tab index is normalized. |
| Downloads | WebView2 `DownloadStarting` is registered when the runtime exposes `ICoreWebView2_4`; names are sanitized and collisions receive numbered suffixes. Private download records are filtered out before persistent storage. |
| Find in page | `Ctrl+F` and Find invoke WebView2 script using the page's find behavior. |
| Shortcuts | Accelerator events are registered on each controller; toolbar/address-bar shortcut handling is also present. |
| Reopen closed tabs | Up to ten normal HTTP(S) tab URLs are retained in memory and reopened with `Ctrl+Shift+T`. |
| Context menu | Native menu exposes tabs, find, zoom, History, Bookmarks, Downloads, and clear-data commands. |
| Clear browsing data | Local History and download records are removed; WebView2 profile clearing is requested through `ICoreWebView2Profile2` when available. |
| Zoom | Per-controller zoom is adjusted and reset through keyboard/context commands. |
| Bookmark import/export | Netscape-style HTML export and basic HTML `HREF` import are implemented with duplicate suppression. |
| Installer | Per-user install/upgrade, Desktop/Start Menu shortcuts, uninstall shortcut, `/uninstall`, and data preservation are implemented. |
| Runtime detection | `GetAvailableCoreWebView2BrowserVersionString` is checked before environment creation. |
| Update checker | Optional GitHub latest-release query has timeout and User-Agent headers. |
| Custom title bar | Optional title bar includes client controls and border hit-test handling. |
| Persistent History fix | Data directory is initialized before WebView2 startup; `SavePersistentData` also self-initializes defensively. Writes are atomic. |
| Privacy refinement | Normal UDF remains persistent for cookies/site data; private controllers remain isolated; profile-level Balanced tracking prevention is requested when the profile API is available. |

## Privacy and convenience review

The normal profile uses a writable per-user WebView2 UDF under `%LOCALAPPDATA%\\Pane Browser\\WebView2` and is intentionally not deleted at ordinary shutdown. This preserves normal cookies and site data for sign-in convenience. Private tabs use `IsInPrivateModeEnabled=TRUE` through WebView2 controller options and are not silently converted into normal tabs if the private controller cannot be created.

The profile helper requests `COREWEBVIEW2_TRACKING_PREVENTION_LEVEL_BALANCED` when the installed runtime exposes the profile interface. This is deliberately conservative and is intended to reduce common cross-site tracking without deleting ordinary first-party cookies. The application does not add extensions, custom DNS, cloud synchronization, or Google-account integration.

## Build verification

The official `build_v3.sh` script completed successfully. The core, portable launcher, and setup launcher each identify as PE32+ Windows x64 GUI executables. Strict syntax checks for `PaneBrowser.cpp`, `single_portable_launcher.cpp`, and `single_installer.cpp` returned status zero. Remaining warnings are the expected WebView2 SDK MSVC pragma and two dynamic Windows API function-pointer casts used for optional theme APIs; no application compile warning remains.

The portable and setup launchers have approximately 3.3 MiB `.rsrc` sections and contain multiple embedded `MZ` signatures, consistent with embedded PE payloads. Their sizes are materially larger than the core because they contain the core and loader resources.

## Important limitations

The development environment has no Wine or Windows WebView2 runtime, so this audit cannot confirm actual Chromium rendering, cookie persistence on a real website, private controller creation on a particular runtime version, tracking-prevention effectiveness, downloads, title-bar mouse behavior, shell shortcut display, or installer/uninstaller behavior. The final release documentation states this limitation explicitly.

The optional update checker contacts the public GitHub API only when enabled or explicitly invoked. It does not silently download or install updates. The single-file launchers embed `WebView2Loader.dll`, but they do not bundle Microsoft's WebView2 Runtime.

## Audit-fix result

The post-audit source now contains mitigations for the reviewed findings: internal-origin validation for WebView2 native commands, local History badges, corrected COM QueryInterface behavior, title-bar button hit targets, directory validation, unique atomic-write temporary paths, stale-data cleanup, History capping, asynchronous clear-data status, asynchronous update checking, tabs-disabled reopen protection, WebView2 initialization failure shutdown, page-content context-menu integration, portable cleanup retry, installer atomic extraction and shortcut diagnostics, per-monitor DPI fallback, and trimmed URL/query normalization.

The refinement is build-valid and conservative: normal cookies remain available for everyday use, private data boundaries are strengthened, and the application does not make absolute anonymity claims. The version remains **v3.0.0**.

The global shortcut focus fix dispatches supported Pane Browser shortcuts from the main Windows message queue before focused controls or WebView2 receive the key message. This removes the requirement to click inside the website first while preserving ordinary text-entry shortcuts for the focused control.
