# v3.0.0 UI refinement audit

The feature toolbar was reorganized without changing version number. Back, Forward, Refresh, Home, and the address bar remain in the main toolbar. History, Bookmarks, Settings, Private, Downloads, and Find are now hidden legacy child buttons routed through a visible owner-drawn vertical-ellipsis button at the right edge of the address bar. The popup menu sends the same command IDs used by existing controls, reducing regression risk.

Settings now persists `searchEngine` and `customSearchUrl`. The built-in catalog contains Google, Bing, DuckDuckGo, Brave Search, Startpage, Mojeek, Swisscows, Yahoo, Baidu, Naver, Yandex, Ecosia, Qwant, Seznam, Ask, AOL, Kagi, MetaGer, SearXNG, You.com, Perplexity, Yep, Sogou, 360 Search, Rambler, Daum, Goo, Excite, Dogpile, Gibiru, and WolframAlpha, plus a Custom URL entry. A custom URL may use `{query}` or `%s`; if neither marker is present the encoded query is appended.

Settings includes a complete table of implemented keyboard shortcuts. `Ctrl+T`, `Ctrl+W`, `Ctrl+H`, and `Ctrl+Shift+T` are present; the latter reopens the most recently closed normal HTTP(S) tab and never reopens private tabs. Startup session restore continues to restore only normal HTTP(S) tabs, and private records are excluded from `session.pbs`.

The v3 core and launcher pass MinGW-w64 syntax checks after the refinement. Runtime UI and web content still require real Windows/WebView2 validation.
