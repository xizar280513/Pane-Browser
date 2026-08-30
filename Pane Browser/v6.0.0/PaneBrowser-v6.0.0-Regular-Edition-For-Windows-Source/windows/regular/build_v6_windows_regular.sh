#!/usr/bin/env bash
set -euo pipefail
ROOT="$(cd "$(dirname "$0")" && pwd)"
SDK="$ROOT/sdk/webview2/build/native"
CXX="${CXX:-x86_64-w64-mingw32-g++}"
WINDRES="${WINDRES:-x86_64-w64-mingw32-windres}"
cd "$ROOT"
rm -f PaneBrowser-v6.0.0-Regular-core.exe PaneBrowser-v6.0.0-Regular-portable.exe PaneBrowser-v6.0.0-Regular-setup.exe PaneBrowser-res.o portable-launcher-res.o installer-res.o
"$WINDRES" PaneBrowser.rc -O coff -o PaneBrowser-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp -I "$ROOT" -I "$SDK/include" -I "$ROOT/../../shared" PaneBrowser.cpp PaneBrowser-res.o "$SDK/x64/WebView2Loader.dll.lib" -lole32 -luuid -luser32 -lgdi32 -lshell32 -lcomctl32 -ladvapi32 -lshlwapi -lwininet -lurlmon -lcrypt32 -o PaneBrowser-v6.0.0-Regular-core.exe
cp "$SDK/x64/WebView2Loader.dll" WebView2Loader.dll
sed 's/PaneBrowser-v5\.0\.0-core\.exe/PaneBrowser-v6.0.0-Regular-core.exe/g; s/PaneBrowser-v5\.0\.0-portable\.exe/PaneBrowser-v6.0.0-Regular-portable.exe/g' portable_launcher.rc > .v6-portable.rc
"$WINDRES" .v6-portable.rc -O coff -o portable-launcher-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp -I "$ROOT" single_portable_launcher.cpp portable-launcher-res.o -lole32 -luuid -luser32 -lshell32 -o PaneBrowser-v6.0.0-Regular-portable.exe
sed 's/PaneBrowser-v5\.0\.0-core\.exe/PaneBrowser-v6.0.0-Regular-core.exe/g; s/PaneBrowser-v5\.0\.0-setup\.exe/PaneBrowser-v6.0.0-Regular-setup.exe/g' installer.rc > .v6-installer.rc
"$WINDRES" .v6-installer.rc -O coff -o installer-res.o
"$CXX" -std=c++17 -O2 -municode -mwindows -static-libgcc -static-libstdc++ -Wl,--no-insert-timestamp -I "$ROOT" single_installer.cpp installer-res.o -lole32 -luuid -luser32 -lshell32 -ladvapi32 -o PaneBrowser-v6.0.0-Regular-setup.exe
rm -f PaneBrowser-res.o portable-launcher-res.o installer-res.o .v6-portable.rc .v6-installer.rc
file PaneBrowser-v6.0.0-Regular-core.exe PaneBrowser-v6.0.0-Regular-portable.exe PaneBrowser-v6.0.0-Regular-setup.exe
