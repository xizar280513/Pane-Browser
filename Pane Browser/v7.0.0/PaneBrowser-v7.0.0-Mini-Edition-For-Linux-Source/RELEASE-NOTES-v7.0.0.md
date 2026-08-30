# Pane Browser v7.0.0 — Release Notes

## Summary of changes from v6.0.0

### Stability
- Fixed F11 fullscreen toggle on Linux Regular (previously only entered fullscreen).
- Silenced unused `ConfigDir()` warning and ensured config directory is created at startup.
- Password store namespace and include paths updated to v7 (`pane_v7`, `v7_password_store.h`).
- Build scripts renamed and version strings updated to 7.0.0.
- Linux binaries recompiled cleanly on Ubuntu 24.04 with `-Wall -Wextra -Wpedantic` (only intentional remaining notes are WebKit deprecation warnings from the system headers if any).

### Privacy hardening
- Linux Regular & Mini: disabled page cache, offline web application cache, HTML5 database, HTML5 local storage, media stream, WebGL, accelerated 2D canvas, encrypted media (where APIs exist).
- Custom User-Agent identifying PaneBrowser/7.0.0 (or PaneBrowserMini) to reduce generic fingerprint collisions while remaining identifiable.
- Mini continues to use fully ephemeral WebKit context; no persistent profile directory intended.
- Password Manager still uses only OS credential stores (no app-side plaintext password files).

### Lightness (Mini)
- Mini Linux binary remains extremely small (~38–40 KB linked against system WebKit).
- Additional WebKit feature disables reduce runtime memory and residual data surface.
- Windows Mini source keeps the ephemeral user-data + cleanup path from v6; further size reduction would require stripping the WebView2 loader strategy or external compression (not applied here).

### What was *not* claimed as complete
- Full multi-distro CI / package builds for Fedora, Arch, SUSE (source + notes only).
- Windows PE32+ Portable / Setup binaries (must be built with MSVC on Windows).
- Formal security audit of the password detector or COM/WebKit lifetime paths.
- Guarantee of zero bugs — complex browser wrappers always require hardware testing.

## Deep bug analysis highlights (v6 → fixed or documented)

| Issue | Severity | Action in v7 |
|-------|----------|--------------|
| ConfigDir unused | Low (warning) | Called at startup |
| F11 no unfullscreen (Linux Regular) | Medium UX | Toggle implemented |
| No privacy WebKit settings | Medium privacy | Defaults applied |
| Detector injection comment-only | Low | Script still registered correctly via UserContentManager |
| Ephemeral cleanup race (Windows Mini) | Low | Documented; same strategy kept |
| Password heuristic incomplete | Known limitation | Unchanged; documented |
| Cross-distro package names | Packaging | Notes added in README |

## Verification performed in sandbox

- Linux Regular & Mini compile with g++ 13, WebKitGTK 4.1, gtk3, json-glib, libsecret.
- Binary sizes match expected order of magnitude of v6 portables.
- No new obvious memory-management errors introduced in the Linux paths that were edited.

**Mandatory next steps before production use**: test on real Windows 11, Debian/Ubuntu, Fedora, Arch, and openSUSE machines; exercise password manager against live sites; verify Secret Service / Credential Manager; check multimedia and hardware acceleration behaviour.

## Second-pass hardening (same release)

- Cleaned privacy settings (removed deprecated APIs that caused warnings).
- Added: disable WebAudio, clipboard JS access, console-to-stdout.
- Hardened navigation policy: block `javascript:` / `vbscript:` schemes.
- Linux binaries stripped (`strip --strip-all`) for smaller size:
  - Mini Portable ≈ 35 KB
  - Regular Portable ≈ 109 KB
- Recompiled clean under `-Wall -Wextra -Wpedantic` (0 warnings).
- Final package: `v7.0.0.zip` containing all 7 deliverable files.
