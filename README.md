# Pane Browser

Pane Browser is an independent, lightweight Windows browser shell for users who want a direct browsing window without a large application framework or a mandatory online account. It combines a native C++17/Win32 interface with Microsoft Edge WebView2 for web rendering.

The project is organized around two separate editions:

| Edition | Purpose |
| --- | --- |
| **Regular** | A full everyday browser with profiles, local persistence, privacy controls, recovery tools, productivity features, and installer support. |
| **MiniEdition** | A deliberately minimal single-tab browser with temporary per-run browser data and a single portable executable. |

The editions have separate source trees and different data models. Pane Browser uses its own implementation and does not claim to contain or reproduce proprietary source code from an unrelated product.

## Project principles

| Principle | Meaning |
| --- | --- |
| **Simple** | Common browsing actions should remain easy to find and quick to use. |
| **Local** | Normal History, bookmarks, settings, downloads, cookies, and profile data are kept on the user's device. |
| **Respectful** | Pane Browser does not operate a central service that receives the user's browsing History by default. |
| **Practical** | Features are selected for everyday Windows use rather than for feature count alone. |
| **Understandable** | Privacy, profile, deletion, popup, and recovery behavior should be visible and predictable. |

## v5.0.0 highlights

Pane Browser v5.0.0 is primarily a quality, stability, privacy, and usability refinement release. It preserves the broad v4 feature set while correcting navigation and lifecycle edge cases.

### Regular navigation correction

User-initiated ordinary links from search results, including `target="_blank"` links from Startpage, now navigate in the current opener tab. This prevents a normal result such as a YouTube link from unexpectedly opening another Pane Browser tab or window.

Authentication and blank bootstrap popups remain separate managed WebView2 child windows. This keeps the popup path available for OAuth, OIDC, Firebase, and similar authentication flows. Unsolicited ordinary popups remain handled and blocked.

### Fullscreen behavior

Regular native browser fullscreen is toggled with `F11`. While fullscreen is active, the tab strip, address bar, Back, Forward, Refresh, Home, and other browser controls are hidden so the WebView can use the complete window. Pressing `F11` again restores the previous window style, placement, toolbar controls, and address-bar content.

HTML video fullscreen is connected to WebView2 `ContainsFullScreenElementChanged`. When a page enters video fullscreen, the active WebView expands and the browser chrome is hidden. Leaving fullscreen restores the previous native layout.

MiniEdition handles F11 through the WebView2 controller's `AcceleratorKeyPressed` event as well as native window routing. F11 therefore works when focus is inside Home, the address bar, or an ordinary website.

## Regular edition features

Regular is intended for everyday Windows browsing while keeping browser-owned data local to the selected profile.

| Area | Features |
| --- | --- |
| Navigation | Smart Address Bar, direct URL navigation, Startpage search, complete final-URL tracking, Back, Forward, Refresh, and Home. |
| Tabs | Optional tabs, tab groups, workspaces, vertical tabs, Tab Search, split view, tab sleeping, closed-tab reopening, and crash recovery. |
| Profiles | Personal, Work, School, Guest, and other normal profiles with profile-scoped data. |
| History | Persistent local History, filtering, Select All, Deselect All, selected deletion, and full final URLs. |
| Bookmarks | Add, open, delete, import, and export bookmarks locally. |
| Downloads | Download manager, configurable destination, and local download records. |
| Privacy | Balanced, Strict, and Custom modes, private tabs, Permission Center, Site Data Center, per-site settings, and explicit deletion controls. |
| Productivity | Command Palette, Find in Page, keyboard shortcuts, zoom control, Reading Mode, Quick Notes, and workspace tools. |
| Settings | Search-engine selection, custom search URL templates, System/Light/Dark themes, optional tabs, custom title bar, vertical tabs, sleeping, profile management, backup/restore, and update checking. |
| Recovery | Session Restore for eligible normal tabs, closed-tab reopening, WebView recovery, and workspace restoration. |
| Data management | Clear Browsing Data, protected Personal profile, eligible profile deletion, backup/export, and permanent Delete All Data. |

### Regular profile and privacy model

Normal profile data is stored locally in the Pane Browser application-data area. Profile-scoped application records and named WebView2 profile contexts separate normal profiles from one another. Normal cookies and local website data remain available for everyday sign-in convenience.

Typical application-owned records include:

```
%LOCALAPPDATA%\Pane Browser\
├── history.pbh
├── bookmarks.pbb
├── settings.pbs
├── session.pbs
├── downloads.pbd
├── profiles\
└── WebView2\
```

Private tabs use isolated WebView2 data. Private and authentication tabs are excluded from normal application History, Session Restore, closed-tab recovery, persistent permissions, site policies, and persistent download records.

Private browsing is not anonymity software. Websites, network operators, Windows administrators, crash services, DNS infrastructure, and other software on the device may still observe or retain information outside the application's local records.

### Regular stability refinements

v5.0.0 resets settings and relevant in-memory collections before loading another profile so missing or incomplete files cannot leave stale values from a previous profile. Disabling Persistent History or Session Restore clears the relevant local records and active memory immediately, including session metadata.

Authentication URLs cannot enter normal History, session persistence, or closed-tab recovery. Tab-sleep callbacks verify that their tab still exists before updating it. In tabs-disabled mode, a private transition replaces the normal tab only after the private controller has been created successfully.

Internal-page state is normalized during transitions among History, Settings, Downloads, Notes, Site Data, Workspace, Command Palette, and Home. The trusted internal-message boundary remains limited to the expected internal context and source.

Delete Profile protects the Personal profile and is limited to eligible normal profiles. Delete All Data is a separate destructive action with confirmation and root validation. It permanently removes the approved Pane Browser data root without touching external download folders or creating a recovery backup.

## MiniEdition

MiniEdition is a separate minimal browser shell. It includes one tab, an address bar, Back, Forward, Refresh, Home, direct URL navigation, Startpage search, native browser fullscreen, and HTML video fullscreen.

It intentionally excludes History, persistent History, Bookmarks, Incognito/private mode, profile management, downloads management, Settings, theme switching, tab strips, multi-tab support, Session Restore, workspaces, extensions, and an installer.

Each MiniEdition run creates a randomly named temporary WebView2 User Data Folder. It does not use the normal Regular profile root and does not intentionally save application-owned History, bookmarks, settings, session records, or profiles. Its single-file executable embeds the minimal browser core and loader payload, so no separate loader DLL is required beside the executable.

Cleanup is best effort. Data retained by websites, Windows, crash services, DNS infrastructure, or the WebView2 Runtime remains outside the application's complete control.

## Keyboard shortcuts

Regular handles supported shortcuts from the native Windows message path and WebView2 accelerator path so they are intended to work when focus is in the web page, address bar, tab strip, or an internal Pane Browser page.

| Shortcut | Action |
| --- | --- |
| `F11` | Toggle native browser fullscreen. |
| `Ctrl+L` | Focus the address bar. |
| `Ctrl+T` | Open a new tab when tabs are enabled. |
| `Ctrl+W` | Close the active tab. |
| `Ctrl+Shift+T` | Reopen the most recently closed normal tab. |
| `Ctrl+Tab` | Move to the next tab. |
| `Ctrl+Shift+Tab` | Move to the previous tab. |
| `Ctrl+R` or `F5` | Reload the active page. |
| `Ctrl+F` | Find in page. |
| `Ctrl+D` | Add the current page to Bookmarks. |
| `Ctrl+J` | Open Downloads. |
| `Ctrl+H` | Open History. |
| `Ctrl+Shift+P` | Open a private tab. |
| `Ctrl+Shift+Space` | Open Command Palette. |
| `Ctrl++` / `Ctrl+-` | Zoom in or out. |
| `Ctrl+0` | Reset zoom. |

## Distribution

The v5.0.0 Regular release provides a single-file portable launcher, a per-user setup installer, and a source package. MiniEdition provides a single-file portable launcher and a source package.

| Package | Intended use |
| --- | --- |
| **Regular Portable** | Download one file and run it without manually placing `WebView2Loader.dll` beside the launcher. |
| **Regular Setup** | Install or upgrade the Regular edition under the user's local application-data directory and create Windows shortcuts. |
| **MiniEdition Portable** | Run the minimal single-tab edition from one executable. |
| **Source packages** | Inspect or rebuild the corresponding edition. |

Both editions require the Microsoft Edge WebView2 Runtime to be installed on Windows. The launchers embed the WebView2 loader payload, but they do not bundle the WebView2 Runtime itself. The Runtime can be obtained from Microsoft's official [WebView2 download page](https://developer.microsoft.com/microsoft-edge/webview2/).

## Release history

| Release | Main role |
| --- | --- |
| **v1.0.0** | Initial native browser shell with navigation, multi-tab browsing, local session data, themes, and the Pane Browser identity. |
| **v2.0.0** | Added persistent application data, bookmarks, private browsing, Settings, optional tabs, custom title-bar support, and single-file distribution. |
| **v3.0.0** | Added Session Restore, Downloads, Find in Page, keyboard shortcuts, context menus, zoom, bookmark import/export, installer upgrade/uninstall, runtime detection, update checking, and additional stability/privacy work. |
| **v4.0.0** | Added authentication compatibility handling, persistent profiles, managed popups, workspaces, vertical tabs, recovery, permissions, site data controls, Command Palette, per-site settings, Reading Mode, Split View, Quick Notes, privacy modes, backup/export, profile deletion, and Delete All Data. |
| **v5.0.0** | Refined ordinary same-tab navigation, popup separation, profile/persistence boundaries, privacy exclusions, lifecycle safety, F11/video fullscreen behavior, and MiniEdition-wide F11 routing. |

For a release download, use the portable executable for a no-install experience, the setup executable for a per-user installation, and the source archive when inspecting or rebuilding the project.

## Building from source

### Regular

The Regular source package contains the native browser source, persistence helpers, resource files, launcher and installer sources, build script, project license, and the minimum WebView2 SDK files required by the build.

```bash
./build_v5.sh
```

The Regular build creates:

```
PaneBrowser-v5.0.0-core.exe
PaneBrowser-v5.0.0-portable.exe
PaneBrowser-v5.0.0-setup.exe
```

### MiniEdition

The MiniEdition source package contains the minimal browser source, single-file launcher sources, resource files, build script, project license, and the minimum WebView2 SDK files required by the build.

```bash
./build_v5_mini.sh
```

The MiniEdition build creates only:

```
PaneBrowser-v5.0.0-MiniEdition-portable.exe
```

A Windows-oriented MinGW-w64 x64 toolchain, GNU `windres`, the included WebView2 SDK files, and an installed Microsoft Edge WebView2 Runtime on the target Windows computer are required. The build scripts are designed for Windows x64 targets and are also cross-buildable in the Linux development environment used for this project.

## Verification and limitations

The v5 source was cross-built as Windows x64 PE32+ targets under Linux. Regular was checked with C++17 compilation, targeted source regression checks, a fail-hard static audit, and clean rebuilding from an extracted source package. MiniEdition was checked with a clean build, a dedicated fullscreen/scope audit, WebView2 accelerator-handler checks, removal of the visible alpha status label, and clean rebuilding from its source package.

Static compilation and PE checks cannot replace direct Windows x64 runtime testing. Before broad distribution, test cold startup and close, Startpage result navigation, authentication popups, HTML video fullscreen, F11 from Home and ordinary websites, F11 exit and restoration of the toolbar/address bar, profile switching, cookies, private mode, Session Restore, closed-tab reopening, tab sleeping, Delete Profile, Delete All Data, installer upgrade/uninstall, system theme changes, multi-monitor placement, high-DPI layout, WebView2 Runtime compatibility, and portable cleanup.

Pane Browser is a local browser shell, not an anonymity system. Local storage is not absolute secrecy, and the application cannot control all data retained by websites, Windows, networks, crash services, DNS infrastructure, or the WebView2 Runtime.

## License

Pane Browser is released under the MIT License. See [`LICENSE`](LICENSE) for the complete license text.

Pane Browser is an independent project and is not affiliated with Microsoft, Google, Startpage, or any other company or service mentioned in this document.
