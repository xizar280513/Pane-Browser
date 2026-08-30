# Pane Browser v6.0.0

Pane Browser v6.0.0 adds a local Password Manager and Linux support while preserving the existing Regular Edition and Mini Edition split.

## Platform targets

The Linux backend uses GTK 3 and WebKitGTK 4.1. Official validation targets are Debian, Ubuntu, Fedora/Red Hat, Arch, and SUSE. Debian and Ubuntu are separate release targets even though Ubuntu is derived from Debian because their package versions, release cadence, security integration, and default desktop environments differ.

Windows builds continue to use Microsoft WebView2. Linux builds use the WebKitGTK system runtime rather than WebView2Loader.dll.

## Password Manager

The password detector watches HTTPS login forms and asks before saving credentials. The user sees two choices: Save or Do not save. Private and internal pages are excluded. The Password Manager can also be opened from Settings and, in Regular Edition, Command Palette to add credentials manually, search saved entries, view a password on demand, or delete an entry.

Windows stores credentials in Windows Credential Manager. Linux stores credentials in the desktop Secret Service through libsecret. The application does not write passwords to a plaintext settings file. Passwords are masked by default and are only placed into the local Password Manager page when the user chooses View password.

Password detection is heuristic and provider-dependent. Some multi-step, iframe, passkey, SSO, or JavaScript-only login flows may not produce a save prompt. The feature must not be treated as a replacement for a dedicated audited password manager until additional security review and real-device testing are complete.

## Editions

Regular Edition retains tabs, history, profiles, privacy controls, Settings, Command Palette, the existing v5 browser features, and the v6 Password Manager. Mini Edition remains a separate minimal and private single-tab browser: it has no history, no persistent browser data, no bookmarks, no Password Manager, and no Regular Edition additions.

## Testing status

The Linux Regular and Mini binaries compile on Ubuntu 24.04 sandbox and pass Xvfb/DBus startup smoke tests. Windows Regular and Mini compile as x64 PE32+ binaries using the bundled WebView2 SDK. Runtime verification on physical Windows 11, the five official Linux families, desktop keyrings, hardware acceleration, and multimedia remains required.
