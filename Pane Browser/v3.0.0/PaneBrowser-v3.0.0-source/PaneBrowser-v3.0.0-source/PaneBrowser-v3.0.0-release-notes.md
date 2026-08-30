## v3.0.0 — August 26, 2026

**Short Description:** A productivity, reliability, and interface refinement release over v2.0.0, covering a toolbar reorganization, a broader search-engine catalog, and multiple security and stability fixes.

**Breaking Changes**
- The main toolbar was reorganized: Back, Forward, Refresh, Home, and the address bar now sit on the left, with a three-dot overflow button at the far right of the address bar containing Find in page, Downloads, Private tab, Settings, Bookmarks, and History. Existing command handlers are reused so there is no duplicate navigation path, but the UI control locations changed from the previous version.

**New Features**
- A broad built-in search-engine catalog in Settings: Google, Bing, DuckDuckGo, Brave Search, Startpage, Mojeek, Swisscows, Yahoo, Baidu, Naver, Yandex, Ecosia, Qwant, Seznam, Ask.com, AOL Search, Kagi, MetaGer, SearXNG, You.com, Perplexity, Yep, Sogou, 360 Search, Rambler, Daum, Goo, Excite, Dogpile, Gibiru, and WolframAlpha — plus a Custom URL entry supporting `{query}` or `%s` placeholders.
- The selected engine is used both for address-bar input and the Home page search flow; the selection and custom URL are saved in `settings.pbs`.
- A complete keyboard shortcut reference table in Settings (`Ctrl+T`, `Ctrl+W`, `Ctrl+H`, `Ctrl+Shift+T`, and others).
- Added session restore, Download Manager, Find in page, reopen-closed-tab, context menu, clear browsing data, zoom controls, bookmark HTML import/export, installer upgrade/uninstall support, WebView2 Runtime detection, and optional GitHub update checking.

**Bug Fixes**
- Native/web trust-boundary validation so WebView2 messages are only accepted from Pane Browser's internal `about:blank` pages, preventing external sites from using the internal command channel.
- History no longer requests Google favicons containing visited hostnames; it now uses a local badge instead.
- Persistence now validates file-vs-directory paths, uses process-specific temporary filenames, cleans up failed temporary writes, removes stale History/session files when those features are disabled, caps History at 10,000 records, and reports write failures through the Windows debug channel.
- Query normalization now trims whitespace and treats dotted phrases containing spaces as searches.
- Reopening a closed tab now respects disabled tab support.
- Clear browsing data now asks for confirmation and reports asynchronous WebView2 completion or partial failure.
- Fixed custom title-bar button hit testing, per-monitor DPI awareness fallback, WebView2 initialization failure shutdown handling, COM `QueryInterface`, page-content context menus, portable cleanup retry, and installer atomic extraction/shortcut diagnostics.
- Fixed global shortcuts so they remain available when focus is in the web page, address bar, tab strip, or internal pages — the dispatcher now checks the main Windows message queue before messages are forwarded to the focused control.

**Performance / Improvements**
- The update checker now runs off the UI thread.
- Balanced tracking prevention is requested at the WebView2 profile level (where the installed Runtime supports it) to reduce common cross-site tracking without deleting first-party cookies or forcing repeated sign-ins.
- QA: the source and launchers pass strict MinGW-w64 syntax checks with warnings treated as errors (excluding known SDK pragma and dynamic Windows API cast warnings).
