## v1.0.0 — August 25, 2026

**Short Description:** The first public release of Pane Browser — an independent Windows browser shell built with native Win32 and Microsoft WebView2.

**Breaking Changes**
- None — this is the first release of Pane Browser.

**New Features**
- A minimal Home page with a large search field.
- Direct URL navigation from Home and the address bar.
- Startpage search fallback for ordinary text input.
- Back, Forward, Refresh, Home, and History controls.
- Multi-tab support with new-tab and close-tab controls.
- Links that request a new window are opened in the active tab.
- Session History with search, full URLs, visit times, and Open, Delete, Select all, and Deselect all actions.
- Windows light/dark theme support with runtime theme refresh.
- A Pane Browser application icon embedded in the executable.
- Portable distribution with `WebView2Loader.dll` included beside the executable.

**Bug Fixes**
- None — there is no previous release to compare against.

**Performance / Improvements**
- A ready-to-use, single-folder portable distribution with no installation process.
- Note: in this version, History is session-only and is not persisted after the application exits.
