# Pane Browser v4.0.0 — v3 baseline audit

## Scope and status

This is a baseline and compatibility audit before v4 implementation. No v4 source or binary has been created, and no v3 source has been modified during this audit.

## Existing v3 baseline

The v3 baseline is a native C++17/Win32 application using Microsoft WebView2. `PaneBrowser.cpp` contains the native window, toolbar, custom tab strip, internal HTML pages, WebView2 controller lifecycle, navigation, downloads, private mode, settings, shortcuts, update checking, and security boundary logic. `v3_data.h` contains application-owned path helpers and escaped UTF-8 line-based persistence for History, bookmarks, session, downloads, and settings.

The existing v3 data model is global to the application: one application data directory, one normal WebView2 UDF, one global settings object, one global History/bookmark/download/session collection, and a vector of BrowserTab objects. It does not yet model multiple named profiles, workspaces, tab groups, per-site settings, site-data inspection, backup manifests, or profile-scoped recovery journals.

## Positive invariants to preserve

1. Normal WebView2 data uses a persistent writable custom UDF under the user's local application data, allowing ordinary cookies and site sign-in to persist.
2. Private tabs use `IsInPrivateModeEnabled=TRUE` when supported and do not silently fall back to a normal controller.
3. Private tabs and private downloads are excluded from application History, session restore, and persistent download records.
4. Internal WebView2 web-message commands are accepted only from trusted internal `about:blank` pages; external pages must not reach native commands.
5. History no longer leaks visited hostnames to an external favicon service.
6. Persistence uses process-specific temporary files and replacement writes; path-vs-directory validation and stale-record cleanup are already part of the v3 hardening direction.
7. History has a cap of 10,000 records.
8. Destructive clear-data actions require confirmation and report asynchronous/partial failures.
9. Global keyboard shortcuts are processed from the native message loop so they do not depend on page focus.
10. WebView2 initialization failure closes the application rather than leaving a blank unusable window.
11. Portable cleanup is best effort and must account for WebView2 child processes still holding files.
12. Installer upgrade/uninstall preserves user data by policy and must not silently destroy profile data.
13. DPI awareness, COM `QueryInterface` behavior, context menu integration, and atomic launcher extraction are treated as regression-sensitive areas.

## Baseline risks relevant to v4

The v3 global state model will become unsafe if v4 adds profiles, workspaces, split view, sleeping tabs, and asynchronous recovery without introducing explicit ownership and lifecycle boundaries. Global `g_settings`, `g_history`, `g_tabs`, `g_environment`, and data-directory helpers must be scoped or routed through a profile/workspace manager rather than duplicated ad hoc.

WebView2 events are registered per controller/webview but tokens are not broadly retained for explicit removal. V4 will need a formal TabController lifecycle so sleeping, closing, profile switching, and crash recovery cannot deliver callbacks into destroyed or replaced tab state.

The current `NewWindowRequested` design is a compatibility concern for authentication. It handles new-window requests and navigates the existing sender. V4's Authentication Window Manager must distinguish ordinary target-blank navigation from popup/OAuth/SAML flows and provide a separate browsing context when required.

The current single portable launcher extracts into a temporary directory. This is incompatible with persistent profiles if the core derives its UDF from its executable path. V4 portable distribution must separate the application payload location from the persistent profile-data location; otherwise every launch creates a new profile and breaks cookie/login convenience.

The current line-based storage format is simple and inspectable but not sufficient by itself for transactional multi-profile/workspace updates, schema migrations, backup manifests, conflict detection, or crash journaling. V4 needs versioned records, atomic transactions, checksums or validation, and recovery from interrupted writes.

The current application-owned History and WebView2's own browsing data are separate layers. A v4 Site Data Center must clearly distinguish app records from WebView2 cookies, DOM storage, cache, permissions, and downloads, and must not claim that clearing one layer clears the other unless verified.

The current private-mode isolation is controller-based and dependent on installed WebView2 API support. V4 must preserve fail-closed behavior, make the active profile/private state visible, and prevent private tabs from entering workspace/session/backup data.

## V4 compatibility boundaries

V4 should preserve the v3 visual language and core navigation controls unless the new tab/workspace features require additive UI. Existing shortcuts, History, Bookmarks, Downloads, Settings, themes, private tabs, session restore, and installer/portable distribution remain compatibility requirements. New profile and workspace features must not change normal cookies into ephemeral data.

The v4 implementation should be treated as a large architecture change, not a small patch. The design must be completed and threat-modeled before implementation, followed by static audits and real Windows/WebView2 validation. Linux cross-compilation cannot prove popup authentication, storage partitioning behavior, WebView2 profile isolation, permission prompts, crash recovery, sleeping/wake behavior, DPI, or installer operation.

## Required next design decisions

- Define profile identities and data-root layout, including normal, guest, private, and portable modes.
- Define how auth popup windows share or isolate the selected profile and how redirect state survives.
- Define tab/workspace/group ownership and persistence boundaries.
- Define WebView2 controller/environment lifecycle for sleeping and crash recovery.
- Define permission and site-data API capability fallbacks by WebView2 runtime version.
- Define backup format, encryption/privacy policy, atomic restore, and exclusion of private data.
- Define per-site policy precedence: global privacy mode, profile policy, workspace policy, site policy, and temporary session overrides.
- Define safe limits for tabs, workspaces, history, notes, backups, and concurrent profiles.
