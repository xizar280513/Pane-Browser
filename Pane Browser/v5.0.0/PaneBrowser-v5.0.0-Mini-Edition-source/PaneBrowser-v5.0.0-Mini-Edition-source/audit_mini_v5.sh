#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SRC="$ROOT/PaneBrowserMini.cpp"
OUT="${1:-$ROOT/mini-static-audit.txt}"
fail=0
check() { if "$@"; then printf 'PASS: %s\n' "${CHECK_NAME:-check}"; else printf 'FAIL: %s\n' "${CHECK_NAME:-check}"; fail=1; fi; }
{
  echo "Pane Browser v5.0.0 MiniEdition static audit"
  echo "== Source invariants =="
  CHECK_NAME='Mini source exists' check test -f "$SRC"
  CHECK_NAME='single-file output naming exists' check grep -Fq 'PaneBrowser-v5.0.0-MiniEdition-portable.exe' "$ROOT/build_v5_mini.sh"
  CHECK_NAME='WebView2 fullscreen event is registered' check grep -Fq 'add_ContainsFullScreenElementChanged' "$SRC"
  CHECK_NAME='native F11 state and dispatch exist' check grep -Fq 'g_f11Fullscreen' "$SRC" && grep -Fq 'msg.wParam == VK_F11' "$SRC"
  CHECK_NAME='WebView2 accelerator F11 handler exists' check grep -Fq 'ICoreWebView2AcceleratorKeyPressedEventHandler' "$SRC" && grep -Fq 'add_AcceleratorKeyPressed' "$SRC" && grep -Fq 'COREWEBVIEW2_KEY_EVENT_KIND_KEY_DOWN' "$SRC"
  CHECK_NAME='Windows application theme preference is read' check grep -Fq 'AppsUseLightTheme' "$SRC"
  CHECK_NAME='Windows theme changes are handled' check grep -Fq 'WM_SETTINGCHANGE' "$SRC" && grep -Fq 'WM_THEMECHANGED' "$SRC"
  CHECK_NAME='Home CSS theme bridge exists' check grep -Fq 'setPaneTheme' "$SRC"
  CHECK_NAME='native theme brushes are updated' check grep -Fq 'g_controlBrush' "$SRC" && grep -Fq 'UpdateSystemTheme' "$SRC"
  CHECK_NAME='alpha status label is absent' check bash -c '! grep -Rqi "early[[:space:]]+alpha" "$1" "$2"' _ "$SRC" "$ROOT/README.md"
  CHECK_NAME='fullscreen restores window placement' check grep -Fq 'SetWindowPlacement(g_main, &g_fullscreenPlacement)' "$SRC"
  CHECK_NAME='fullscreen hides MiniEdition chrome' check grep -Fq 'ShowWindow(g_address, SW_HIDE)' "$SRC"
  CHECK_NAME='temporary WebView2 data folder is used' check grep -Fq 'CreateEphemeralUserDataFolder' "$SRC"
  CHECK_NAME='regular outputs are removed by Mini build' check grep -Fq 'PaneBrowser-v5.0.0-portable.exe PaneBrowser-v5.0.0-setup.exe' "$ROOT/build_v5_mini.sh"
  echo "== Build output =="
  CHECK_NAME='MiniEdition executable exists' check test -f "$ROOT/PaneBrowser-v5.0.0-MiniEdition-portable.exe"
  file "$ROOT/PaneBrowser-v5.0.0-MiniEdition-portable.exe"
  echo "== Scope =="
  if find "$ROOT" -maxdepth 1 -type f \( -name 'PaneBrowser-v5.0.0-portable.exe' -o -name 'PaneBrowser-v5.0.0-setup.exe' \) | grep -q .; then
    echo 'FAIL: Regular outputs are present in MiniEdition root'; fail=1
  else
    echo 'PASS: Regular outputs are absent from MiniEdition root'
  fi
} > "$OUT" 2>&1 || fail=1
cat "$OUT"
exit "$fail"
