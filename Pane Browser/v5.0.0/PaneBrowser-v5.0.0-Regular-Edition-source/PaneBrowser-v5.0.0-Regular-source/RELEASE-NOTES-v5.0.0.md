# Pane Browser v5.0.0

Pane Browser v5.0.0 is a quality, stability, privacy, and usability refinement release. It contains two independent editions: **Regular Edition** and **Mini Edition**. Both are native Windows C++17/Win32 applications using Microsoft Edge WebView2, but they have separate source trees, separate data models, and different distribution scopes.

This release uses its own source tree and implementation. It does not claim to contain, reproduce, or reuse proprietary source code from an unrelated product.

## Release summary

| Edition | Purpose | Data model | Distribution |
|---|---|---|---|
| **Regular Edition** | Full everyday browser with profiles, persistence, privacy controls, recovery, and productivity features. | Local normal-profile data with cookies and site data; isolated private contexts. | Portable launcher, per-user setup installer, and source package. |
| **Mini Edition** | Minimal single-tab browser focused on simple browsing and reduced local persistence. | Temporary per-run WebView2 user-data folder; no application-owned persistent browsing records. | Single portable launcher and source package. |

The Microsoft Edge WebView2 Runtime is an external Windows prerequisite for both editions. It is not embedded as a complete browser engine.

## Regular Edition

Regular Edition is the full Pane Browser experience for everyday use. It retains normal cookies and local website data inside the selected normal profile so that supported websites can preserve sign-in sessions between launches. Profile data remains local and profile-scoped.

### Features

| Area | Included functionality |
|---|---|
| Navigation | Smart Address Bar, complete final-URL tracking, direct URL navigation, Startpage search, Back, Forward, Refresh, Home, and ordinary same-tab navigation. |
| Tabs | Optional tabs, tab groups, workspaces, vertical tabs, Tab Search, split view, tab sleeping, closed-tab reopening, and crash recovery. |
| Profiles | Personal, Work, School, Guest, and other normal profiles with separate History, Bookmarks, Downloads, cookies, site data, settings, permissions, and session state. |
| History | Persistent local History, filtering, Select All, Deselect All, delete selected records, and complete final URLs. |
| Bookmarks | Local add, open, delete, import, and export operations. |
| Downloads | Download manager and local download records. Private-tab records are excluded from normal persistence. |
| Privacy | Balanced, Strict, and Custom privacy modes, Permission Center, Site Data Center, per-site settings, private tabs, and explicit data-deletion controls. |
| Productivity | Command Palette, Find in Page, keyboard shortcuts, zoom control, Reading Mode, Quick Notes, and workspace tools. |
| Settings | Search-engine selection and custom templates, themes, optional tabs, custom title bar, vertical tabs, tab sleeping, profile management, backup/restore, and update checking. |
| Recovery | Session Restore for eligible normal tabs, closed-tab reopening, WebView recovery, and profile-scoped workspace restoration. |
| Data management | Protected Personal profile, eligible profile deletion, Clear Browsing Data, backup/export, and permanent Delete All Data. |

### Navigation fix

The main v5 navigation fix changes the handling of ordinary user-initiated links from search results. A normal `target="_blank"` link, including a Startpage result that opens YouTube, now navigates in the current opener tab instead of unexpectedly creating a new Pane Browser tab or window.

Authentication and blank bootstrap popups remain separate managed WebView2 child windows. This preserves the intended compatibility path for OAuth, OIDC, Firebase, and similar authentication flows. Non-user ordinary popups remain handled and blocked. Provider-specific restrictions on embedded authentication are not bypassed.

### View Source and Inspect

Regular Edition supports `Ctrl+U` and the right-click **View Source** command. For an eligible HTTP or HTTPS page, it opens a new tab using the `view-source:` scheme, such as `view-source:https://www.startpage.com/sp/search`.

`F12` and the right-click **Inspect** command open the Microsoft Edge WebView2 DevTools window. The DevTools window supplies the panels provided by the installed WebView2 Runtime, including Elements, Console, Sources, Network, Performance, Memory, Application, Security, and other available panels. Links opened as new windows from a `view-source:` document are routed to a new Regular Edition tab when tabs are enabled. The separate DevTools window is owned by WebView2 and is not recreated as a Pane Browser tab.

### Fullscreen behavior

Regular Edition has two related fullscreen paths:

- **Native browser fullscreen through F11.** While F11 fullscreen is active, the tab strip, address bar, Back, Forward, Refresh, Home, and other browser controls are hidden. The active WebView occupies the full window.
- **HTML video fullscreen.** WebView2 `ContainsFullScreenElementChanged` tracks a page's fullscreen element and expands the active WebView when a video requests fullscreen.[1]

Pressing F11 again exits native browser fullscreen. The previous window style, placement, toolbar visibility, and address-bar content are restored. The address bar is hidden temporarily during fullscreen; it is not deleted. The same restoration behavior applies when HTML video fullscreen ends.

### Profile and privacy behavior

Normal browsing data is stored locally under the Pane Browser application-data area. Each normal profile has its own application records and named WebView2 profile context. Normal cookies and local website data are retained for everyday sign-in convenience.

Private tabs use isolated WebView2 data and are excluded from normal application History, Session Restore, closed-tab recovery, persistent permissions, site policies, and persistent download records. Authentication URLs are excluded from ordinary History and recovery paths.

The Personal profile is protected from deletion. Delete Profile is limited to eligible normal profiles and requires the supported WebView2 deletion API. Delete All Data is a separate destructive operation that requires confirmation and permanently removes the approved Pane Browser data root, including local records, profiles, WebView2 data, cookies, cache, bookmarks, downloads, and settings. It does not create a recovery backup.

### Stability refinements

v5.0.0 resets settings and relevant in-memory collections before loading another profile, preventing stale state from leaking between profiles. Disabling Persistent History or Session Restore clears the corresponding local records and active memory immediately. Session metadata is removed when restore is disabled.

Authentication windows cannot enter closed-tab recovery or normal session persistence. Tab-sleep callbacks retain the model safely and verify that a tab is still present before updating it. In tabs-disabled mode, a private transition replaces the normal tab only after the private controller has been created successfully. Internal-page flags are normalized during navigation to prevent stale History, Settings, Downloads, Notes, Site Data, Workspace, Command Palette, or Home state.

## Mini Edition

Mini Edition is a deliberately minimal and independent single-tab browser shell. It includes an address bar, Back, Forward, Refresh, Home, direct URL navigation, Startpage search, native browser fullscreen, and HTML video fullscreen.

Mini Edition intentionally excludes History, persistent History, Bookmarks, Incognito/private mode, profile management, downloads management, Settings, theme switching, tab strips, multi-tab support, Session Restore, workspaces, extensions, and an installer.

### Mini Edition F11 and video fullscreen

Mini Edition handles F11 through the WebView2 controller's `AcceleratorKeyPressed` event as well as native window routing. This allows F11 to work while focus is inside Home, the address bar, a normal website, or another WebView2 page. The WebView2 `ContainsFullScreenElementChanged` event continues to handle HTML video fullscreen.

During fullscreen, Mini Edition hides its browser controls and expands the WebView. Pressing F11 again restores the previous native window state.

### Mini Edition data model

Each Mini Edition run creates a randomly named temporary WebView2 User Data Folder. Mini Edition does not use the normal Regular Edition profile root and does not intentionally save application-owned History, Bookmarks, settings, session records, or profiles. Its single-file executable embeds the minimal browser core and loader payload, so no separate loader DLL is required beside the executable.

Cleanup is best effort. Data retained by websites, Windows, crash services, DNS infrastructure, or the WebView2 Runtime is outside the application's complete control.

## Distribution

The Regular Edition build produces a portable launcher, a per-user setup installer, and a development core executable. The Mini Edition build produces only its single portable executable. Source packages contain the files required by their corresponding build scripts and do not contain generated application executables.

## Build from source

Regular Edition:

```bash
./build_v5.sh
./audit_v5.sh
python3 test_v5_regressions.py
```

Mini Edition:

```bash
./build_v5_mini.sh
./audit_mini_v5.sh
```

The source packages include the minimum WebView2 SDK files required by these scripts, including the header, x64 loader import library, loader payload, and applicable SDK notices.

## Verification

Regular Edition v5 was cross-built as Windows x64 PE32+ targets under Linux and checked with C++17 compilation, targeted source regression assertions, a fail-hard static audit, and a clean rebuild from an extracted source package. The fullscreen checks cover F11 dispatch, hidden browser chrome while fullscreen, saved window placement, restored toolbar controls, and restored address-bar content.

Mini Edition was checked with a clean build, a dedicated fail-hard fullscreen/scope audit, a WebView2 accelerator-handler source check, removal of the visible alpha status label, and a clean rebuild from its source package.

## Runtime testing notice

Static and cross-build verification cannot replace direct Windows x64 testing. Before broad distribution, test cold startup and close, ordinary Startpage result navigation, authentication popups, HTML video fullscreen, F11 from Home and normal websites, F11 exit and toolbar/address-bar restoration, profile switching, private mode, session restore, closed-tab reopening, tab sleeping, Delete Profile, Delete All Data, installer upgrade/uninstall, system theme changes, WebView2 Runtime compatibility, multi-monitor behavior, and high-DPI scaling.

## Known limitations

Pane Browser is a local browser shell, not an anonymity system. Normal websites can make their own network requests and apply their own privacy policies. Local storage and private mode do not prevent website-side collection, operating-system logging, DNS visibility, or all WebView2 Runtime behavior.

The Linux development environment cannot execute the Windows WebView2 Runtime or validate every Windows rendering, focus, shell, DPI, and installer behavior. Final confirmation of fullscreen focus handling, video fullscreen, window restoration, custom title-bar interaction, and cleanup must be performed on Windows.

## References

[1]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2 "Microsoft Learn — ICoreWebView2, including ContainsFullScreenElementChanged"

[2]: https://learn.microsoft.com/en-us/microsoft-edge/webview2/reference/win32/icorewebview2controller "Microsoft Learn — ICoreWebView2Controller, including AcceleratorKeyPressed"
