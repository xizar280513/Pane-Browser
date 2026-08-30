#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SDK="$ROOT/sdk/webview2/build/native"
BUILD="$ROOT/build"
DIST="$ROOT/dist"
CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
WEBVIEW2_LOADER_DLL="${WEBVIEW2_LOADER_DLL:-$SDK/x64/WebView2Loader.dll}"
mkdir -p "$BUILD" "$DIST"
if [[ ! -f "$WEBVIEW2_LOADER_DLL" ]]; then
  echo "Missing WebView2Loader.dll: $WEBVIEW2_LOADER_DLL" >&2
  exit 1
fi

"$WINDRES" "$ROOT/PaneBrowser.rc" -O coff -o "$BUILD/PaneBrowser-v1-res.o"
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ \
  -I "$ROOT" -I "$SDK/include" \
  "$ROOT/PaneBrowser.cpp" "$BUILD/PaneBrowser-v1-res.o" \
  "$SDK/x64/WebView2Loader.dll.lib" \
  -lole32 -luuid -luser32 -lgdi32 -lshell32 -lcomctl32 -ladvapi32 \
  -o "$BUILD/PaneBrowser-v1.0.0-core.exe"
cp "$WEBVIEW2_LOADER_DLL" "$BUILD/WebView2Loader.dll"

"$WINDRES" "$ROOT/portable_v1.rc" -O coff -o "$BUILD/portable-v1-res.o"
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ \
  -I "$ROOT" "$ROOT/single_portable_launcher_v1.cpp" "$BUILD/portable-v1-res.o" \
  -o "$DIST/PaneBrowser-v1.0.0-portable.exe"

file "$BUILD/PaneBrowser-v1.0.0-core.exe" "$DIST/PaneBrowser-v1.0.0-portable.exe"
printf 'Built v1.0.0 core and single portable EXE in %s and %s\n' "$BUILD" "$DIST"
