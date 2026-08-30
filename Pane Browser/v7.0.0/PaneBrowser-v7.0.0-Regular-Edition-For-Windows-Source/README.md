## v7.0.0 — August 29, 2026

**Short Description:** A release focused on Linux privacy hardening, stability fixes, and significantly smaller Linux binaries.

**Breaking Changes**
- Several WebView features are now disabled by default on Linux: page cache, HTML5 local storage, HTML5 database, media stream, WebGL, encrypted media, WebAudio, and JavaScript clipboard access — this may affect sites that rely on these features.
- The User-Agent string changed to `PaneBrowser/7.0.0` (Regular) and `PaneBrowserMini/7.0.0` (Mini).
- The Linux config directory is now created as `pane-browser-v7` at startup.

**New Features**
- No significant new features in this release; the focus is privacy hardening and stability.

**Bug Fixes**
- F11 fullscreen now toggles correctly on Regular Edition on Linux.
- Navigation policy now blocks `javascript:` and `vbscript:` schemes.

**Performance / Improvements**
- Linux binaries are dramatically smaller: Mini Portable is 34.2 KB and Regular Portable is 106 KB (binaries are stripped; Mini no longer links `libsecret`).
- Paths, namespace, titles, and packaging aligned to v7.
- Windows Regular binaries now identify as v7.0.0.
- Note: Password Manager detection is still heuristic; Linux builds require system GTK 3 and WebKitGTK 4.1; Windows builds require the WebView2 Runtime.
