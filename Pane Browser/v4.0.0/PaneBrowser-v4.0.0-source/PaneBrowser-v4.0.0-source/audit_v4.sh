#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC="$ROOT/PaneBrowser.cpp"
DATA="$ROOT/v4_data.h"
REPORT="${1:-$ROOT/v4-static-audit.txt}"
{
  echo "Pane Browser v4.0.0 static audit"
  echo "Generated: $(date -u +%Y-%m-%dT%H:%M:%SZ)"
  echo
  echo "== Build/source identity =="
  grep -nE 'Pane Browser v4|PaneBrowser/4|v4\.0\.0|Built Pane Browser' "$ROOT/build_v4.sh" "$SRC" "$ROOT"/*.rc 2>/dev/null || true
  echo
  echo "== Potential version drift =="
  grep -nE 'v3\.0\.0|3\.0|PaneBrowser/3\.0|Built v3' "$SRC" "$ROOT/build_v4.sh" "$ROOT"/*.rc 2>/dev/null || true
  echo
  echo "== WebView2 lifecycle handlers =="
  grep -nE 'CreateCoreWebView2Environment|CreateCoreWebView2Controller|CreateCoreWebView2ControllerWithOptions|BrowserProcessExited|ProcessFailed|WindowCloseRequested|NewWindowRequested|GetDeferral|put_NewWindow|Close\(\)' "$SRC" || true
  echo
  echo "== Storage/profile/privacy paths =="
  grep -nE 'ProfileDataDirectory|WebViewUserDataFolder|LoadProfileContext|SwitchActiveProfile|SavePersistentData|LoadPersistentData|privateMode|authenticationWindow|ClearBrowsingData|ClearBrowsingDataAll|cookies|token' "$SRC" "$DATA" | head -260 || true
  echo
  echo "== Feature routing =="
  grep -nE 'Command Palette|Site Data Center|Workspace|Quick Notes|Reading Mode|Split View|verticalTabs|tabSleeping|privacyMode|profile-create|profile-switch|permission:' "$SRC" | head -260 || true
  echo
  echo "== Empty/stale callback risks =="
  grep -nE 'g_tabs\.erase|g_tabs\.clear|g_environment\.Reset|g_recovery|g_splitSecondTab|g_activeTab' "$SRC" | head -260 || true
  echo
  echo "== Source/build syntax =="
  x86_64-w64-mingw32-g++ -std=c++17 -Wall -Wextra -Wshadow -Wconversion -Wno-unused-parameter -fsyntax-only -municode -I "$ROOT" -I "$ROOT/sdk/webview2/build/native/include" "$SRC"
  echo "syntax_status=0"
  echo
  echo "== Binary targets =="
  file "$ROOT/PaneBrowserCore.exe" "$ROOT/PaneBrowser-Portable.exe" "$ROOT/PaneBrowser-Setup.exe"
} > "$REPORT" 2>&1
cat "$REPORT"
