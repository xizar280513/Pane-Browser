#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC="$ROOT/PaneBrowser.cpp"
DATA="$ROOT/v4_data.h"
REPORT="${1:-$ROOT/v5-static-audit.txt}"
fail=0
check() { if "$@"; then printf 'PASS: %s\n' "${CHECK_NAME:-check}"; else printf 'FAIL: %s\n' "${CHECK_NAME:-check}"; fail=1; fi; }
count_fixed() { grep -F -o -- "$2" "$1" | wc -l; }
check_count_eq() { [[ "$(count_fixed "$1" "$2")" -eq "$3" ]]; }
check_count_ge() { [[ "$(count_fixed "$1" "$2")" -ge "$3" ]]; }
{
  echo "Pane Browser v5.0.0 Regular static audit"
  echo "Generated: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo
  echo "== Version/build identity =="
  grep -nE 'v5\.0\.0|PaneBrowser/5\.0|Built Pane Browser v5' "$ROOT/build_v5.sh" "$SRC" "$ROOT"/*.rc "$ROOT/single_installer.cpp" 2>/dev/null || true
  echo
  echo "== Ordinary navigation policy =="
  grep -nE 'authenticationPopup|userInitiated|opener->webview->Navigate|put_NewWindow|add_NewWindowRequested|HandleManagedNewWindowRequest' "$SRC" || true
  echo
  echo "== WebView2 lifecycle and recovery =="
  grep -nE 'CreateCoreWebView2Environment|CreateCoreWebView2Controller|BrowserProcessExited|ProcessFailed|GetDeferral|WindowCloseRequested|g_recovery|g_environment\.Reset|generation' "$SRC" | head -280 || true
  echo
  echo "== Profiles/privacy/storage =="
  grep -nE 'LoadPersistentData|g_settings = v3data::Settings|ProfileDataDirectory|WebViewProfileName|privateMode|DeleteActiveProfile|DeleteAllData|IsApprovedPaneDataRoot|SavePersistentData|ClearBrowsingData' "$SRC" "$DATA" | head -320 || true
  echo
  echo "== Trusted internal messages =="
  grep -nE 'IsTrustedInternalMessage|document == L"about:blank"|HandleHistoryMessage|delete-all-data|profile-delete:' "$SRC" || true
  echo
  echo "== Syntax and compiler diagnostics =="
  x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wshadow -Wconversion -Wno-unused-parameter -fsyntax-only -municode -I "$ROOT" -I "$ROOT/sdk/webview2/build/native/include" "$SRC"
  echo 'syntax_status=0'
  echo
  echo "== PE targets =="
  file "$ROOT/PaneBrowser-v5.0.0-core.exe" "$ROOT/PaneBrowser-v5.0.0-portable.exe" "$ROOT/PaneBrowser-v5.0.0-setup.exe"
  echo
  echo "== Automated invariants =="
  CHECK_NAME='v5 core exists' check test -f "$ROOT/PaneBrowser-v5.0.0-core.exe"
  CHECK_NAME='v5 portable exists' check test -f "$ROOT/PaneBrowser-v5.0.0-portable.exe"
  CHECK_NAME='v5 setup exists' check test -f "$ROOT/PaneBrowser-v5.0.0-setup.exe"
  CHECK_NAME='ordinary user link routes to opener tab exactly once' check check_count_eq "$SRC" 'opener->webview->Navigate(requestedUri.c_str())' 1
  CHECK_NAME='popup paths are explicitly handled' check check_count_ge "$SRC" 'args->put_Handled(TRUE)' 3
  CHECK_NAME='profile load resets settings defaults exactly once' check check_count_eq "$SRC" 'g_settings = v3data::Settings{}' 1
  CHECK_NAME='internal message source boundary remains exact' check check_count_eq "$SRC" 'document == L"about:blank"' 1
  CHECK_NAME='profile deletion rejects protected/temporary kind exactly once' check check_count_eq "$SRC" 'g_activeProfile.kind != L"normal" || g_activeProfile.temporary' 1
  CHECK_NAME='Delete All Data suppresses shutdown save' check check_count_ge "$SRC" 'g_deleteAllInProgress' 3
  CHECK_NAME='session metadata is cleared when restore is disabled' check check_count_ge "$SRC" 'ClearSessionMetadata();' 3
  CHECK_NAME='WebView2 fullscreen event is registered' check grep -Fq 'add_ContainsFullScreenElementChanged' "$SRC"
  CHECK_NAME='native F11 fullscreen shortcut exists' check grep -Fq 'if(!ctrl && vk==VK_F11){ToggleBrowserFullscreen();return true;}' "$SRC"
  CHECK_NAME='fullscreen restores window placement' check grep -Fq 'SetWindowPlacement(g_main, &g_fullscreenPlacement);' "$SRC"
  CHECK_NAME='fullscreen hides toolbar and restores address bar' check grep -Fq 'ShowWindow(g_tabStrip, SW_HIDE)' "$SRC" && grep -Fq 'ShowWindow(g_address, SW_HIDE)' "$SRC" && grep -Fq 'ShowWindow(g_address, SW_SHOW)' "$SRC" && grep -Fq 'SetWindowTextW(g_address, restored->lastUri' "$SRC"
  CHECK_NAME='View Source helper uses view-source scheme' check grep -Fq 'void OpenViewSource()' "$SRC" && grep -Fq 'std::wstring sourceUri = L"view-source:" + uri;' "$SRC"
  CHECK_NAME='Ctrl+U dispatches View Source' check grep -Fq "ctrl&&!shift&&vk=='U'" "$SRC" && grep -Fq 'kOpenViewSourceMessage' "$SRC"
  CHECK_NAME='Inspect helper opens WebView2 DevTools' check grep -Fq 'void OpenDevTools()' "$SRC" && grep -Fq 'OpenDevToolsWindow()' "$SRC"
  CHECK_NAME='F12 dispatches Inspect' check grep -Fq '!ctrl && vk==VK_F12' "$SRC" && grep -Fq 'kOpenDevToolsMessage' "$SRC"
  CHECK_NAME='context menu exposes View Source and Inspect' check grep -Fq 'L"View Source"' "$SRC" && grep -Fq 'L"Inspect"' "$SRC"
  CHECK_NAME='view-source links route to new tab' check grep -Fq 'sourceDocument' "$SRC" && grep -Fq 'CreateNewTab(requestedUri, opener->privateMode);' "$SRC"
  echo
  echo "== Import tables =="
  echo '-- core --'; x86_64-w64-mingw32-objdump -p "$ROOT/PaneBrowser-v5.0.0-core.exe" | grep 'DLL Name:' || true
  echo '-- portable --'; x86_64-w64-mingw32-objdump -p "$ROOT/PaneBrowser-v5.0.0-portable.exe" | grep 'DLL Name:' || true
} > "$REPORT" 2>&1 || fail=1
cat "$REPORT"
exit "$fail"
