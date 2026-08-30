# Pane Browser v4.0.0

## Overview

Pane Browser v4.0.0 is a major usability and reliability release built on the independent native C++17/Win32/WebView2 codebase. It keeps the existing navigation, tabs, bookmarks, downloads, history, settings, private mode, and installer workflow while adding profile isolation, managed authentication windows, workspace tooling, site controls, crash recovery, and local backup.

## Highlights

- Authentication Compatibility Layer for redirect and popup-based OAuth, OIDC, Firebase, and SAML-style flows where the provider permits embedded WebView2 authentication.
- Persistent normal profiles with separate app-owned data and named WebView2 profiles.
- Active-profile deletion with explicit confirmation, protected Personal/root data, validated app-data cleanup, and official WebView2 profile deletion when supported; unsupported runtimes fail closed.
- Delete All Data action with two confirmations and a deferred cleanup helper that closes the app and permanently removes the complete validated `%LOCALAPPDATA%\\Pane Browser\\` tree, including app records and the shared WebView2 UDF.
- Guest Session entry through the privacy-oriented private-tab flow.
- Managed popup/authentication windows that preserve opener profile and support blank `about:blank` popup initialization.
- Tab groups, workspace metadata, vertical tabs, Tab Search, split view, and session restore.
- WebView2 tab sleeping/resume and controlled recovery after WebView2 process failure.
- Permission Center and Site Data Center with profile-scoped per-origin script, dialog, and sensitive-permission policies.
- Command Palette with filtering and native context-menu access to v4 productivity features.
- Smart Address Bar resolution for exact bookmarks and recent-history matches.
- Quick Notes, Reading Mode, and privacy dashboard modes.
- Profile backup and restore for app-owned data, including workspace/group metadata, without exporting cookies, tokens, passwords, or WebView2 site storage.
- Clear Browsing Data integration for app-owned records and supported WebView2 profile data.
- History filtering now clears hidden-row selections and makes Select All select only visible matching results.
- The complete-root deletion action does not touch files outside Pane Browser's app-data tree, including downloads saved to a custom external directory.

## Authentication limitation

The release does not promise universal compatibility with every login provider. A provider can still reject embedded authentication because of redirect-domain configuration, cross-origin helper requirements, storage partitioning, enterprise policy, or a system-browser-only policy. Windows testing with real Firebase redirect, OAuth/OIDC popup, and SAML flows is required before production deployment.

## Privacy boundary

Normal profile cookies and site data remain local so supported websites can remember sign-in. Private and authentication tabs are excluded from app-owned History/session restore rules as appropriate. Backup deliberately excludes the WebView2 User Data Folder, cookies, passwords, tokens, and site storage.

## Distribution

The portable launcher and installer require the Microsoft Edge WebView2 Runtime. The Runtime is not embedded in either executable. The portable launcher embeds the browser core and `WebView2Loader.dll` and extracts them to a temporary directory at runtime; the browser's persistent profile data is stored in local app data.

## Verification

The source passes MinGW-w64 syntax and normal build checks, plus targeted static regression checks for filtered History selection, Personal protection, safe profile IDs, the WebView2 Profile8 deletion API, complete-root deletion guards, deferred shutdown cleanup, and non-UDF profile cleanup boundaries. Core, portable, and installer targets are PE32+ Windows x64 GUI executables. Final validation must be performed on Windows x64 with WebView2 Runtime installed, including authentication, profile creation/switch/deletion, profile isolation, private/guest behavior, permissions, crash recovery, tab sleeping, split view, backup/restore, installer upgrade, and uninstallation.
