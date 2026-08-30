#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SDK="$ROOT/sdk/webview2/build/native"
CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
cd "$ROOT"
CORE="PaneBrowserMiniCore-v7.0.0.exe"
FINAL="PaneBrowser-v7.0.0-Mini-Edition-For-Windows-Portable.exe"
rm -f "$FINAL" "$CORE" mini_launcher-res.o mini_heap.o "$SDK/x64/WebView2Loader.dll.lz4"
python3 compress_resource.py "$SDK/x64/WebView2Loader.dll" "$SDK/x64/WebView2Loader.dll.lz4" > /tmp/pane-browser-v7-win-mini-compress.log
"$CXX" -std=c++17 -Os -s -static-libgcc -static-libstdc++ -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows -I. -I"$SDK/include" -I"$ROOT/../../shared" -c mini_heap.cpp -o mini_heap.o
"$CXX" -std=c++17 -Os -s -static-libgcc -static-libstdc++ -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows -I. -I"$SDK/include" -I"$ROOT/../../shared" PaneBrowserMini.cpp mini_heap.o -L"$SDK/x64" -lWebView2Loader -lole32 -luuid -luser32 -lshell32 -ladvapi32 -lcrypt32 -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--strip-all -Wl,--no-insert-timestamp -o "$CORE"
sed "s/PaneBrowserMiniCore.exe/$CORE/g" mini_launcher.rc > .v7-mini-launcher.rc
"$WINDRES" .v7-mini-launcher.rc -O coff -o mini_launcher-res.o
"$CXX" -std=c++17 -Os -s -static-libgcc -static-libstdc++ -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows mini_launcher.cpp mini_launcher-res.o -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--strip-all -Wl,--no-insert-timestamp -o "$FINAL"
rm -f mini_launcher-res.o "$CORE" mini_heap.o "$SDK/x64/WebView2Loader.dll.lz4" .v7-mini-launcher.rc
file "$FINAL"
