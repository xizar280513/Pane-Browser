# Pane Browser v4.0.0 — Deep Audit Findings

## Scope

This audit covers the v4.0.0 native Win32/WebView2 source after the profile, authentication-window, permission, workspace, sleeping, recovery, backup, and productivity integrations. The audit is source-level and build-level; it is not a substitute for running the application on Windows with the WebView2 Runtime.

## High-priority findings and disposition

| Area | Finding | Disposition |
|---|---|---|
| Authentication popup | OAuth/Firebase flows can open a blank `about:blank` popup and navigate it later. Dropping blank popups would break this flow before the provider redirect. | Fixed in source: blank popup requests are managed as authentication windows, retain the opener profile, and are connected with `put_NewWindow`. |
| Authentication profile | WebView2 requires the supplied new WebView to use the same profile as its opener. | Implemented: popup metadata copies the opener profile ID/name, privacy state, workspace, and group; controller options are created from the same environment before `put_NewWindow`. This still requires Windows runtime testing with real providers. |
| Redirect/session state | The original v1 single-file extraction model used a temporary UDF; v4 now uses a custom persistent UDF under local app data and profile names under the shared UDF. | v4 design avoids the v1 temporary-profile limitation. The auth provider may still reject embedded redirect flows because of its own origin, cookie, storage-partition, or policy requirements. |
| Site policy timing | Applying JavaScript policy only after `NavigationCompleted` is too late for the new document. | Fixed: v4 applies the saved per-origin script/dialog policy on `NavigationStarting` and reapplies it after commit. |
| WebView2 crash recovery | Recreating controllers while the old browser process or shared UDF is still active can cause locks or repeated failures. | Mitigated: controller references are closed, the environment is released, session metadata is captured, and a bounded 2-second fallback timer recreates the environment. Residual risk: exact process-exit timing remains Windows-runtime dependent. |
| Callback lifetime | Custom COM callback wrappers without virtual destructors produced a compiler warning and made lifecycle review harder. | Fixed for v4-added callback wrappers with virtual destructors. Static syntax audit passes. |
| Private data | Private tabs must not enter app-owned history, session restore, downloads, or profile policy persistence. | Source excludes private tabs from these app-owned stores and denies sensitive permission requests. WebView2 private-profile behavior remains a runtime test item. |
| Backup privacy | Exporting the WebView2 UDF would export cookies, tokens, and site storage, creating an unnecessary privacy risk. | v4 backup exports only app-owned records: history, bookmarks, settings, session, workspace/group metadata, downloads, and notes. It deliberately excludes cookies, tokens, and the WebView2 UDF. |
| Clear browsing data | Clearing app-owned files without resetting policy maps could leave per-site decisions active. | Fixed: Clear Browsing Data clears history/download/session records and resets site-policy and permission maps before invoking WebView2 profile data clearing. Bookmarks and general settings are retained by design. |
| Command routing | Several feature commands used hand-counted string offsets, which can silently drop the first character or make a button no-op. | Fixed and re-audited for Permission, Tab Groups, Restore Session, and Sleep duration commands. |
| Profile identity | Windows and WebView2 profile names are case-insensitive, so `Personal` and `personal` could otherwise collide. | Fixed: profile lookup and registry loading now compare IDs case-insensitively and reject duplicate IDs. |
| Profile deletion | Deleting an active normal profile must not remove Personal/root data, other profile directories, or the shared WebView2 UDF. | Refined: Settings protects Personal; deletion requires explicit confirmation, validates the target ID, requires `ICoreWebView2Profile8::Delete`, closes the active controllers before app-data cleanup, removes only the validated additional-profile directory, updates the registry atomically, and returns to Personal. Runtimes without the official API fail closed. |
| Filtered History selection | The old Select All handler selected every `.pick` checkbox, including rows hidden by the search filter. | Fixed: filtering clears hidden-row selections and Select All/Deselect All operate on `visibleRows()` only; searching `youtube` then selecting all cannot select hidden `google.com` rows. |
| Complete data deletion | Removing the entire app-data root while WebView2 is alive could fail, race with autosave, or target an unintended path. | Refined: the Settings action requires two confirmations, verifies the exact Local AppData/fallback root, starts a same-executable cleanup worker, closes controllers and environment, suppresses shutdown autosave, exits, and lets the worker delete only after the parent process terminates. Custom external download folders are not targeted. |
| Split View deletion | Removing a tab could leave `g_splitSecondTab` pointing at the wrong tab or an invalid index. | Fixed: Split View is disabled when either panel is closed and the secondary index is adjusted when earlier tabs are removed. |
| Build reproducibility | PE timestamps made clean rebuild hashes differ even when source and toolchain were unchanged. | Fixed: build links use `--no-insert-timestamp`; clean ZIP rebuild hashes now match core, portable, and setup outputs exactly. |

## Medium-priority findings

| Area | Finding | Current state |
|---|---|---|
| Guest mode | The current UI exposes Guest Session through the existing private-tab path rather than a separate durable guest profile. | Safe fallback, but not yet a full independently named guest-profile lifecycle. This must be documented or completed before claiming full profile parity. |
| Smart Address Bar | v4 resolves exact bookmark/history matches before normal URL/search normalization. | Implemented as local resolution; a visual suggestion dropdown is not yet present. |
| Tab Search | Workspace includes client-side filtering over tab rows. | Implemented; it does not yet provide a dedicated native omnibox dropdown. |
| Per-site permission UI | The Site Data Center stores decisions for camera, microphone, location, notifications, and clipboard read. | Implemented with default-deny for sensitive permissions. The list is intentionally limited to SDK-supported, high-impact capabilities. |
| Split View | Two normal tabs are shown side-by-side; private/authentication tabs are not automatically included. | Implemented; closing or reordering tabs while split view is active requires real UI testing. |

## Low-priority findings and warnings

The compiler warning audit succeeds with status 0. Remaining warnings are baseline or toolchain-related: the Microsoft header contains an MSVC-only `#pragma warning`, and the source dynamically resolves Win32 APIs through `GetProcAddress`, which GCC reports as an incompatible function-pointer cast. These casts are intentional and guarded by null checks. No new v4 logic warning remains for callback shadowing or non-virtual callback destruction.

## Security and privacy boundaries

Pane Browser v4 stores app-owned data locally under the active profile directory. WebView2 itself stores browser data such as cookies, DOM storage, cache, permissions, and other profile data in the configured UDF. The application does not export those values through its backup feature. Authentication is not implemented by copying credentials or intercepting provider tokens; it relies on the provider's normal WebView2-compatible flow.

Per-site origin keys are derived from the URI scheme and authority and are lower-cased. Policy files contain encoded origin and policy values, not cookie or token values. Internal-page commands continue to be accepted only through the existing trusted internal-page message path.

## Authentication-specific conclusion

The `missing initial state` error is plausibly reduced by v4's persistent custom UDF and same-profile popup handling, especially for providers that use redirect state in browser storage. It cannot be guaranteed to disappear for every provider. Firebase, OAuth, OIDC, and SAML integrations can still fail when the site uses a different `authDomain`, cross-origin helper, blocked third-party storage, partitioned storage, enterprise policy, or an authentication flow that requires the system browser. A real Windows test matrix is required before claiming universal authentication compatibility.

## Required Windows test matrix before release

| Test | Expected result |
|---|---|
| Fresh normal profile | UDF and profile directories are created in a writable local-app-data location. |
| Two normal profiles | Cookies, login, history, bookmarks, settings, and permissions remain isolated. |
| Private tab | No app-owned history/session/download persistence; closing the tab removes its visible state. |
| Guest Session | No unintended reuse of normal profile cookies or app-owned records. |
| Firebase redirect sign-in | Redirect returns to the original site without missing initial state. |
| OAuth/OIDC popup | Blank popup is retained, uses opener profile, receives the provider redirect, and closes cleanly. |
| SAML login | IDP-initiated and SP-initiated flows are tested separately. |
| WebView2 process failure | Browser process recovery recreates tabs once, without duplicate tabs or infinite recovery loops. |
| Site Data Center | Per-origin script/dialog/permission decisions apply after reload and remain profile-scoped. |
| Clear Browsing Data | WebView2 data is cleared where supported; bookmarks/settings are retained. |
| Delete Profile | Confirmed non-Personal active profile is deleted; Personal, other profiles, shared UDF, and unsafe IDs remain protected. Unsupported WebView2 profile-delete capability must show a refusal with no deletion. |
| History filter + Select All | Search `youtube` with both Google and YouTube history present, then Select All. Only visible YouTube rows are checked and the selection count excludes hidden Google rows. |
| Delete All Data | Confirm twice; the app exits and the exact Pane Browser root is removed, including profiles, app records, WebView2 UDF, cookies, cache, and website data. No new settings or backup is written afterward. |
| Backup/restore | App-owned data restores correctly and the archive contains no WebView2 cookies/tokens. |
| DPI/theme/layout | Light/dark mode, custom title bar, vertical tabs, split view, and narrow windows remain usable. |

## References

[1]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2controlleroptions?view=webview2-1.0.4129.50 "Microsoft Learn — ICoreWebView2ControllerOptions"
[2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2newwindowrequestedeventargs?view=webview2-1.0.4129.50 "Microsoft Learn — ICoreWebView2NewWindowRequestedEventArgs"
[3]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder "Microsoft Learn — Manage user data folders"
[4]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/clear-browsing-data "Microsoft Learn — Clear browsing data from the user data folder"

The official documentation states that a WebView2 UDF stores browser data such as cookies, permissions, and cached resources [3], and that a new WebView supplied through `NewWindowRequested` must use the same profile as the opener [2]. The Clear Browsing Data API operates on the profile's UDF [4].

**Audit status:** source, warning, package, deterministic clean-rebuild, and targeted profile/history/complete-deletion regression validation pass; Windows runtime validation remains required before release.

**Author:** Manus AI
