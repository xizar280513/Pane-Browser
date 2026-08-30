## v6.0.0 — August 27, 2026

**Short Description:** A cross-platform expansion adding Linux Regular and Mini implementations, plus a local Password Manager for the Regular Edition.

**Breaking Changes**
- DNS Settings and the experimental DNS-over-HTTPS (DoH) integration were removed entirely from the final release — the source, binaries, Debian package, and source archives contain no DNS settings UI, DNS persistence, DoH browser arguments, or DNS message handlers.
- The Windows credential storage key format changed to a safe encoded representation (older keys remain readable/removable through backward-compatible handling, but the internal storage format changed).

**New Features**
- A local Password Manager for the Regular Edition (accessible from Settings and the Command Palette), supporting both manual credential entry and automatic login detection.
- Each saved credential stores four fields: Website, Password name, Username, and Password.
- Heuristic automatic login detection with an explicit confirmation prompt (Save / Do not save) before a credential is saved.
- Passwords are masked by default, with SVG open-eye/closed-eye controls for temporary reveal.
- Edit, Update password, and Delete per credential, plus a selection checkbox, Select all, Deselect all, and state-aware Delete selected.
- Credential storage via Windows Credential Manager (Windows) and the Secret Service/`libsecret` (Linux).
- Linux Regular and Mini implementations built on GTK 3 and WebKitGTK (replacing WebView2 on this platform), distributed as a portable binary, a Debian package (Regular only), and a source archive.
- Mini Edition was kept deliberately minimal — no Password Manager, DNS Settings, Settings page, history, bookmarks, or profiles were added to this edition.

**Bug Fixes**
- Fixed manual-save validation for HTTPS origins with a trailing slash.
- Removed an incorrect dependency on the previously active tab during manual saves.
- Replaced the fragile tab-delimited credential transport with validated JSON.
- Added field-size and source-context validation before native storage calls.
- Fixed Website and Username display corruption caused by slash delimiters in Windows credential target names.
- Windows startup fix: the Regular Windows prompt no longer depends on `TaskDialogIndirect`, which previously caused an `Entry Point Not Found` startup failure; it now uses the compatible `MessageBoxW` path.
- Added guards for a missing Secret Service backend so listing/removing credentials no longer crashes when no desktop keyring is available.

**Performance / Improvements**
- Replaced eye glyph placeholders with the supplied inline SVG icons and disabled the duplicate native WebView password reveal control; the unnecessary focus outline on saved password fields was also removed.
- Linux binaries are dynamically linked against distribution-provided libraries.
- The release package now consists of exactly 10 official assets (Regular/Mini × Windows/Linux × Portable/Setup/Source, as applicable) with no unrequested release artifacts.
- Additional verification: static regression checks, negative scope checks (confirming Mini contains no Regular-only features), source archive cleanliness checks, Windows x64 PE32+ compilation checks, and headless startup smoke tests for the Linux binaries.
