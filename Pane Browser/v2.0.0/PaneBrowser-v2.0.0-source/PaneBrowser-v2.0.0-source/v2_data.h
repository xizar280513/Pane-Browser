#pragma once
#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif
#include <windows.h>
#include <shlobj.h>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <algorithm>

namespace v2data {

struct V2HistoryEntry {
    std::wstring uri;
    std::wstring time;
};

struct V2Bookmark {
    std::wstring title;
    std::wstring uri;
};

struct V2Settings {
    std::wstring theme = L"system";
    bool tabsEnabled = true;
    bool customTitleBar = false;
    bool persistHistory = true;
    std::wstring searchEngine = L"startpage";
};

inline std::wstring ModuleDirectory() {
    wchar_t path[32768] = {};
    DWORD n = GetModuleFileNameW(nullptr, path, static_cast<DWORD>(sizeof(path) / sizeof(path[0])));
    if (!n || n >= sizeof(path) / sizeof(path[0])) return L".";
    std::wstring full(path, n);
    const size_t slash = full.find_last_of(L"\\/");
    return slash == std::wstring::npos ? L"." : full.substr(0, slash);
}

inline std::wstring Join(const std::wstring& a, const std::wstring& b) {
    return a + (a.empty() || a.back() == L'\\' ? L"" : L"\\") + b;
}

inline std::wstring DataDirectory() {
    wchar_t local[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, local)) && local[0]) {
        std::wstring root = Join(local, L"Pane Browser");
        CreateDirectoryW(root.c_str(), nullptr);
        return root;
    }
    std::wstring root = Join(ModuleDirectory(), L"PaneBrowser.Data");
    CreateDirectoryW(root.c_str(), nullptr);
    return root;
}

inline std::wstring PrivateDirectory() {
    wchar_t temp[MAX_PATH] = {};
    DWORD n = GetTempPathW(static_cast<DWORD>(sizeof(temp) / sizeof(temp[0])), temp);
    std::wstring root = n ? std::wstring(temp, n) : ModuleDirectory();
    root = Join(root, L"PaneBrowser-Private-" + std::to_wstring(GetCurrentProcessId()));
    CreateDirectoryW(root.c_str(), nullptr);
    return root;
}

inline std::string ToUtf8(const std::wstring& value) {
    if (value.empty()) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0, nullptr, nullptr);
    if (n <= 0) return {};
    std::string out(static_cast<size_t>(n), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), out.data(), n, nullptr, nullptr);
    return out;
}

inline std::wstring FromUtf8(const std::string& value) {
    if (value.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (n <= 0) return {};
    std::wstring out(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), out.data(), n);
    return out;
}

inline std::string EncodeField(const std::wstring& value) {
    std::string s = ToUtf8(value);
    std::string out;
    static const char hex[] = "0123456789ABCDEF";
    for (unsigned char c : s) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') out.push_back(static_cast<char>(c));
        else { out.push_back('%'); out.push_back(hex[c >> 4]); out.push_back(hex[c & 15]); }
    }
    return out;
}

inline std::wstring DecodeField(const std::string& value) {
    std::string bytes;
    for (size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '%' && i + 2 < value.size()) {
            auto hex = [](char c) -> int { if (c >= '0' && c <= '9') return c - '0'; if (c >= 'A' && c <= 'F') return c - 'A' + 10; if (c >= 'a' && c <= 'f') return c - 'a' + 10; return -1; };
            int hi = hex(value[i + 1]), lo = hex(value[i + 2]);
            if (hi >= 0 && lo >= 0) { bytes.push_back(static_cast<char>((hi << 4) | lo)); i += 2; continue; }
        }
        bytes.push_back(value[i]);
    }
    return FromUtf8(bytes);
}

inline void WriteLines(const std::wstring& path, const std::vector<std::string>& lines) {
    std::ofstream file(path.c_str(), std::ios::binary | std::ios::trunc);
    for (const auto& line : lines) file << line << '\n';
}

inline std::vector<std::string> ReadLines(const std::wstring& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) lines.push_back(line);
    return lines;
}

inline void SaveHistory(const std::vector<V2HistoryEntry>& entries, const std::wstring& dir) {
    std::vector<std::string> lines;
    for (const auto& e : entries) lines.push_back(EncodeField(e.time) + "\t" + EncodeField(e.uri));
    WriteLines(Join(dir, L"history.pbh"), lines);
}

inline void LoadHistory(std::vector<V2HistoryEntry>& entries, const std::wstring& dir) {
    entries.clear();
    for (const auto& line : ReadLines(Join(dir, L"history.pbh"))) {
        const size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        entries.push_back({DecodeField(line.substr(tab + 1)), DecodeField(line.substr(0, tab))});
    }
}

inline void SaveBookmarks(const std::vector<V2Bookmark>& entries, const std::wstring& dir) {
    std::vector<std::string> lines;
    for (const auto& e : entries) lines.push_back(EncodeField(e.title) + "\t" + EncodeField(e.uri));
    WriteLines(Join(dir, L"bookmarks.pbb"), lines);
}

inline void LoadBookmarks(std::vector<V2Bookmark>& entries, const std::wstring& dir) {
    entries.clear();
    for (const auto& line : ReadLines(Join(dir, L"bookmarks.pbb"))) {
        const size_t tab = line.find('\t');
        if (tab == std::string::npos) continue;
        entries.push_back({DecodeField(line.substr(0, tab)), DecodeField(line.substr(tab + 1))});
    }
}

inline void SaveSettings(const V2Settings& settings, const std::wstring& dir) {
    std::vector<std::string> lines = {
        "theme=" + EncodeField(settings.theme),
        "tabs=" + std::string(settings.tabsEnabled ? "1" : "0"),
        "customTitleBar=" + std::string(settings.customTitleBar ? "1" : "0"),
        "persistHistory=" + std::string(settings.persistHistory ? "1" : "0"),
        "searchEngine=" + EncodeField(settings.searchEngine)
    };
    WriteLines(Join(dir, L"settings.pbs"), lines);
}

inline void LoadSettings(V2Settings& settings, const std::wstring& dir) {
    for (const auto& line : ReadLines(Join(dir, L"settings.pbs"))) {
        const size_t eq = line.find('=');
        if (eq == std::string::npos) continue;
        const std::string key = line.substr(0, eq), value = line.substr(eq + 1);
        if (key == "theme") settings.theme = DecodeField(value);
        else if (key == "tabs") settings.tabsEnabled = value != "0";
        else if (key == "customTitleBar") settings.customTitleBar = value != "0";
        else if (key == "persistHistory") settings.persistHistory = value != "0";
        else if (key == "searchEngine") settings.searchEngine = DecodeField(value);
    }
}

} // namespace v2data
