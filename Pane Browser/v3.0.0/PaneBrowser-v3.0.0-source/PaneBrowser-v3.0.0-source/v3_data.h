#ifndef PANE_BROWSER_V3_DATA_H
#define PANE_BROWSER_V3_DATA_H

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

namespace v3data {

struct HistoryEntry { std::wstring uri; std::wstring time; };
struct Bookmark { std::wstring title; std::wstring uri; };
struct SessionEntry { std::wstring uri; bool privateMode = false; };
struct DownloadRecord { std::wstring fileName; std::wstring path; std::wstring uri; std::wstring state; };
struct Settings {
    std::wstring theme = L"system";
    bool tabsEnabled = true;
    bool customTitleBar = false;
    bool persistHistory = true;
    bool restoreSession = true;
    bool checkUpdates = false;
    std::wstring searchEngine = L"startpage";
    std::wstring customSearchUrl;
    std::wstring downloadDirectory;
};

inline std::wstring ModuleDirectory() {
    wchar_t path[32768] = {};
    DWORD n = GetModuleFileNameW(nullptr, path, static_cast<DWORD>(sizeof(path) / sizeof(path[0])));
    if (!n || n >= sizeof(path) / sizeof(path[0])) return L".";
    std::wstring full(path, n);
    size_t slash = full.find_last_of(L"\\/");
    return slash == std::wstring::npos ? L"." : full.substr(0, slash);
}

inline std::wstring Join(const std::wstring& a, const std::wstring& b) {
    return a + (a.empty() || a.back() == L'\\' ? L"" : L"\\") + b;
}

inline bool EnsureDirectory(const std::wstring& path) {
    if (path.empty()) return false;
    if (CreateDirectoryW(path.c_str(), nullptr)) return true;
    if (GetLastError() != ERROR_ALREADY_EXISTS) return false;
    DWORD attrs = GetFileAttributesW(path.c_str());
    return attrs != INVALID_FILE_ATTRIBUTES && (attrs & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

inline std::wstring DataDirectory() {
    wchar_t local[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, local)) && local[0]) {
        std::wstring root = Join(local, L"Pane Browser");
        if (EnsureDirectory(root)) return root;
    }
    std::wstring fallback = Join(ModuleDirectory(), L"PaneBrowser.Data");
    return EnsureDirectory(fallback) ? fallback : L"";
}

inline std::wstring DefaultDownloadDirectory() {
    PWSTR known = nullptr;
    if (SUCCEEDED(SHGetKnownFolderPath(FOLDERID_Downloads, 0, nullptr, &known)) && known) { std::wstring result(known); CoTaskMemFree(known); return result; }
    wchar_t personal[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_PERSONAL, nullptr, SHGFP_TYPE_CURRENT, personal)) && personal[0]) return Join(personal, L"Downloads");
    return ModuleDirectory();
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
    int n = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (n <= 0) n = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (n <= 0) return {};
    std::wstring out(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), out.data(), n);
    return out;
}

inline std::string Encode(const std::wstring& value) {
    std::string s = ToUtf8(value), out;
    static const char hex[] = "0123456789ABCDEF";
    for (unsigned char c : s) {
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || c == '-' || c == '_' || c == '.' || c == '~') out.push_back(static_cast<char>(c));
        else { out.push_back('%'); out.push_back(hex[c >> 4]); out.push_back(hex[c & 15]); }
    }
    return out;
}

inline std::wstring Decode(const std::string& value) {
    std::string bytes;
    auto hex = [](char c) -> int { if (c >= '0' && c <= '9') return c - '0'; if (c >= 'A' && c <= 'F') return c - 'A' + 10; if (c >= 'a' && c <= 'f') return c - 'a' + 10; return -1; };
    for (size_t i = 0; i < value.size(); ++i) {
        if (value[i] == '%' && i + 2 < value.size()) {
            int hi = hex(value[i + 1]), lo = hex(value[i + 2]);
            if (hi >= 0 && lo >= 0) { bytes.push_back(static_cast<char>((hi << 4) | lo)); i += 2; continue; }
        }
        bytes.push_back(value[i]);
    }
    return FromUtf8(bytes);
}

inline bool WriteLines(const std::wstring& path, const std::vector<std::string>& lines) {
    std::wstring temp = path + L".tmp." + std::to_wstring(GetCurrentProcessId()) + L"." + std::to_wstring(GetTickCount64());
    std::ofstream file(temp.c_str(), std::ios::binary | std::ios::trunc);
    if (!file) return false;
    for (const auto& line : lines) file << line << '\n';
    file.flush();
    if (!file.good()) { file.close(); DeleteFileW(temp.c_str()); return false; }
    file.close();
    BOOL moved = MoveFileExW(temp.c_str(), path.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
    if (!moved) DeleteFileW(temp.c_str());
    return moved != FALSE;
}

inline std::vector<std::string> ReadLines(const std::wstring& path) {
    std::ifstream file(path.c_str(), std::ios::binary);
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(file, line)) lines.push_back(line);
    return lines;
}

inline bool SaveHistory(const std::vector<HistoryEntry>& entries, const std::wstring& dir) {
    std::vector<std::string> lines;
    for (const auto& e : entries) lines.push_back(Encode(e.time) + "\t" + Encode(e.uri));
    return WriteLines(Join(dir, L"history.pbh"), lines);
}
inline void LoadHistory(std::vector<HistoryEntry>& entries, const std::wstring& dir) {
    entries.clear();
    for (const auto& line : ReadLines(Join(dir, L"history.pbh"))) { size_t tab = line.find('\t'); if (tab != std::string::npos) entries.push_back({Decode(line.substr(tab + 1)), Decode(line.substr(0, tab))}); }
}
inline bool SaveBookmarks(const std::vector<Bookmark>& entries, const std::wstring& dir) {
    std::vector<std::string> lines;
    for (const auto& e : entries) lines.push_back(Encode(e.title) + "\t" + Encode(e.uri));
    return WriteLines(Join(dir, L"bookmarks.pbb"), lines);
}
inline void LoadBookmarks(std::vector<Bookmark>& entries, const std::wstring& dir) {
    entries.clear();
    for (const auto& line : ReadLines(Join(dir, L"bookmarks.pbb"))) { size_t tab = line.find('\t'); if (tab != std::string::npos) entries.push_back({Decode(line.substr(0, tab)), Decode(line.substr(tab + 1))}); }
}
inline bool SaveSession(const std::vector<SessionEntry>& entries, int active, const std::wstring& dir) {
    std::vector<std::string> lines; lines.push_back("active=" + std::to_string(active));
    for (const auto& e : entries) if (!e.privateMode && (e.uri.rfind(L"http://", 0) == 0 || e.uri.rfind(L"https://", 0) == 0)) lines.push_back("tab=" + Encode(e.uri));
    return WriteLines(Join(dir, L"session.pbs"), lines);
}
inline void LoadSession(std::vector<SessionEntry>& entries, int& active, const std::wstring& dir) {
    entries.clear(); active = 0;
    for (const auto& line : ReadLines(Join(dir, L"session.pbs"))) {
        if (line.rfind("active=", 0) == 0) { try { active = std::stoi(line.substr(7)); } catch (...) { active = 0; } }
        else if (line.rfind("tab=", 0) == 0) { std::wstring uri = Decode(line.substr(4)); if (uri.rfind(L"http://", 0) == 0 || uri.rfind(L"https://", 0) == 0) entries.push_back({uri, false}); }
    }
    if (entries.empty()) active = 0; else if (active < 0 || active >= static_cast<int>(entries.size())) active = static_cast<int>(entries.size()) - 1;
}
inline bool SaveDownloads(const std::vector<DownloadRecord>& entries, const std::wstring& dir) {
    std::vector<std::string> lines;
    for (const auto& e : entries) lines.push_back(Encode(e.fileName) + "\t" + Encode(e.path) + "\t" + Encode(e.uri) + "\t" + Encode(e.state));
    return WriteLines(Join(dir, L"downloads.pbd"), lines);
}
inline void LoadDownloads(std::vector<DownloadRecord>& entries, const std::wstring& dir) {
    entries.clear();
    for (const auto& line : ReadLines(Join(dir, L"downloads.pbd"))) {
        std::vector<std::string> fields; size_t start = 0;
        while (start <= line.size()) { size_t end = line.find('\t', start); fields.push_back(line.substr(start, end == std::string::npos ? std::string::npos : end - start)); if (end == std::string::npos) break; start = end + 1; }
        if (fields.size() >= 4) entries.push_back({Decode(fields[0]), Decode(fields[1]), Decode(fields[2]), Decode(fields[3])});
    }
}
inline bool SaveSettings(const Settings& s, const std::wstring& dir) {
    return WriteLines(Join(dir, L"settings.pbs"), {"theme=" + Encode(s.theme), "tabs=" + std::string(s.tabsEnabled ? "1" : "0"), "customTitleBar=" + std::string(s.customTitleBar ? "1" : "0"), "persistHistory=" + std::string(s.persistHistory ? "1" : "0"), "restoreSession=" + std::string(s.restoreSession ? "1" : "0"), "checkUpdates=" + std::string(s.checkUpdates ? "1" : "0"), "searchEngine=" + Encode(s.searchEngine), "customSearchUrl=" + Encode(s.customSearchUrl), "downloadDirectory=" + Encode(s.downloadDirectory)});
}
inline void LoadSettings(Settings& s, const std::wstring& dir) {
    for (const auto& line : ReadLines(Join(dir, L"settings.pbs"))) {
        size_t eq = line.find('='); if (eq == std::string::npos) continue; std::string k = line.substr(0, eq), v = line.substr(eq + 1);
        if (k == "theme") s.theme = Decode(v); else if (k == "tabs") s.tabsEnabled = v != "0"; else if (k == "customTitleBar") s.customTitleBar = v != "0"; else if (k == "persistHistory") s.persistHistory = v != "0"; else if (k == "restoreSession") s.restoreSession = v != "0"; else if (k == "checkUpdates") s.checkUpdates = v != "0"; else if (k == "searchEngine") s.searchEngine = Decode(v); else if (k == "customSearchUrl") s.customSearchUrl = Decode(v); else if (k == "downloadDirectory") s.downloadDirectory = Decode(v);
    }
    if (s.downloadDirectory.empty()) s.downloadDirectory = DefaultDownloadDirectory();
}
inline void ClearHistory(const std::wstring& dir) { DeleteFileW(Join(dir, L"history.pbh").c_str()); }
inline void ClearSession(const std::wstring& dir) { DeleteFileW(Join(dir, L"session.pbs").c_str()); }

} // namespace v3data
#endif
