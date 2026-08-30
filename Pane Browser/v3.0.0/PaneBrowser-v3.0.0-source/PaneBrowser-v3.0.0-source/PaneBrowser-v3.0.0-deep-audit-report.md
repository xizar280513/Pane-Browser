# Pane Browser v3.0.0 — Deep Audit Report

## Audit scope and method

This audit reviewed the latest v3.0.0 source, data layer, build script, resource scripts, portable launcher, installer, source package, and final PE artifacts. It included strict MinGW-w64 compilation with application warnings treated as errors, source-pattern/invariant checks, WebView2 API review, persistence review, and packaging inspection. No source changes were made as part of this audit request.

The strict syntax audit passed for `PaneBrowser.cpp`, `single_portable_launcher.cpp`, and `single_installer.cpp`. The build outputs identify as PE32+ Windows x64 GUI executables. The portable and setup launchers contain large resource sections and embedded PE signatures. This proves source/build consistency, not full Windows runtime correctness.

## Executive summary

The v3.0.0 architecture has several good foundations: normal WebView2 data is kept in a writable per-user profile, normal cookies can persist, private-controller creation does not silently fall back to normal mode, application History and session restore exclude private tabs, and private download records are filtered from persistent downloads.

However, the audit found one **high-severity security issue** and several **medium-severity stability, privacy, and data-integrity issues**. The highest-priority issue is the WebView2 native/web message boundary: `HandleHistoryMessage` accepts command strings without validating the source document origin. Microsoft explicitly recommends validating the WebView2 message source before consuming web messages [1] [2]. Because normal external websites can use `chrome.webview.postMessage`, a malicious or compromised page may be able to invoke Pane Browser's native commands.

## Findings by severity

| ID | Severity | Area | Finding | Confidence |
|---|---|---|---|---|
| S-01 | High | WebView2 security boundary | `WebMessageReceived` commands are accepted without checking `args->get_Source()` against an internal-page origin. External pages may potentially trigger History deletion, clear browsing data, settings changes, navigation, bookmark actions, private-tab creation, and download shell actions. | Confirmed statically; runtime exploitability should be reproduced on Windows |
| S-02 | High/Medium | Privacy | History renders a Google favicon URL containing each visited host: `https://www.google.com/s2/favicons?domain=...`. This can disclose visited hostnames to Google whenever the History page loads. | Confirmed statically |
| S-03 | Medium | Custom title bar | `WM_NCHITTEST` returns `HTCAPTION` for the whole top title area, including the painted minimize/maximize/close glyph region. Windows may therefore route clicks as non-client caption clicks instead of `WM_LBUTTONDOWN`; the custom buttons may not respond reliably. | Likely from Win32 message flow; requires Windows confirmation |
| S-04 | Medium | Persistence | `EnsureDirectory` treats `ERROR_ALREADY_EXISTS` as success without checking that the path is actually a directory. If `%LOCALAPPDATA%\\Pane Browser` or the download path is a file, writes/profile initialization can fail silently. | Confirmed statically |
| S-05 | Medium | Data integrity | `WriteLines` uses a fixed `.tmp` path and callers ignore the boolean result. Concurrent instances can race on temporary files, and failed writes are not reported to the user. | Confirmed statically |
| S-06 | Medium | Privacy semantics | Disabling Persistent History does not remove an existing `history.pbh`; disabling session restore does not remove an existing `session.pbs`. Old data remains on disk even though future loading is skipped. | Confirmed statically |
| S-07 | Medium | Clear data UX | `ClearBrowsingDataAll(nullptr)` is asynchronous, but the code ignores its result and immediately displays a success message. There is also no confirmation before cookies, site data, and active sign-ins are cleared. | Confirmed statically |
| S-08 | Medium | Portable cleanup | The portable launcher waits for the core process but not necessarily all WebView2 child processes before deleting the temporary extraction folder. Cleanup can fail and leave files behind. Microsoft documents the need to wait for browser processes before deleting a UDF [3]. | Confirmed statically/documented risk |
| S-09 | Medium | Installer | The installer ignores shortcut creation failures and runs the uninstall script asynchronously, then immediately reports success. Upgrade extraction is not transactional; a partial failure can leave a mixed installation. | Confirmed statically |
| S-10 | Medium | Startup stability | The optional update checker performs synchronous WinINet I/O on the UI thread from a timer. A slow network can freeze the window for up to the configured timeout. | Confirmed statically |
| S-11 | Medium | Optional tabs | `Ctrl+Shift+T` calls `ReopenClosedTab` without checking `tabsEnabled`. In tabs-disabled mode it can create a hidden additional tab, contradicting the setting. | Confirmed statically |
| S-12 | Medium | Crash/lifecycle | If asynchronous WebView2 environment creation fails, the app shows an error but keeps the main window/message loop alive without a usable browser. | Confirmed statically |
| S-13 | Medium | COM correctness | Callback `QueryInterface` implementations ignore `riid` and return their callback interface for arbitrary requested IIDs. This is nonconforming COM behavior and can cause fragile runtime interactions. | Confirmed statically |
| S-14 | Medium/Low | Privacy | History and session files store complete URLs, including query strings that can contain sensitive search terms or tokens. This is browser-like behavior but should be documented and optionally redacted in a future privacy control. | Confirmed statically/design risk |
| S-15 | Low/Medium | Persistence growth | History is unbounded and each normal navigation writes synchronously to disk on the UI path. Long-running use can increase file size and cause UI latency. | Confirmed statically/design risk |
| S-16 | Low/Medium | Download reliability | Filename/path collision checks are separate from creation, so simultaneous downloads can race. Some URI-derived names may still be problematic on Windows because trailing dots/spaces are not fully normalized. | Confirmed statically |
| S-17 | Low/Medium | Context menu | Native `WM_CONTEXTMENU` handling is installed on the host window. WebView2 web content may receive its own context menu, so the advertised native context menu may not appear consistently over page content. | Likely; requires Windows confirmation |
| S-18 | Low/Medium | Input behavior | `NormalizeInput` treats any text containing a period as a URL. Queries such as `version 3.0` may be converted into an invalid `https://` navigation instead of a search. | Confirmed statically |

## S-01 — WebMessageReceived trust-boundary issue

The most important issue is the callback chain:

```text
WebView2 page -> chrome.webview.postMessage(...) -> add_WebMessageReceived -> HandleHistoryMessage
```

`HandleHistoryMessage` retrieves the string message but does not retrieve or validate the sender source. The handler contains commands such as `clear-data`, `delete-selected:...`, `settings-titlebar:...`, `settings-custom-search:...`, `bookmark-delete:...`, `download-open:...`, and navigation commands. These commands were designed for internal HTML pages, but the same WebView2 control also navigates to arbitrary external websites.

Microsoft's security guidance says to treat all web content as insecure, check the origin of the document, and validate web messages before consuming them [1]. Microsoft's WebView2 messaging sample explicitly checks `args->get_Source()` and rejects messages from an unexpected source [2]. Therefore this is not merely a theoretical code-style concern; it is a missing security boundary.

The most concerning consequence is `download-open:`. A page could potentially start a download and then send a native command that invokes `ShellExecuteW` on the recorded result path. The `clear-data` command is also destructive and currently has no confirmation. This finding should be fixed before describing the build as hardened for hostile web content.

## S-02 — Google favicon privacy leak

The History page constructs an image URL like this:

```text
https://www.google.com/s2/favicons?domain=<visited-host>
```

When the History page loads, the browser may request that URL for each history row. This can disclose visited hostnames to Google. It is separate from the app's own History upload behavior, but it conflicts with a strong privacy-oriented user expectation. A local fallback icon, a generated text/domain badge, or no favicon request would avoid the leak.

## Positive privacy findings

Normal WebView2 data uses `%LOCALAPPDATA%\\Pane Browser\\WebView2` and is intentionally preserved, so ordinary cookies/site data can support everyday sign-in convenience. Private tabs attempt `IsInPrivateModeEnabled=TRUE` through controller options. If private controller creation fails, the source removes the failed tab instead of silently making it normal. Private tabs are excluded from application History, session restore, closed-tab restore, and persistent download records.

These findings are consistent with Microsoft's documentation that a WebView2 user-data folder stores cookies and additional browser data, and that profiles separate cookie values and profile data [3] [4]. They do not eliminate the S-01 message-boundary issue or the S-02 favicon leak.

## Installer and portable conclusions

The single-file concept is structurally valid: the launcher resources contain the core and loader, and the outputs are PE32+ x64. The portable launcher cleanup is explicitly best-effort, which is acceptable only if documentation does not promise that every temporary file is always deleted. The installer is a per-user installer with upgrade and uninstall support, but it is not transactional and ignores some shell-operation failures. It should be described as a lightweight per-user installer, not as a fully transactional enterprise installer.

## Recommended priority order

The next fix cycle should first validate the source origin and restrict native commands to internal pages. It should remove the external Google favicon request. It should then fix custom title-bar hit testing, path-vs-file checks, persistence error reporting, async clear-data completion, synchronous update-check networking, and the tabs-disabled `Ctrl+Shift+T` behavior. COM `QueryInterface` correctness and launcher/process-tree cleanup should follow.

These changes can remain a **v3.0.0 refinement**; they do not require extensions, DNS changes, cloud sync, or Google-account integration.

## Testing still required on Windows

Static checks cannot confirm several runtime behaviors. On Windows x64 with WebView2 Runtime installed, test normal cookie persistence by signing into a test site and restarting Pane Browser; verify that private tabs cannot see normal cookies; inspect network requests while opening History to confirm no visited-host favicon requests; attempt a benign `chrome.webview.postMessage` from an external page and verify that native commands are rejected; test custom title-bar minimize/maximize/close buttons and border resizing; test two simultaneous app launches; test unwritable/data-path-conflict scenarios; test portable cleanup after downloads; test installer upgrade/uninstall; and test all shortcuts and context-menu positions at multiple DPI scales.

## References

[1]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/security "Microsoft Learn — Develop secure WebView2 apps"
[2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/how-to/communicate-btwn-web-native "Microsoft Learn — Interop of native and web code"
[3]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder "Microsoft Learn — Manage user data folders"
[4]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/winrt/microsoft_web_webview2_core/corewebview2profile "Microsoft Learn — CoreWebView2Profile Class"
