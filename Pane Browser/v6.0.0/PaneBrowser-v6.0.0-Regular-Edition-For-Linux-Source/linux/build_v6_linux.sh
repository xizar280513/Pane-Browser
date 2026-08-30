#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
OUT="$ROOT/../../pane-browser-v6-build/linux"
CXX="${CXX:-g++}"
mkdir -p "$OUT"
cd "$ROOT"
CFLAGS="-std=c++17 -O2 -Wall -Wextra -Wpedantic"
LIBS="$(pkg-config --cflags --libs gtk+-3.0 webkit2gtk-4.1 json-glib-1.0 libsecret-1)"
if [ -f "$ROOT/pane_browser_linux.cpp" ]; then
  "$CXX" $CFLAGS -I"$ROOT/../shared" pane_browser_linux.cpp $LIBS -o "$OUT/PaneBrowser-v6.0.0-Regular-Edition-For-Linux-Portable"
fi
if [ -f "$ROOT/pane_browser_linux_mini.cpp" ]; then
  "$CXX" $CFLAGS pane_browser_linux_mini.cpp $LIBS -o "$OUT/PaneBrowser-v6.0.0-Mini-Edition-For-Linux-Portable"
fi
file "$OUT"/PaneBrowser-v6.0.0-*-Edition-For-Linux-Portable 2>/dev/null || true
