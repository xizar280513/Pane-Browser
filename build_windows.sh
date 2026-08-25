#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SDK="$ROOT/sdk/webview2/build/native"
CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
"$WINDRES" "$ROOT/PaneBrowser.rc" -O coff -o "$ROOT/PaneBrowser-res.o"
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ \
  -I "$ROOT" -I "$SDK/include" \
  "$ROOT/PaneBrowser.cpp" \
  "$ROOT/PaneBrowser-res.o" \
  "$SDK/x64/WebView2Loader.dll.lib" \
  -lole32 -luuid -luser32 -lgdi32 -lshell32 -lcomctl32 -ladvapi32 \
  -o "$ROOT/PaneBrowser.exe"
cp "$SDK/x64/WebView2Loader.dll" "$ROOT/WebView2Loader.dll"
printf 'Built: %s\n' "$ROOT/PaneBrowser.exe"
