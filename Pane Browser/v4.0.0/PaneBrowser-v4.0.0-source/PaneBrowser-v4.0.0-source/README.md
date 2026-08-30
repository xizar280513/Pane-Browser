## v4.0.0 — August 26, 2026

**Short Description:** A major usability, authentication-compatibility, privacy, and profile-management release introducing a profile-aware browsing model, an Authentication Compatibility Layer, workspaces/tab groups, and explicit data lifecycle controls.

**Breaking Changes**
- Pane Browser is no longer organized around a single browser data container; it now uses a profile-aware model (Personal, Work, School, etc.) with separate data directories per profile under `%LOCALAPPDATA%\Pane Browser\profiles\<profile-id>\`.
- New-window popup handling (including blank `about:blank` popups) is now managed through the Authentication Compatibility Layer, changing the popup-handling flow compared to previous versions.

**New Features**
- Authentication Compatibility Layer for OAuth, OIDC, Firebase, and SAML flows on WebView2, including deferral handling for `NewWindowRequested` and support for blank `about:blank` bootstrap popups.
- Persistent Profile Manager: normal profiles (Personal, Work, School, etc.) with separate History, Bookmarks, Downloads, cookies, site data, settings, permissions, and session state per profile.
- Guest Session as an isolated privacy-oriented entry (still uses the existing private-tab mechanism rather than becoming a durable new profile).
- Profile deletion and **Delete All Data** — a destructive action with two native confirmations, a deferred cleanup worker, and app-data root verification.
- A popup and authentication window manager that preserves the opener's profile, privacy state, and workspace/tab-group context.
- Tab groups, workspaces, vertical tabs, Tab Search, Split View, tab sleeping, and controlled WebView2 crash recovery.
- Privacy Dashboard with Balanced, Strict, and Custom modes.
- Permission Center and Site Data Center for managing permissions (camera, microphone, geolocation, notifications, clipboard) and per-site policies (JavaScript, dialogs).
- Command Palette and Smart Address Bar (exact bookmark/history resolution before ordinary URL normalization).
- Reading Mode, Quick Notes, and profile backup/restore (excluding the WebView2 User Data Folder, cookies, cache, passwords, and authentication tokens).

**Bug Fixes**
- Fixed filtered-History selection: when History is filtered, hidden rows are now excluded from the active selection, and Select All only applies to visible rows.

**Performance / Improvements**
- More deterministic builds, profile-aware storage documentation, clearer cleanup boundaries, and safer profile/root deletion behavior.
- App-owned data is stored locally under `%LOCALAPPDATA%\Pane Browser\`; the application does not upload History or profile data to any server.
- QA: the MinGW-w64 build and the standard v4 build both passed; static checks cover callback lifetime, popup profile matching, private-data exclusion, per-site policy timing, profile-scoped persistence, recovery recreation, backup boundaries, case-insensitive profile identity, safe profile/root deletion, deferred shutdown cleanup, filtered History selection, and final Settings action ordering.
