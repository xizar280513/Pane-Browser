#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SDK="$ROOT/sdk/webview2/build/native"
CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
cd "$ROOT"
"$WINDRES" PaneBrowser.rc -O coff -o PaneBrowser-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -I "$ROOT" -I "$SDK/include" PaneBrowser.cpp PaneBrowser-res.o "$SDK/x64/WebView2Loader.dll.lib" -lole32 -luuid -luser32 -lgdi32 -lshell32 -lcomctl32 -ladvapi32 -lshlwapi -lwininet -lurlmon -o PaneBrowserCore.exe
cp "$SDK/x64/WebView2Loader.dll" WebView2Loader.dll
"$WINDRES" portable_launcher.rc -O coff -o portable-launcher-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -I "$ROOT" single_portable_launcher.cpp portable-launcher-res.o -lole32 -luuid -luser32 -lshell32 -o PaneBrowser-Portable.exe
"$WINDRES" installer.rc -O coff -o installer-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -I "$ROOT" single_installer.cpp installer-res.o -lole32 -luuid -luser32 -lshell32 -ladvapi32 -o PaneBrowser-Setup.exe
printf 'Built v3.0.0 artifacts in %s\n' "$ROOT"
file PaneBrowserCore.exe PaneBrowser-Portable.exe PaneBrowser-Setup.exe
