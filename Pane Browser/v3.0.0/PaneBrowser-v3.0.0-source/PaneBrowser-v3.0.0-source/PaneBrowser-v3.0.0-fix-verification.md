# Pane Browser v3.0.0 — Post-Audit Fix Verification

## Scope

This document records the v3.0.0 refinement after the deep audit. The version remains v3.0.0; these are security, privacy, reliability, and compatibility fixes rather than a new feature release.

## Mitigations

| Finding area | Mitigation status |
|---|---|
| WebView2 native command boundary | Web messages are accepted only from Pane Browser internal `about:blank` pages; external pages are rejected. |
| History privacy | External Google favicon requests were removed; History uses a local hostname badge. |
| COM callbacks | Callback `QueryInterface` implementations validate requested IIDs and return `E_NOINTERFACE` when unsupported. |
| Custom title bar | Title-bar control region is treated as client area before resize-edge hit testing. |
| Persistence paths | Existing paths are verified as directories; fallback paths fail closed when not writable. |
| Atomic data writes | Process-specific temporary names are used; failed temporary files are deleted; replacement uses `MoveFileExW` write-through. |
| Stale local data | Disabling persistent History or session restore removes the corresponding stale record file. |
| History growth | Application History is capped at 10,000 records. |
| Clear browsing data | Destructive action requires confirmation and reports asynchronous WebView2 completion or partial failure. |
| Update checker | Network access runs on a worker thread with a five-second WinINet timeout and UI result message. |
| Tabs disabled | Reopen-closed-tab refuses to create a hidden tab when tab support is disabled. |
| WebView2 startup | Initialization failure now closes the window and exits instead of leaving a blank process running. |
| Context menu | WebView2 page-content context menu event is connected to the native Pane Browser menu. |
| Portable cleanup | Temporary extraction cleanup retries after WebView2 child processes have time to release files. |
| Installer upgrade | Core and loader extraction uses atomic replacement; shortcut and directory failures are reported. |
| Input routing | Address input is trimmed; dotted phrases containing whitespace remain searches instead of becoming invalid URLs. |
| DPI/device compatibility | Per-monitor DPI awareness V2 is requested with a legacy fallback. |

## Verification performed

The core, portable launcher, and installer pass MinGW-w64 C++17 strict syntax checks with application warnings treated as errors. The complete build produces PE32+ Windows x64 GUI executables. Resource inspection confirms that the portable and setup launchers contain the browser core and `WebView2Loader.dll` payloads. A post-fix invariant scan passes 18/18 checks covering security boundary, privacy, persistence, launcher, installer, lifecycle, DPI, and UI wiring.

## Remaining Windows test requirement

The Linux build environment cannot run the Microsoft WebView2 Runtime or Windows shell. A real Windows x64 test remains required for Chromium rendering, origin rejection at runtime, normal cookie persistence, private controller isolation, tracking prevention behavior, downloads, title-bar mouse behavior, page context menus, DPI scaling, shortcut display, upgrade, uninstall, and temporary cleanup. The build does not claim that static verification replaces those tests.

## Privacy boundary

Normal WebView2 cookies and site data remain persistent for convenience. Private tabs remain isolated and are excluded from normal History, session restore, and persistent download records. Pane Browser does not upload its own History database to a Pane Browser server. This is local privacy hardening, not a guarantee of anonymity against websites, network operators, Windows administrators, or other software with access to the device.
