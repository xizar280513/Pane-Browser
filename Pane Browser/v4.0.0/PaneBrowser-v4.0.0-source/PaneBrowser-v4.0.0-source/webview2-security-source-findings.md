# External security source findings for v3.0.0 audit

Source: Microsoft Learn, Develop secure WebView2 apps
URL: https://learn.microsoft.com/en-us/microsoft-edge/webview2/concepts/security

Microsoft explicitly advises that all web content be treated as insecure, that the host always check the origin of the document inside WebView2, and that web messages and host-object parameters be validated before consumption. It also recommends checking the WebView2 `Source` when processing `WebMessageReceived`, and disabling `IsWebMessageEnabled` when web messaging is not needed.

Source: Microsoft Learn, Interop of native and web code
URL: https://learn.microsoft.com/en-us/microsoft-edge/webview2/how-to/communicate-btwn-web-native

Microsoft's WebView2 sample checks `args->get_Source()` and rejects messages whose source is not the expected internal sample URI before executing commands. This is directly relevant because Pane Browser's `HandleHistoryMessage` currently accepts command strings without checking the sending document origin.

Audit implication: Pane Browser's public-web pages can potentially invoke native host commands exposed through `chrome.webview.postMessage`, including navigation, History deletion, clear browsing data, bookmark actions, Settings changes, and private-tab actions. This is a high-severity host/web trust-boundary finding. A future fix must restrict command handling to the application's internal `about:blank`/NavigateToString pages (or a dedicated internal origin) and treat malformed/unknown messages as no-ops. External site pages should not receive internal command privileges.
