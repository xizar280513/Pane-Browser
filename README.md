# Pane Browser

Pane Browser is an independent, lightweight Windows browser shell built for people who want a straightforward browsing window without the weight of a large application framework.

It combines a native Win32 interface with Microsoft WebView2 for web rendering. The result is a compact desktop browser focused on the essentials: opening websites, searching, organizing tabs, keeping useful local browsing data, and providing a clear separation between ordinary browsing and private browsing.

## Purpose

Pane Browser exists to provide a simple and understandable browser experience for Windows. It is intentionally designed around a small native application rather than a cloud account, a synchronization service, or an extensive platform ecosystem.

The project prioritizes four principles:

| Principle | Meaning |
| --- | --- |
| **Simple** | Common browsing actions should remain easy to find and quick to use. |
| **Local** | History, bookmarks, settings, downloads, and normal browser profile data are kept on the user's device. |
| **Respectful** | Pane Browser does not operate a central service that receives the user's browsing history. |
| **Practical** | Features are selected for everyday Windows use rather than for feature count alone. |

## What it does

Pane Browser provides normal web navigation through a native address bar and a configurable search engine. It supports multiple tabs, bookmarks, persistent local History, Downloads, session restore, private tabs, page search, zoom, context menus, keyboard shortcuts, configurable themes, and an optional custom title bar.

The application also includes a three-dot overflow menu for secondary browser actions. The primary navigation controls remain close to the address bar, while less frequently used features are grouped in one predictable location.

### Everyday browsing

- Direct URL navigation and search from the address bar.

- Configurable search providers, including a custom search URL.

- Back, Forward, Refresh, Home, and multi-tab browsing.

- Tab closing, reopening of recently closed normal tabs, and session restore.

- Find in page, page zoom, context actions, and keyboard shortcuts.

- Downloads with a configurable destination and local status records.

- Bookmarks with HTML import and export.

### Appearance and controls

- System, Light, and Dark interface themes.

- Optional tab support for users who prefer a single-page layout.

- Optional custom title bar.

- Native Windows application icon and controls.

- Overflow menu for History, Bookmarks, Settings, Downloads, Find, and Private browsing.

## Local data and privacy model

Pane Browser uses separate local storage areas for application data and the ordinary WebView2 browser profile. The normal profile is persistent so websites can retain cookies and sign-in sessions when those websites choose to issue persistent cookies.

Typical application data is stored under:

```
%LOCALAPPDATA%\Pane Browser\
├── history.pbh
├── bookmarks.pbb
├── settings.pbs
├── session.pbs
├── downloads.pbd
└── WebView2\
```

Normal History, bookmarks, settings, session records, download records, cookies, and site data are not uploaded to a Pane Browser server. The application does not provide cloud synchronization by default.

Private tabs are designed for a separate browsing context. Private navigation is excluded from normal Pane Browser History and session restore, and private download records are not written to the persistent application download list. Private browsing does not make a user anonymous to websites, network operators, Windows administrators, or other software on the device.

Pane Browser uses conservative privacy behavior. It requests profile-level Balanced tracking prevention where the installed WebView2 Runtime supports that capability, while keeping ordinary first-party cookies available for convenience. Website security policies can still require a user to sign in again.

> Local storage is not the same as absolute secrecy. Anyone or any software with sufficient access to the Windows account or device may be able to access local browser data.

## Releases

The repository's release assets contain the Windows builds and source packages. The release line is intentionally incremental:

| Release | Role |
| --- | --- |
| **v1.0.0** | Initial native browser shell with navigation, multi-tab browsing, local session History, themes, and the Pane Browser identity. |
| **v2.0.0** | Added persistent application data, bookmarks, private browsing, Settings, optional tabs, custom title-bar support, and single-file distribution. |
| **v3.0.0** | Added session restore, Downloads, Find in page, keyboard shortcuts, context menus, zoom, bookmark import/export, installer upgrade/uninstall, runtime detection, update checking, and stability/privacy refinements. |

For each release, use the portable executable for a no-install experience, the setup executable for a per-user installation, and the source archive when inspecting or rebuilding the project.

## Distribution

Pane Browser is distributed in two Windows executable forms:

| Package | Intended use |
| --- | --- |
| **Portable executable** | Download one file and run it without manually placing `WebView2Loader.dll` beside the launcher. Temporary launcher files are extracted at runtime and cleaned up on a best-effort basis. |
| **Setup executable** | Install or upgrade Pane Browser under the user's local application data directory and create Desktop and Start Menu shortcuts. The installer also provides an uninstall path while preserving user data. |

Both packages require the **Microsoft WebView2 Runtime** to be installed on Windows. The launchers embed `WebView2Loader.dll`, but they do not bundle the WebView2 Runtime itself. Obtain the runtime from Microsoft's official [WebView2 download page](https://developer.microsoft.com/microsoft-edge/webview2/).

## Keyboard shortcuts

Pane Browser handles its supported shortcuts from the main Windows message loop, so they are intended to work regardless of whether focus is currently in the web page, address bar, tab strip, or an internal Pane Browser page.

| Shortcut | Action |
| --- | --- |
| `Ctrl+L` | Focus the address bar |
| `Ctrl+T` | Open a new tab |
| `Ctrl+W` | Close the active tab |
| `Ctrl+Shift+T` | Reopen the most recently closed normal tab |
| `Ctrl+Tab` | Move to the next tab |
| `Ctrl+Shift+Tab` | Move to the previous tab |
| `Ctrl+R` or `F5` | Reload the active page |
| `Ctrl+F` | Find in page |
| `Ctrl+D` | Add the current page to Bookmarks |
| `Ctrl+J` | Open Downloads |
| `Ctrl+H` | Open History |
| `Ctrl+Shift+P` | Open a private tab |
| `Ctrl++` / `Ctrl+-` | Zoom in or out |
| `Ctrl+0` | Reset zoom |

## Technical foundation

Pane Browser is a native C++17 Win32 application. Web content is rendered by Microsoft WebView2, while the surrounding window, toolbar, tab strip, title bar, internal pages, and persistence layer are implemented by the project.

The project is an independent rebuild and is not the source code of another browser. It does not claim affiliation with Microsoft, Google, Startpage, or any of the search providers available in its settings.

## Building and testing

The release source archive contains the C++ sources, resource files, build script, required project headers, documentation, and the MIT License. A Windows-oriented MinGW-w64 build script is provided for maintainers who want to reproduce the executables.

Static compilation and PE structure checks can be performed in a cross-compilation environment. Final behavior should still be tested on a real Windows x64 system with the WebView2 Runtime installed, including cookies, private mode, downloads, keyboard focus, DPI scaling, title-bar interaction, shortcut creation, upgrade, uninstall, and portable cleanup.

## Project status

Pane Browser v3.0.0 is intended to be a practical personal browser shell rather than a replacement for a full commercial browser ecosystem. It is suitable for everyday browsing when the user has the Microsoft WebView2 Runtime installed and understands the local-storage privacy model.

The project remains open to careful improvements that preserve its central character: a small, direct, local-first Windows browser with understandable behavior.

## License

Pane Browser is released under the MIT License. See [`LICENSE`](LICENSE) for the complete license text.

Pane Browser is an independent project and is not affiliated with Microsoft, Google, Startpage, or any other company or service mentioned in this document.
