## v2.0.0 — August 25, 2026

**Short Description:** Expands the Win32/WebView2 browser shell with local data storage, bookmarks, private browsing, settings, configurable themes, optional tabs, and single-file Windows distribution.

**Breaking Changes**
- None — this release is purely additive on top of v1.0.0.

**New Features**
- Persistent file-based browsing history.
- Bookmarks with add, open, and delete actions.
- Private browsing tabs using WebView2 private controller options.
- An optional custom client-area title bar.
- Optional tab support controlled from Settings.
- A Settings page for theme, tabs, persistent history, and title-bar preferences.
- System, Light, and Dark UI theme options.
- A single-file portable executable that embeds the browser core and `WebView2Loader.dll`.
- A single-file installer that installs Pane Browser and creates Desktop and Start Menu shortcuts.

**Bug Fixes**
- No bug fixes were explicitly recorded for this release; the focus is new feature additions.

**Performance / Improvements**
- The portable build extracts its embedded runtime files temporarily and removes them after the browser closes.
- The installer places the application under the user's local application data directory and creates shortcuts automatically.
- History and bookmarks are stored locally; private browsing does not add visited pages to normal browsing history.
