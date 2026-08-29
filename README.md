# Pane Browser

Pane Browser is an independent, local-first browser shell designed for people who want a direct browsing window without a mandatory online account or a central browsing service. The project uses a native C++ interface and an embedded web engine: Microsoft Edge WebView2 on Windows and GTK/WebKitGTK on Linux.

Pane Browser is organized as two deliberately different editions. The **Regular Edition** is the everyday browser with persistence, profiles, privacy controls, productivity tools, recovery behavior, and a local Password Manager. The **Mini Edition** is a separate minimal browser intended to remain lightweight and private: it has one tab, temporary per-run browser data, and no Regular Edition feature set.

> Pane Browser is an independent implementation. It does not claim to contain, reproduce, or distribute proprietary source code from any unrelated browser project.

## Project principles

| Principle             | Meaning                                                                                                                         |
| --------------------- | ------------------------------------------------------------------------------------------------------------------------------- |
| **Simple**            | Common browsing actions should remain easy to find and quick to use.                                                            |
| **Local**             | Normal application records are kept on the user's device rather than uploaded to a Pane Browser service.                        |
| **Private by design** | Private and Mini workflows avoid the normal persistent application records where the respective edition supports that behavior. |
| **Practical**         | Features are selected for everyday browsing rather than feature count alone.                                                    |
| **Understandable**    | Profiles, deletion, popup handling, recovery, and storage behavior should be visible and predictable.                           |

## v7.0.0 at a glance

Pane Browser v7.0.0 is a stability, privacy, and lightness update over v6. It keeps the same Regular vs Mini separation and the same Windows + Linux packaging layout, while hardening Linux privacy defaults, fixing Linux fullscreen toggle behavior, and reducing Linux binary size. The release does **not** reintroduce DNS Settings; that feature remains outside the project scope.

| Edition             | Windows                                                                | Linux                                                         | Primary role                                        |
| ------------------- | ---------------------------------------------------------------------- | ------------------------------------------------------------- | --------------------------------------------------- |
| **Regular Edition** | WebView2, portable launcher, per-user setup executable, source archive | GTK/WebKitGTK portable binary, Debian package, source archive | Full everyday browsing and local productivity       |
| **Mini Edition**    | Single-file portable executable, source archive                        | Portable binary, source archive                               | Minimal single-tab browsing with temporary run data |

## Regular Edition

The Regular Edition is the full Pane Browser experience. It combines navigation, tabs, persistence, privacy, recovery, profile, and productivity work with the local Password Manager and Linux support.

### Browsing and navigation

The Regular Edition provides a Smart Address Bar, direct URL navigation, Startpage search, final-URL tracking, Back, Forward, Refresh, Home, page loading state, context-menu actions, and ordinary same-tab navigation for user-initiated links. Links that would otherwise request a new browsing context are handled according to the browser's popup and authentication rules rather than unexpectedly opening an unrelated external window.

On Linux, navigation policy blocks `javascript:` and `vbscript:` schemes.

### Tabs and workspaces

Optional tabs, tab groups, workspaces, vertical tabs, Tab Search, split view, tab sleeping, closed-tab reopening, and crash recovery are available in the Regular Edition. Tabs that belong to private or authentication flows are kept outside normal session persistence and recovery records where the security model requires that separation.

### Profiles and local persistence

The Regular Edition supports normal profiles such as Personal, Work, and School, together with guest-style workflows where applicable. Profile data is kept under the user's local Pane Browser application-data area, and the browser uses profile-scoped WebView2 user-data contexts on Windows. Normal cookies and website data remain available for everyday sign-in convenience.

Typical application-owned records include local history, bookmarks, settings, session information, download records, profile metadata, and WebView2 profile data. Exact paths and filenames can vary by platform and build configuration.

### History

History is persistent and local in the Regular Edition when the corresponding setting is enabled. History records preserve complete final URLs rather than displaying only a host name. The History page supports filtering, Select all, Deselect all, selection-aware deletion, and deletion of selected records. Filtering is applied before selection operations so that Select all affects the visible filtered results rather than unrelated hidden entries.

### Bookmarks and downloads

Bookmarks can be added, opened, deleted, imported, and exported locally. The Download Manager displays download activity and keeps non-private download records locally. The user can choose a download folder through Settings where supported by the current build.

### Privacy and data controls

The Regular Edition includes private tabs, Balanced, Strict, and Custom privacy modes, a Permission Center, a Site Data Center, per-site settings, Reading Mode, Split View, Quick Notes, and explicit data deletion controls. Delete Profile protects the Personal profile and applies only to eligible profiles. Delete All Data is a separate destructive action that removes the approved Pane Browser application-data root after confirmation; it does not remove unrelated external download folders or data owned by other applications.

On Linux, WebKit defaults are hardened: page cache, HTML5 local storage, HTML5 database, media stream, WebGL, encrypted media, WebAudio, and JavaScript clipboard access are disabled by default.

Private browsing is not anonymity software. Websites, operating-system components, network operators, crash services, and the embedded web runtime may retain or observe information outside the application's local records.

### Command Palette and shortcuts

The Command Palette provides a searchable route to supported browser actions, including History, Bookmarks, Downloads, private browsing, Settings, Find in Page, Password Manager, and other Regular Edition commands. Supported keyboard shortcuts are handled through native window routing and WebView2 accelerator handling so they are intended to work when focus is in the page, address bar, tab strip, or an internal Pane Browser page.

| Shortcut            | Action                                      |
| ------------------- | ------------------------------------------- |
| `F11`               | Toggle native browser fullscreen.           |
| `Ctrl+L`            | Focus the address bar.                      |
| `Ctrl+T`            | Open a new tab when tabs are enabled.       |
| `Ctrl+W`            | Close the active tab.                       |
| `Ctrl+Shift+T`      | Reopen the most recently closed normal tab. |
| `Ctrl+Tab`          | Move to the next tab.                       |
| `Ctrl+Shift+Tab`    | Move to the previous tab.                   |
| `Ctrl+R` or `F5`    | Reload the active page.                     |
| `Ctrl+F`            | Find in page.                               |
| `Ctrl+D`            | Add the current page to Bookmarks.          |
| `Ctrl+J`            | Open Downloads.                             |
| `Ctrl+H`            | Open History.                               |
| `Ctrl+Shift+P`      | Open a private tab.                         |
| `Ctrl+Shift+Space`  | Open Command Palette.                       |
| `Ctrl++` / `Ctrl+-` | Zoom in or out.                             |
| `Ctrl+0`            | Reset zoom.                                 |

### Password Manager

The Password Manager is available only in the Regular Edition. It supports both manual entry and automatic login detection. A saved entry contains four user-facing fields:

| Field             | Purpose                                                        |
| ----------------- | -------------------------------------------------------------- |
| **Website**       | The HTTPS website origin associated with the credential.       |
| **Password name** | A human-readable label such as Gmail or Work account.          |
| **Username**      | The account identifier used by the website.                    |
| **Password**      | The secret stored by the operating-system credential facility. |

For automatic detection, the browser watches eligible HTTPS login form activity and asks before saving. The prompt provides **Save** and **Do not save** choices. The detector is heuristic and may not recognize every SSO, passkey, iframe, JavaScript-only, or multi-step authentication flow.

Saved passwords are masked by default. The custom inline SVG eye control temporarily reveals a password and hides it again when pressed. Each saved entry can be searched, edited, deleted, or selected for a bulk operation. The selection toolbar exposes **Select all** initially; **Deselect all** and **Delete selected** appear only when at least one entry is checked. When a search filter is active, selection commands apply to the visible matching entries.

On Windows, credentials are stored through Windows Credential Manager. On Linux, credentials are stored through the desktop Secret Service using libsecret. The application does not intentionally write saved password secrets to a plaintext settings file. These facilities still depend on the security of the user's operating-system account, desktop keyring, device, and runtime environment.

Credentials saved under earlier v6 builds remain readable after upgrading to v7.

### Authentication and popup handling

Authentication and blank bootstrap popups are managed separately from ordinary navigation so that OAuth, OIDC, Firebase, SAML, and similar flows have an available child-window path. Ordinary unsolicited popups remain subject to the browser's popup handling rules. Authentication windows are excluded from normal History, Session Restore, closed-tab recovery, persistent site policies, and normal private-data records where applicable.

### Fullscreen and recovery

F11 toggles the native browser fullscreen state. The browser chrome is hidden during fullscreen and restored when fullscreen is exited. On Linux Regular, F11 enter and exit are both supported. HTML video fullscreen is connected to the WebView2 fullscreen event path on Windows and the corresponding web-engine behavior on Linux. Session Restore applies to eligible normal tabs and excludes private and authentication tabs. Crash recovery checks that tabs and controllers still exist before applying recovery updates.

## Mini Edition

The Mini Edition is intentionally not a smaller copy of the Regular Edition. It is a separate minimal/private browser shell with a single tab, Startpage navigation, direct URL navigation, Back, Forward, Refresh, Home, native fullscreen, and HTML video fullscreen.

The Mini Edition intentionally has **no** History, persistent History, bookmarks, Password Manager, DNS feature, Settings page, profile manager, private/incognito mode, downloads manager, theme switcher, tab strip, multi-tab support, Session Restore, workspace tools, extensions, or installer. No Regular Edition feature was added to Mini in v7.

Each Mini run uses a randomly named temporary WebView user-data directory and does not use the normal Regular profile root. The Windows Mini portable build is a single executable with its loader payload embedded; it does not require a separate `WebView2Loader.dll` beside the executable. The Linux Mini build uses the system GTK/WebKitGTK runtime and is delivered as a portable binary plus source package.

On Linux Mini, the same privacy-oriented WebKit defaults as Regular are applied, and the build remains fully ephemeral.

Mini cleanup is best effort. Websites, the operating system, network components, crash services, and the WebView runtime may retain information outside the application's control.

## Linux support

The Linux implementation uses GTK 3 and WebKitGTK 4.1 rather than WebView2. The release packaging is aimed at the Debian/Ubuntu, Fedora/Red Hat, Arch, and SUSE families, but Linux distributions are not interchangeable: package names, WebKitGTK availability, desktop Secret Service providers, graphics stacks, and system integration can differ.

The Linux Regular build uses libsecret for Password Manager integration. A working desktop Secret Service provider is required for normal password storage. If no keyring service is available, the application should not be treated as having successfully persisted a password.

Linux v7 binaries are smaller than v6 (Mini Portable ≈ 34 KB, Regular Portable ≈ 106 KB) and are stripped. Mini no longer links libsecret.

The Linux binaries are dynamically linked against distribution-provided components. Users may need to install the matching GTK, WebKitGTK, JSON-GLib, libsecret, and related runtime packages for their distribution. The portable label refers to the Pane Browser binary packaging; it does not mean that every system library is bundled.

## Distribution

v7.0.0 provides exactly ten logical release assets:

| Asset                    | Intended use                                                        |
| ------------------------ | ------------------------------------------------------------------- |
| Regular Linux Portable   | Run the Linux Regular Edition binary directly.                      |
| Regular Linux Setup      | Install the Linux Regular Edition as a Debian package.              |
| Regular Linux Source     | Inspect or rebuild the Linux Regular Edition source.                |
| Regular Windows Portable | Run the Windows Regular Edition without a traditional installation. |
| Regular Windows Setup    | Install or upgrade the Windows Regular Edition per user.            |
| Regular Windows Source   | Inspect or rebuild the Windows Regular Edition source.              |
| Mini Linux Portable      | Run the minimal Linux Mini Edition binary directly.                 |
| Mini Linux Source        | Inspect or rebuild the Linux Mini Edition source.                   |
| Mini Windows Portable    | Run the single-file Windows Mini Edition.                           |
| Mini Windows Source      | Inspect or rebuild the Windows Mini Edition source.                 |

Windows builds require the Microsoft Edge WebView2 Runtime on the target computer. The release launchers embed the WebView2 loader payload but do not bundle the WebView2 Runtime itself. Obtain the Runtime from Microsoft's official WebView2 download page: https://developer.microsoft.com/microsoft-edge/webview2/

Linux builds require the runtime libraries provided by the target distribution, including GTK and WebKitGTK. The Debian setup package declares the main runtime dependencies for Debian-family systems, but users on other families should use their distribution's package manager and package names.

## Building from source

The source archives are edition- and platform-specific. A Regular source archive contains only the Regular source for its target platform plus the shared Password Manager module where required. A Mini source archive intentionally omits the shared Password Manager module and contains only the minimal Mini source for its target platform.

### Windows Regular

Use a MinGW-w64 x86_64 toolchain, GNU windres, the included WebView2 SDK files, and the Windows Regular build script:

bash windows/regular/build_v7_windows_regular.sh

The build produces the Regular core, portable launcher, and setup executable. The build is cross-buildable from Linux, but direct runtime testing must be performed on Windows.

### Windows Mini

Build the minimal single-file Windows Mini target with:

bash windows/mini/build_v7_windows_mini.sh

The script uses static GCC runtimes for the Mini target so the resulting portable executable does not require separate libstdc++-6.dll or libgcc files.

### Linux

Build the available Linux target in the source archive with:

bash linux/build_v7_linux.sh

The Linux build requires a C++17 compiler and the development packages for GTK 3, WebKitGTK 4.1, JSON-GLib, and, for the Regular Password Manager, libsecret. The build script can compile the Regular or Mini source that is present in the extracted archive.

## Verification status and limitations

The v7 source and package workflow includes static feature checks, exact-asset checks, clean source-payload checks, Linux compilation, and headless Linux startup smoke tests in the development sandbox. The tests confirm that Mini source packages do not contain the Regular Password Manager or other Regular-only additions.

These checks cannot replace direct runtime testing on a physical Windows 11 system, a working WebView2 Runtime installation, or each supported Linux distribution. Before broad distribution, test cold startup and close, Startpage navigation, ordinary and authentication popups, password save/edit/delete flows, keyring behavior, fullscreen, F11 entry and exit, profile switching, cookies, private mode, Session Restore, closed-tab reopening, tab sleeping, Delete Profile, Delete All Data, installer upgrade/uninstall, theme changes, high-DPI layouts, multimedia, and portable cleanup.

Pane Browser is not an independently audited password manager, anonymity system, or security product. Use strong device security, keep the operating system and WebView runtime updated, and do not rely on the application as the only copy of an important credential.

## Release history

| Release    | Main role                                                                                                                                                                                                                                                                                               |
| ---------- | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| **v1.0.0** | Initial native browser shell with navigation, multi-tab browsing, local session data, themes, and Pane Browser identity.                                                                                                                                                                                |
| **v2.0.0** | Added persistent application data, bookmarks, private browsing, Settings, optional tabs, custom title-bar support, and single-file distribution.                                                                                                                                                        |
| **v3.0.0** | Added Session Restore, Downloads, Find in Page, keyboard shortcuts, context menus, zoom, bookmark import/export, installer upgrade/uninstall, runtime detection, update checking, and additional stability/privacy work.                                                                                |
| **v4.0.0** | Added authentication compatibility handling, persistent profiles, managed popups, workspaces, vertical tabs, recovery, permissions, site-data controls, Command Palette, per-site settings, Reading Mode, Split View, Quick Notes, privacy modes, backup/export, profile deletion, and Delete All Data. |
| **v5.0.0** | Refined ordinary same-tab navigation, popup separation, profile and persistence boundaries, privacy exclusions, lifecycle safety, F11/video fullscreen behavior, and Mini Edition-wide F11 routing.                                                                                                     |
| **v6.0.0** | Added Linux Regular/Mini implementations and a local Regular Edition Password Manager with manual save, automatic detection confirmation, Password name, masked reveal, editing, selection, and bulk deletion. DNS was removed from the final v6 scope.                                                 |
| **v7.0.0** | Hardened Linux privacy defaults, fixed Linux F11 fullscreen toggle, blocked dangerous navigation schemes, reduced Linux binary size, and aligned paths/packaging to v7 while keeping Regular/Mini feature boundaries unchanged.                                                                      |

## License and project identity

Pane Browser is released under the MIT License. See LICENSE for the complete license text.

Pane Browser is an independent project and is not affiliated with Microsoft, Google, Startpage, WebKitGTK, GTK, libsecret, or any other company or service mentioned in this document.
