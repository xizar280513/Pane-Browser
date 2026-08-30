# Pane Browser v4.0.0

Pane Browser is an independent native Windows browser shell built with C++17, Win32, and Microsoft WebView2. Native controls provide the window chrome, toolbar, tabs, menus, and productivity surfaces, while web content is rendered by the installed WebView2 Runtime.

Version 4.0.0 focuses on **comfortable daily browsing with stronger profile isolation, more reliable authentication windows, recovery from WebView2 failures, and local-first data controls**. It extends the v3 navigation model without claiming to replace the underlying web identity provider or browser engine.

## Highlights

### Authentication and profiles

The Authentication Compatibility Layer preserves redirect and popup flows for providers that support embedded WebView2 authentication. OAuth, OIDC, Firebase, and SAML-style flows can use managed authentication windows, including blank `about:blank` popups that later navigate to a provider. Authentication popups keep the same WebView2 environment and profile as the opener, which is required for popup state and cookies to remain coherent.

The Profile Manager supports separate normal profiles such as Personal, Work, and School. Each normal profile has its own app-owned History, Bookmarks, Downloads, Notes, Session, site policies, settings, and WebView2 profile data. The active profile can be deleted from Settings after an explicit warning; deletion removes its app-owned directory and calls the official WebView2 profile deletion API when the installed Runtime supports it. The Personal profile is protected, shared/root data and other profiles are not targeted, and older Runtimes refuse the operation rather than deleting data unsafely. Settings also provides **Delete All Data**, with two confirmations, which closes Pane Browser and permanently removes the entire validated `%LOCALAPPDATA%\\Pane Browser\\` tree, including all profiles, app records, cookies, cache, WebView2 website data, and local sign-in state. It deliberately does not delete files stored outside that tree, such as downloads saved to a custom external folder. A Guest Session uses the privacy-oriented private-tab path and is not intended to become a durable normal profile. Private tabs are isolated and excluded from normal History, session restore, downloads persistence, and app-owned policy persistence.

Authentication remains provider-dependent. Pane Browser does not copy passwords, intercept tokens, or create Google accounts. A provider may still refuse embedded authentication because of its own redirect-domain configuration, cross-origin storage requirements, enterprise policy, storage partitioning, or a system-browser-only policy.

### Tabs and workspaces

Pane Browser supports optional multi-tab browsing, close and reopen actions, tab groups, workspace metadata, vertical tabs, tab search, split view for two normal tabs, and session restore. Workspace and group metadata are saved with normal session records. Private and authentication windows are not restored as normal tabs.

Inactive normal tabs can use WebView2 `TrySuspend`/resume behavior when Tab Sleeping is enabled. The active tab, private tabs, authentication windows, and already sleeping tabs are excluded from the sleeping pass. A WebView2 process failure is shown as a crashed tab state and schedules controlled environment recreation with a bounded fallback timer.

### Privacy and site controls

The Privacy Dashboard offers Balanced, Strict, and Custom modes. Balanced and Strict request the corresponding WebView2 tracking-prevention level where supported by the installed Runtime; Custom retains a conservative balanced floor while per-site controls are applied separately.

The Site Data Center shows the active origin and provides profile-scoped controls for JavaScript, script dialogs, camera, microphone, location, notifications, and clipboard read. Sensitive permissions default to deny unless the user explicitly stores an allow decision for that origin. Cookie values and authentication tokens are never displayed by the Site Data Center.

### Productivity

The Command Palette provides filtered access to navigation, profiles, privacy, workspace, notes, recovery, backup, and restore actions. The Smart Address Bar resolves exact bookmark and recent-history matches locally before applying normal URL or search-engine handling. Reading Mode, Quick Notes, Split View, Workspace, and Tab Search are available from the Command Palette and the native context menu.

### Local backup and restore

Backup exports app-owned records for the active profile: History, Bookmarks, Settings, Session, workspace/group metadata, Downloads, and Quick Notes. It intentionally does **not** export the WebView2 User Data Folder, cookies, passwords, authentication tokens, or site storage. Restore imports only those app-owned records into the active profile.

## Keyboard shortcuts

| Shortcut | Action |
|---|---|
| `Ctrl+L` | Focus the address bar |
| `Ctrl+T` | Open a new tab when tabs are enabled |
| `Ctrl+W` | Close the active tab |
| `Ctrl+Shift+T` | Reopen the most recently closed normal tab |
| `Ctrl+Tab` / `Ctrl+Shift+Tab` | Move between tabs |
| `Ctrl+R` or `F5` | Refresh the active page |
| `Ctrl+F` | Find in page |
| `Ctrl+D` | Add the current page to Bookmarks |
| `Ctrl+J` | Open Downloads |
| `Ctrl+H` | Open History |
| `Ctrl+Shift+P` | Open a private tab |
| `Ctrl+Shift+Space` | Open Command Palette |
| `Ctrl++` / `Ctrl+-` | Zoom in or out |
| `Ctrl+0` | Reset zoom |

Shortcuts are dispatched from the native message loop so they do not depend on first clicking inside the web page. A website's own JavaScript may still consume a shortcut in content-specific circumstances.

## Local data layout

Normal app-owned data is stored under `%LOCALAPPDATA%\\Pane Browser\\`. The Personal profile keeps the v3-compatible root layout. Additional profiles are stored under `profiles\\<profile-id>\\`. WebView2 browser data is stored in the shared `WebView2\\` User Data Folder and is separated by WebView2 profile name.

```text
%LOCALAPPDATA%\\Pane Browser\\
├── history.pbh                 # Personal profile compatibility file
├── bookmarks.pbb
├── settings.pbs
├── session.pbs
├── session-meta.pbv            # Workspace/group metadata
├── downloads.pbd
├── quick-note.pbn
├── site-policies.pbp
├── permissions.pbp
├── profiles.pbr                # Normal profile registry
├── profiles\\<profile-id>\\...
└── WebView2\\                  # Cookies, DOM storage, cache, and site data
```

Writes use process-specific temporary files followed by replacement to reduce partial-record risk. Profile IDs are sanitized and validated before being used as directory names. A profile switch closes the existing controllers before loading another profile's app-owned data. Deleting a profile only removes its validated `profiles\\<profile-id>\\` directory; the Personal/root directory and shared `WebView2\\` UDF are protected from recursive app cleanup. Delete All Data uses a separate deferred helper process: the main process releases WebView2 and exits without autosaving, then the helper verifies the exact app-owned root and removes it after the parent process terminates. In History, filtering also clears selections from hidden rows, and Select All applies only to visible filtered results.

## Distribution

`PaneBrowser-v4.0.0-portable.exe` is intended to be the normal portable download. It is a single self-extracting executable containing the browser core and `WebView2Loader.dll`; it does not require users to carry the loader DLL beside it. The launcher extracts its payload to a temporary directory and the browser itself stores its persistent profile data under local app data.

`PaneBrowser-v4.0.0-setup.exe` is the per-user installer with upgrade and uninstall support. It does not install the Microsoft WebView2 Runtime.

Both distributions require the Microsoft Edge WebView2 Runtime to be installed on Windows x64. The runtime is an external platform prerequisite, not a component embedded into Pane Browser.

## Building from source

The source package includes the minimum WebView2 SDK files required by the build. A MinGW-w64 x64 toolchain and GNU `windres` are required.

```bash
./build_v4.sh
```

The build creates the following generated files in the project directory:

```text
PaneBrowserCore.exe
PaneBrowser-Portable.exe
PaneBrowser-Setup.exe
WebView2Loader.dll
```

For distribution, provide the portable or setup launcher rather than the core executable alone.

## Project files

| File | Purpose |
|---|---|
| `PaneBrowser.cpp` | Native window, WebView2 environment, profiles, tabs, auth popups, permissions, privacy, recovery, productivity pages, backup, and settings |
| `v4_data.h` | Profile registry, path validation, atomic app-data files, profile layout, and safe cleanup helpers |
| `v3_data.h` | Backward-compatible local records, bookmarks, downloads, session, and settings formats |
| `single_portable_launcher.cpp` | Single-file portable self-extractor |
| `single_installer.cpp` | Per-user installer, upgrade path, shortcuts, and uninstaller mode |
| `build_v4.sh` | Reproducible MinGW-w64 build script |
| `PaneBrowser.rc` | Browser icon resources |
| `portable_launcher.rc` | Embedded portable payload resources |
| `installer.rc` | Embedded installer payload resources |
| `PaneBrowser.ico` | Multi-resolution application icon |
| `resource_v2.h` | Shared resource identifiers |
| `EventToken.h` | WebView2 event-token dependency for the included SDK setup |
| `v4-deep-audit-findings.md` | Detailed source-level audit, residual risks, and Windows test matrix |
| `LICENSE` | MIT License |

## Verification status

The source currently passes the MinGW-w64 syntax build and the normal v4 build. Core, portable, and setup targets are PE32+ Windows x64 GUI executables. The source includes static checks for callback lifetime, popup profile matching, private-data exclusion, per-site policy timing, profile-scoped persistence, recovery recreation, backup boundaries, case-insensitive profile identity, safe profile deletion guards, complete-root deletion guards, deferred shutdown cleanup, and filtered History selection.

A real Windows x64 test remains mandatory before calling the release production-ready. The required matrix includes Firebase redirect login, OAuth/OIDC popup login, SAML flows, cookies across restarts, profile isolation, Guest Session cleanup, WebView2 process failure, tab sleeping/resume, permissions, split view, vertical tabs, DPI/theme changes, backup/restore, installer upgrade, and uninstall behavior.

## License

Pane Browser is distributed under the MIT License. See `LICENSE`.
