#!/usr/bin/env bash
set -euo pipefail

CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
SDK="sdk/webview2/build/native"
CORE="PaneBrowserMiniCore.exe"
FINAL="PaneBrowser-v5.0.0-MiniEdition-portable.exe"
rm -f "$FINAL" "$CORE" mini_launcher-res.o mini_heap.o sdk/webview2/build/native/x64/WebView2Loader.dll.lz4
python3 compress_resource.py sdk/webview2/build/native/x64/WebView2Loader.dll sdk/webview2/build/native/x64/WebView2Loader.dll.lz4 >/tmp/pane-browser-v5-mini-compress.log

"$CXX" -std=c++17 -Os -s -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows -I. -I"$SDK/include" -c mini_heap.cpp -o mini_heap.o
"$CXX" -std=c++17 -Os -s -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows -I. -I"$SDK/include" PaneBrowserMini.cpp mini_heap.o \
  -L"$SDK/x64" -lWebView2Loader -lole32 -luuid -luser32 -lshell32 -ladvapi32 \
  -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--strip-all -Wl,--no-insert-timestamp -o "$CORE"

"$WINDRES" mini_launcher.rc -O coff -o mini_launcher-res.o
"$CXX" -std=c++17 -Os -s -fno-rtti -fno-exceptions -ffunction-sections -fdata-sections -municode -mwindows mini_launcher.cpp mini_launcher-res.o \
  -Wl,--subsystem,windows -Wl,--gc-sections -Wl,--strip-all -Wl,--no-insert-timestamp -o "$FINAL"

rm -f mini_launcher-res.o "$CORE" mini_heap.o sdk/webview2/build/native/x64/WebView2Loader.dll.lz4
rm -f PaneBrowser-v5.0.0-portable.exe PaneBrowser-v5.0.0-setup.exe
file "$FINAL"
