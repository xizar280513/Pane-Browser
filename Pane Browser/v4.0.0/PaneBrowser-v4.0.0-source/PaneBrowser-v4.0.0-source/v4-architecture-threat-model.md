# Pane Browser v4.0.0 — Architecture and Threat Model

## Status

Dokumen ini adalah hasil perancangan sebelum implementasi v4.0.0. Tujuannya adalah mencegah refactor besar merusak cookies, login, private mode, shortcuts, session restore, persistence, dan launcher v3. Belum ada source v4 atau binary v4 yang dibuat.

## Kesimpulan arsitektur utama

Pane Browser v4 perlu memisahkan empat lapisan yang pada v3 masih sangat global:

| Lapisan | Tanggung jawab | Aturan utama |
|---|---|---|
| **Application shell** | Window utama, toolbar, tab strip, command palette, menus, theme, DPI, global shortcuts | Tidak menyimpan cookies/token atau memutuskan policy situs secara langsung. |
| **Profile and policy manager** | Profile normal, Guest, Private, UDF, per-site policy, permission state, privacy mode | Setiap tab selalu menunjuk ke satu profile context yang immutable selama controller hidup. |
| **WebView session manager** | Environment, controller, popup, tab lifecycle, sleeping, crash recovery, event tokens | Semua callback memiliki generation/lifecycle guard agar tidak memakai object yang sudah ditutup. |
| **Application data store** | History, bookmarks, downloads, workspaces, groups, notes, settings, recovery journal, backups | Data app terpisah dari WebView2 UDF; semua write atomic, versioned, validated, dan profile-scoped. |

WebView2 mendukung banyak profile di bawah satu UDF, dengan folder terpisah untuk cookies, preferences, cache, dan data profile; controller dapat dibuat dengan `ProfileName` dan `IsInPrivateModeEnabled`.[1] Karena itu, profile normal v4 sebaiknya menggunakan satu UDF persistent dengan profile names, bukan membuat satu UDF baru untuk setiap tab.

## Profile model

### Normal profiles

Profile normal adalah profile persistent seperti `Personal`, `Work`, atau `School`. Directory layout yang direncanakan:

```text
%LOCALAPPDATA%\\Pane Browser\\
├── app-settings.json
├── profiles\\
│   ├── personal\\
│   │   ├── profile.json
│   │   ├── app-data\\
│   │   ├── workspaces\\
│   │   ├── notes\\
│   │   └── backups\\
│   └── work\\
└── WebView2\\
    └── Profiles\\...
```

WebView2 cookies, DOM storage, cache, permissions, and site data berada di UDF WebView2, sedangkan History, bookmarks, downloads, workspace, dan notes milik Pane Browser berada pada app-data profile yang bersesuaian. Pemisahan ini penting agar Site Data Center dan Backup tidak keliru menganggap `history.pbh` sebagai seluruh data browser.[2]

### Guest

Guest adalah profile sementara yang tidak disimpan sebagai profile normal. App-owned data dan WebView2 data harus berada pada temporary root yang dibuat per sesi. Guest tidak boleh menulis History, bookmarks, downloads record, workspace, atau notes ke profile normal. Cleanup hanya boleh dilakukan setelah seluruh WebView2 controls ditutup dan browser process sudah selesai; WebView2 mendokumentasikan bahwa UDF tidak boleh dihapus ketika session masih aktif.[3]

### Private

Private tab tetap memakai controller dengan `IsInPrivateModeEnabled=TRUE` bila runtime mendukung. V4 harus fail closed: jika private controller tidak dapat dibuat, jangan membuat controller normal sebagai fallback. Private tabs tidak boleh masuk ke History, session restore, closed-tab stack, workspace persistence, backup, or normal download records.

## Authentication Compatibility Layer

V4 tidak boleh menjanjikan bahwa semua provider dapat dipaksa bekerja di embedded WebView2. Microsoft mendokumentasikan bahwa Google Authentication dinonaktifkan pada embedded webviews, termasuk WebView2, karena kebijakan keamanan Google.[4] Maka lapisan autentikasi harus memiliki tiga jalur yang jelas:

| Flow | Perilaku v4 |
|---|---|
| Redirect di dalam WebView2 | Pertahankan tab, profile, storage context, dan navigation chain; jangan melakukan `NavigateToString` atau reload tidak perlu selama callback. |
| Popup yang diizinkan provider | Buat child WebView/window dari environment dan profile yang sama; `NewWindowRequested` harus menggunakan `put_NewWindow`, bukan hanya menandai `Handled` lalu menavigasi sender. WebView2 mensyaratkan target WebView yang dipasang sebagai new window berada pada environment dan profile yang sama.[5] |
| Provider yang melarang embedded webview | Tampilkan diagnostics dan gunakan provider/system-browser fallback yang policy-compliant jika provider mendukungnya. Jangan mencoba mengakali kebijakan provider. |

Firebase `signInWithRedirect()` menggunakan cross-origin helper/iframe dan dapat gagal ketika third-party storage atau storage partitioning memblokir akses. Firebase merekomendasikan same-origin/custom auth domain, proxy/self-hosted helper, popup, atau provider-independent credential flow.[6] V4 dapat memperbaiki host-side lifecycle dan popup handling, tetapi konfigurasi `authDomain`, helper origin, dan policy provider tetap menjadi tanggung jawab situs.

Auth diagnostics harus mencatat secara lokal metadata non-sensitive seperti flow type, start origin, callback origin, navigation sequence, storage availability result, and WebView2 runtime version. Jangan mencatat access token, refresh token, password, cookie value, authorization code, atau full sensitive query string ke History/log.

## Popup and Authentication Window Manager

`NewWindowRequested` harus dibagi menjadi tiga kelas: ordinary target-blank, user popup, dan authentication popup. Untuk setiap request, manager perlu memeriksa source frame/origin, `IsUserInitiated`, target URI, window features, dan apakah opener masih hidup. Event deferral digunakan ketika pembuatan child window asynchronous; setelah child WebView siap, target dipasang dengan `put_NewWindow`.[5]

Popup policy default:

- user-initiated ordinary links: open as tab, unless user selects separate window;
- authentication popup: open as a managed temporary auth window using the opener's same normal profile;
- non-user popup: block or show a quiet notification by default;
- private opener: popup must remain private and cannot attach to normal profile;
- popup close request: close only the child window, never the opener tab;
- popup callback navigation: do not record auth helper URLs or authorization codes in app History.

## Tab, group, workspace, and split-view model

A `TabModel` should own URL/title/active state, profile ID, privacy mode, group ID, workspace ID, WebView controller, event registration tokens, current generation, sleeping state, and recovery state. A `WorkspaceModel` should own normal tab descriptors, group descriptors, active tab, layout mode, and per-workspace UI preferences. Private tabs must never be serialized into these models except as in-memory temporary descriptors.

Split view should use two normal `TabModel` instances sharing the same profile/environment but separate controllers. It must have explicit focus ownership, independent navigation history, and a close/merge operation. It must not duplicate or move a controller while callbacks are pending.

Vertical tabs and tab search are shell-only features. They should query the model rather than inspect WebView content, and they must remain usable when a tab is sleeping, crashed, or displaying an internal page.

## Crash recovery and sleeping

WebView2 documents that `ProcessFailed` is raised for process failures and that the environment raises `BrowserProcessExited` when the main browser process exits. A renderer failure may be recovered by reload/recreate; a browser-process failure affects all controls sharing the environment and requires coordinated recreation.[7]

Recovery design:

1. classify failure as GPU/utility auto-recoverable, renderer recoverable, browser-process fatal-to-session, or unknown;
2. deduplicate repeated events by browser-process ID, tab ID, and generation;
3. freeze navigation and persistence during recovery;
4. snapshot only safe descriptors such as URL, title, workspace/group/profile ID, scroll position if available, and whether the tab was private;
5. recreate controllers only after the correct WebView2 process lifecycle event;
6. restore non-private HTTP(S) tabs and display a recovery notice when content/form state cannot be restored;
7. cap retries to prevent recovery loops and offer “Close tab”, “Reload”, or “Restart profile session”.

Tab sleeping should close or detach the controller only for eligible tabs. Do not sleep active authentication, popup, download, media playback, file upload, WebRTC, unsaved form, or private tabs by default. Waking a sleeping tab restores its profile and URL but must honestly indicate that in-memory DOM/form state may be lost.

## Permission and Site Data Center

`PermissionRequested` should be handled per profile and per origin with a clear Allow once, Allow for session, Allow always, and Block policy. Default should be conservative for camera, microphone, location, notifications, clipboard, and sensors. Permission decisions must be shown with origin and permission type; third-party iframe requests should not be silently attributed to the top-level site.

Site Data Center must distinguish:

| Data | Source | Default v4 behavior |
|---|---|---|
| App History/bookmarks/download records | Pane Browser app store | List, export, delete, and backup according to profile policy. |
| Cookies | WebView2 Profile/CookieManager | Show origin/name/metadata where supported; do not expose cookie values by default. |
| DOM storage/cache | WebView2 profile | Clear through selected WebView2 browsing-data kinds; report runtime capability and failures. |
| Permissions | WebView2 profile/profile settings | List and revoke per origin where supported. |
| Auth state | Cookies, local storage, provider-specific state | Never export secrets by default; warn before profile deletion. |

WebView2's Clear Browsing Data API clears selected data kinds for the profile on which it is called, and can operate on all data or selected types/time ranges.[8] V4 must not claim that clearing Pane Browser History also signs the user out unless the corresponding WebView2 profile data operation succeeds.

## Privacy Dashboard

The privacy policy precedence should be explicit:

```text
Global safety floor
  -> Profile privacy mode
    -> Workspace policy
      -> Per-site policy
        -> Temporary user decision
```

Balanced remains the default. Strict may block or limit more storage/permissions but must warn that modern login flows can fail. Custom should display the exact policy in force. Private and Guest are lifecycle modes, not merely stricter tracking-prevention flags.

## Command Palette and internal command security

All internal pages use a versioned JSON command envelope with a nonce or per-tab channel identifier. The native side validates:

- source URI is an exact Pane Browser internal origin;
- tab generation and channel ID match the active internal document;
- command name is allowlisted;
- arguments are typed, bounded, and length-limited;
- the command is allowed for the current profile/privacy state;
- no command can directly request arbitrary filesystem paths or process execution.

External websites must not be able to post a string that invokes bookmarks, backup, profile switching, clear-data, or native menus.

## Backup/export/import

Default backup must export only app-owned, non-secret data: profile metadata, settings, bookmarks, History if selected, workspaces, groups, notes, and download records without opening executable content. It must not include WebView2 UDF, cookies, refresh tokens, sessionStorage, saved credentials, or authorization codes by default.

Backup format requirements:

- versioned manifest;
- profile ID and human-readable name;
- file list with sizes, limits, and checksums;
- atomic write and temp-file cleanup;
- no absolute paths in exported records unless explicitly marked and sanitized;
- import into a new profile by default, never overwrite silently;
- path traversal and reparse-point checks;
- private/guest data excluded by construction;
- restore preview and conflict policy;
- rollback on any failed validation.

## Main threat model

| Threat | Consequence | Required control |
|---|---|---|
| External website sends internal command | Native action, data deletion, or profile switch | Exact internal-origin validation, nonce/channel, command allowlist. |
| Popup auth loses profile context | Login failure or account mixing | Same environment/profile child WebView via `put_NewWindow`; auth window lifecycle manager. |
| Provider blocks embedded auth | Login impossible despite host correctness | Policy-compliant system-browser/provider fallback and clear diagnostics. |
| Private data enters normal state | Privacy leak through History, restore, backup, or workspace | Type-level private/guest exclusion and tests at every serialization boundary. |
| Profile mix-up | Wrong cookies/account shown | Immutable profile ID on every tab/controller; visible profile badge; no cross-profile controller reuse. |
| Backup leaks secrets | Account takeover | Exclude UDF/secrets by default; encrypted optional export only after explicit design review. |
| Malicious backup/import path | Arbitrary overwrite or code execution | Canonical path validation, no executable extraction, size/count limits, atomic staging. |
| WebView2 browser process crash | All tabs lost or stale callbacks | Process ID/generation coordination, BrowserProcessExited barrier, bounded recovery. |
| Sleeping tab loses unsaved work | User data loss | Eligibility rules, warning/opt-out, snapshot only as best effort. |
| Concurrent app instances use same UDF | Corruption or initialization failure | Single-instance/profile lock policy or coordinated same-session handling. |
| API missing on old runtime | Feature crash or false success | QueryInterface capability detection and visible fallback states. |
| Large tab/profile count | Memory/CPU exhaustion | Limits, sleeping policy, per-profile budgets, backpressure for backup and UI. |

## Implementation gate before release

V4 must not be called stable until it passes static tests for ownership, origin validation, profile separation, private exclusion, backup path safety, atomic writes, event-token cleanup, recovery generation checks, and runtime capability fallbacks. Real Windows x64 tests with the installed WebView2 Runtime are required for popup authentication, Google/provider policy behavior, cookies, storage, permissions, crash recovery, sleeping, DPI, split view, and installer/portable lifecycle.

## References

[1]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/multi-profile-support "Support multiple profiles under a single user data folder"
[2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/user-data-folder "Manage user data folders"
[3]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/process-related-events "Handling process-related events in WebView2"
[4]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/browser-features "Differences between Microsoft Edge and WebView2"
[5]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2newwindowrequestedeventargs?view=webview2-1.0.4129.50 "ICoreWebView2NewWindowRequestedEventArgs"
[6]: https://firebase.google.com/docs/auth/web/redirect-best-practices "Best practices for signInWithRedirect"
[7]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/process-related-events "Handling process-related events in WebView2"
[8]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/clear-browsing-data "Clear browsing data from the user data folder"
