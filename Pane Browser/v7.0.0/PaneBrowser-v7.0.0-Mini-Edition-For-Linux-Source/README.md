# Pane Browser v7.0.0

Pane Browser v7.0.0 focuses on stability hardening, privacy defaults, and lighter Mini Edition while keeping the Regular / Mini split.

## Goals for v7 (as requested)

1. Regular Edition and Mini Edition more stable on Linux across Debian/Ubuntu, Fedora/Red Hat, Arch, and SUSE (source + build notes improved; full matrix still needs real hardware validation).
2. Regular and Mini more stable on Windows (source cleaned, version bumped; Windows binaries must be built on a Windows machine with Visual Studio + WebView2 SDK).
3. Mini Edition for Windows and Linux made as light as practical (ephemeral context, disabled local storage / WebGL / media stream / page cache, smaller attack surface).
4. Privacy improved on both editions (disabled unnecessary WebKit features, custom UA, no persistent storage on Mini, OS credential stores only for passwords).
5. Regular kept feature-rich; Mini stays single-tab, no history, no bookmarks, no Password Manager, no persistent browser data.

## Platform notes

- **Linux**: GTK 3 + WebKitGTK 4.1 + JSON-GLib + libsecret. Official validation targets remain Debian, Ubuntu, Fedora/Red Hat, Arch, SUSE. Package names differ; see `linux/build_v7_linux.sh` and distro notes below.
- **Windows**: Microsoft WebView2. Source includes the WebView2 SDK headers/loader. Portable Mini embeds loader strategy from v6; Regular expects system WebView2 Runtime.

## Password Manager (Regular only)

Same design as v6: heuristic HTTPS login detection, explicit Save / Do not save, OS stores (Windows Credential Manager / Linux Secret Service). Heuristic limitations remain (SSO, iframes, passkeys, multi-step).

## Privacy changes in v7

- Mini: ephemeral WebKit context + disabled local storage, HTML5 DB, offline cache, page cache, media stream, WebGL, encrypted media.
- Regular Linux: same feature disables where reasonable + reduced page cache + custom UA identifying PaneBrowser/7.
- No plaintext password files; passwords stay in OS credential stores.

## Build status in this package

- Linux Regular + Mini **source updated and recompiled** on Ubuntu 24.04 (WebKitGTK 4.1).
- Windows source **version-bumped and path-fixed**; `.exe` / Setup must be produced on Windows.
- Deep static analysis performed (see RELEASE-NOTES). Runtime certification on physical hardware and all four Linux families is still required.

## Distro package hints (Linux)

| Family          | Typical packages                                              |
|-----------------|---------------------------------------------------------------|
| Debian/Ubuntu   | `libgtk-3-dev libwebkit2gtk-4.1-dev libjson-glib-dev libsecret-1-dev` |
| Fedora/RHEL     | `gtk3-devel webkit2gtk4.1-devel json-glib-devel libsecret-devel` |
| Arch            | `gtk3 webkit2gtk-4.1 json-glib libsecret`                     |
| openSUSE        | `gtk3-devel webkit2gtk3-devel json-glib-devel libsecret-devel` (names may vary by release) |

Always verify with `pkg-config --exists webkit2gtk-4.1` (or the distro equivalent) before compiling.
