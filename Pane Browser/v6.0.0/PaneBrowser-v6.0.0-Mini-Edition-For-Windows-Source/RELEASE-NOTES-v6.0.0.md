# Pane Browser v6.0.0

## New in v6

Pane Browser v6.0.0 introduces a cross-platform Linux backend based on GTK and WebKitGTK, while Windows continues to use Microsoft WebView2. The supported Linux release targets are Debian, Ubuntu, Fedora/Red Hat, Arch, and SUSE.

Regular Edition adds a local Password Manager. HTTPS login-form detection asks the user whether to Save or not save a detected credential. The Password Manager is also available from Settings and Command Palette, with manual save, saved-entry search, on-demand View password, Edit, per-entry checkboxes, Select all, Deselect all, and Delete selected controls. Passwords are masked by default and use inline open-eye/closed-eye SVG controls for temporary reveal. Manual and automatic credential messages use validated JSON, and website origins are canonicalized so a URL with a trailing slash is accepted. Windows uses Windows Credential Manager, while Linux uses the desktop Secret Service through libsecret. Private and internal pages are excluded.

Mini Edition remains separate from the Regular feature set: it stays single-tab, minimal, and private, without history, persistent browser data, bookmarks, Password Manager, or other v6 feature additions.

## Security and compatibility notes

The password detector is heuristic and may not detect every provider's login flow, including some SSO, passkey, iframe, and multi-step authentication designs. The browser requires explicit confirmation before saving. Real-world security review and testing on physical Windows and Linux systems remain necessary.

Linux packages depend on the target distribution's GTK, WebKitGTK, JSON-GLib, and Secret Service packages. WebKitGTK is not embedded into the portable Linux binary. Windows packages require the Microsoft WebView2 Runtime; the Regular source package includes the WebView2 SDK and the Mini portable build embeds the loader in its launcher.

## Verification

The v6 Linux Regular and Mini binaries compile on Ubuntu 24.04 and pass virtual-display/DBus startup smoke tests. Windows Regular and Mini compile as x64 PE32+ executables. The sandbox cannot certify behavior on physical Windows 11 hardware or every supported Linux distribution.
