# Pane Browser v3.0.0 cookie and privacy audit

## Source findings

The normal WebView2 environment is created with a custom user-data folder under `%LOCALAPPDATA%\\Pane Browser\\WebView2` (with a local fallback if the directory cannot be created). WebView2's normal user-data folder is therefore reused across application launches; the source does not delete it at normal shutdown. This is the mechanism that allows cookies, site permissions, cache, local storage, and other profile data to persist.

Each normal tab is created from the shared normal environment without private controller options. Each private tab attempts `ICoreWebView2Environment10::CreateCoreWebView2ControllerOptions`, sets `IsInPrivateModeEnabled=TRUE`, and calls `CreateCoreWebView2ControllerWithOptions`. If that path fails, the source refuses to create the private tab instead of silently falling back to a normal controller. This is the correct privacy direction.

The source currently sets the profile's default download folder but does not explicitly configure cookie policy, tracking-prevention level, password autosave, autofill, or permission prompts. The existing Clear browsing data path can clear profile data only where the installed WebView2 API exposes the profile clear-data interface. The app's own `history.pbh` is separate from WebView2's browsing history and does not itself control cookies.

## Official documentation findings

Microsoft documents that a WebView2 user-data folder stores cookies, permissions, cached resources, and additional browser data, and recommends a writable custom folder for Win32 apps. Microsoft documents that reusing the same UDF across sessions persists user data. Microsoft also documents that profiles share cookies within the profile and that `CookieManager` can inspect or manipulate cookies.

Microsoft documents that `IsInPrivateModeEnabled` is a controller option and that an InPrivate profile is distinct from the default profile. Microsoft documents that profile-level tracking prevention can be configured and persisted, while private mode should remain isolated.

Microsoft also documents that WebView2 itself has required diagnostic data and optional telemetry behavior governed partly by Windows diagnostic settings. Therefore it is inaccurate to promise that no data ever leaves the computer; Pane Browser does not send its own History to a Pane Browser server, but the WebView2 runtime, SmartScreen, websites, and Windows privacy settings are separate layers.

## Preliminary conclusion

Cookies for normal tabs should already persist in v3.0.0 as long as the application reaches WebView2 initialization successfully, keeps the same data directory, and the website sets a persistent cookie. Private tabs should not reuse normal cookies and should not be added to app History or session restore. A narrow v3 refinement is technically worthwhile: expose a clear privacy/convenience policy, request profile tracking prevention at a conservative level where the installed runtime supports it, explicitly keep normal cookies/site storage, make private-tab isolation visible, and improve diagnostics for profile initialization. The current refinement also filters private download records out of persistent storage. It does not add extensions, DNS changes, Google account integration, or a new profile manager.

Potential risks requiring Windows validation are permission/access failure on `%LOCALAPPDATA%`, concurrent processes using the same UDF, sites that deliberately expire cookies or require reauthentication, third-party-cookie restrictions, WebView2 runtime version differences, whether the runtime honors the profile tracking-prevention request, and whether the current private controller path is supported on the user's installed runtime.
