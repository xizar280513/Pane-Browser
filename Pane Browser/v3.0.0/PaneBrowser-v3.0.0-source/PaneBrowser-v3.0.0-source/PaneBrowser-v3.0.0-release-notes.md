# Pane Browser v3.0.0

Pane Browser v3.0.0 is a productivity, reliability, and interface refinement release over v2.0.0. The version remains v3.0.0; the latest changes are refinements within this release.

## Interface refinement

The main toolbar now keeps Back, Forward, Refresh, Home, and the address bar on the left. A three-dot overflow button is placed at the far right of the address bar. Its menu contains Find in page, Downloads, Private tab, Settings, Bookmarks, and History. The existing command handlers are reused so the reorganization does not create duplicate navigation paths.

## Search settings

Settings now includes a broad built-in search catalog: Google, Bing, DuckDuckGo, Brave Search, Startpage, Mojeek, Swisscows, Yahoo, Baidu, Naver, Yandex, Ecosia, Qwant, Seznam, Ask.com, AOL Search, Kagi, MetaGer, SearXNG, You.com, Perplexity, Yep, Sogou, 360 Search, Rambler, Daum, Goo, Excite, Dogpile, Gibiru, and WolframAlpha. A Custom URL entry is also available. Custom templates may contain `{query}` or `%s`; if neither marker is present, the encoded query is appended.

The selected engine is used for text entered in the address bar and for the Home page search flow. The selected engine and custom URL are saved in `settings.pbs`.

## Keyboard and session behavior

Settings includes a complete table of the shortcuts implemented by the application. In particular, `Ctrl+T` opens a new tab, `Ctrl+W` closes the active tab, `Ctrl+H` opens History, and `Ctrl+Shift+T` restores the most recently closed normal HTTP(S) tab. Startup session restore continues to restore only normal HTTP(S) tabs. Private tabs are neither written to session storage nor reopened by the closed-tab stack.

## Existing v3 capabilities

The release also includes session restore, Download Manager, Find in page, keyboard shortcuts, reopen-closed-tab support, context menu, clear browsing data, zoom controls, bookmark HTML import/export, installer upgrade/uninstall support, WebView2 Runtime detection, optional GitHub update checking, persistent-history initialization and atomic writes, private mode, optional tabs, custom title bar, and single-file portable/setup launchers.

## Privacy and stability refinement

The normal WebView2 profile remains persistent so ordinary cookies and sign-in sessions can be reused. Private controllers remain isolated, and private download records are filtered out before persistent download data is written. The application requests Balanced tracking prevention at the profile level where the installed WebView2 Runtime supports it; this is intended to reduce common cross-site tracking without deleting first-party cookies or forcing repeated sign-ins. No extension, custom DNS, cloud history, or Google-account integration was added.

The Settings page explains this policy to users. The implementation does not disable SmartScreen or other runtime security features merely to pursue privacy claims.

## Audit fixes included in v3.0.0 refinement

The native/web trust boundary now validates that WebView2 messages originate from Pane Browser's internal `about:blank` pages before executing native commands. External websites cannot use the internal command channel. History no longer requests Google favicons containing visited hostnames; it uses a local badge instead.

Persistence now validates file-versus-directory paths, uses process-specific temporary filenames, cleans up failed temporary writes, removes stale History/session files when those features are disabled, caps application History at 10,000 records, and reports write failures through the Windows debug channel. Query normalization trims whitespace and treats dotted phrases containing spaces as searches. Reopening a closed tab respects disabled tab support.

Clear browsing data now asks for confirmation and reports asynchronous WebView2 completion or partial failure. The update checker runs away from the UI thread. Custom title-bar button hit testing, per-monitor DPI awareness fallback, WebView2 initialization failure shutdown, COM `QueryInterface`, page-content context menus, portable cleanup retry, and installer atomic extraction/shortcut diagnostics were also corrected.

## Distribution

- `PaneBrowser-v3.0.0-portable.exe` — single-file portable launcher.
- `PaneBrowser-v3.0.0-setup.exe` — single-file per-user installer with upgrade and uninstall support.
- `PaneBrowser-v3.0.0-source.zip` — source package, minimal SDK files, documentation, license, and QA reports.

The two launchers embed `PaneBrowserCore.exe` and `WebView2Loader.dll`. The Microsoft WebView2 Runtime itself remains a required Windows dependency and is not bundled.

## Global shortcut focus fix

The shortcut dispatcher now checks the main Windows message queue before messages are translated or dispatched to a focused control. This makes `Ctrl+T`, `Ctrl+W`, `Ctrl+H`, `Ctrl+Shift+T`, `Ctrl+Shift+P`, `Ctrl+L`, `Ctrl+F`, reload, zoom, and the other Pane Browser shortcuts available when focus is in the web page, address bar, tab strip, or internal pages. Normal text-entry shortcuts such as copy, paste, and typing remain delegated to the focused control.

## QA status

The source and launchers pass MinGW-w64 strict syntax checks with application warnings treated as errors after excluding the known SDK pragma and dynamic Windows API cast warnings. The final artifacts are PE32+ Windows x64 GUI executables, and launcher resource sections contain embedded payloads. Privacy hardening is intentionally conservative: normal cookies remain persistent, private download records are not saved, external web messages cannot invoke native commands, History uses no external favicon service, and profile-level Balanced tracking prevention is requested where supported. There is no Wine or Windows WebView2 runtime in the development environment, so real WebView2 rendering, cookie persistence, tracking-prevention behavior, downloads, private profile behavior, custom title-bar interaction, shortcuts, and installer/uninstaller behavior must still be tested on Windows x64 with WebView2 Runtime installed.
