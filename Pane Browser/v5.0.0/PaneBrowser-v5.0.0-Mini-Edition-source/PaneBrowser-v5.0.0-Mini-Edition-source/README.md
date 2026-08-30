## v5.0.0 — August 26, 2026

**Short Description:** A quality, stability, privacy, and usability refinement release that splits Pane Browser into two independent editions: Regular Edition and Mini Edition.

**Breaking Changes**
- Pane Browser is now split into two editions with separate source trees, data models, and distribution scopes: **Regular Edition** (full feature set) and **Mini Edition** (a minimal single-tab browser with no persistent data).
- Mini Edition now deliberately excludes History, Bookmarks, private/incognito mode, profile management, downloads, Settings, theme switching, multi-tab support, Session Restore, workspaces, extensions, and an installer.

**New Features**
- Mini Edition: a deliberately minimal and independent single-tab browser shell with an address bar, basic navigation, Startpage search, and both native and HTML video fullscreen.
- Regular Edition retains the full v4.0.0 feature set (profiles, tab groups/workspaces, privacy modes, Command Palette, etc.) with the addition of the Smart Address Bar and more complete final-URL tracking during navigation.

**Bug Fixes**
- Navigation fix: ordinary `target="_blank"` links (including a Startpage result that opens YouTube) now navigate in the current opener tab instead of unexpectedly opening a new Pane Browser tab or window. Authentication popups and blank bootstrap popups remain handled separately as managed WebView2 child windows.
- Settings and related in-memory collections are now reset before loading another profile, preventing stale state from leaking between profiles.
- Disabling Persistent History or Session Restore now immediately clears the corresponding local records and active memory; session metadata is also removed when restore is disabled.
- Authentication windows can no longer enter closed-tab recovery or normal session persistence.
- Tab-sleep callbacks now verify a tab still exists before updating its state, and a transition into private mode (when tabs are disabled) only replaces the normal tab after the private controller has been created successfully.
- Internal-page flags are normalized during navigation to prevent stale History, Settings, Downloads, Notes, Site Data, Workspace, Command Palette, or Home state.

**Performance / Improvements**
- Native (F11) fullscreen and HTML video fullscreen behavior was refined, including restoring window style, placement, toolbar visibility, and address-bar content after exiting fullscreen.
- Mini Edition creates a randomly named temporary WebView2 User Data Folder per run and does not persist application-owned data; its executable embeds the browser core and loader payload, so no separate loader DLL is required.
- Verification: Regular Edition was cross-built as Windows x64 PE32+ targets under Linux and checked with C++17 compilation, source regression assertions, a fail-hard static audit, and a clean rebuild from an extracted source package.
