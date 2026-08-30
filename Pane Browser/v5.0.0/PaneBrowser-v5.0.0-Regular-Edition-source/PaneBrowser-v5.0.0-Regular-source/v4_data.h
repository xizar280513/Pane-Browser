#ifndef PANE_BROWSER_V4_DATA_H
#define PANE_BROWSER_V4_DATA_H

#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <shlobj.h>
#include <algorithm>
#include <cctype>
#include <fstream>
#include <string>
#include <vector>
#include "v3_data.h"

namespace v4data {

struct ProfileInfo {
    std::wstring id;
    std::wstring name;
    std::wstring kind = L"normal"; // normal, guest, private
    bool temporary = false;
};

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

inline std::wstring BaseDirectory() {
    wchar_t local[MAX_PATH] = {};
    if (SUCCEEDED(SHGetFolderPathW(nullptr, CSIDL_LOCAL_APPDATA, nullptr, SHGFP_TYPE_CURRENT, local)) && local[0]) {
        std::wstring root = Join(local, L"Pane Browser");
        if (EnsureDirectory(root)) return root;
    }
    std::wstring fallback = Join(v3data::ModuleDirectory(), L"PaneBrowser.Data");
    return EnsureDirectory(fallback) ? fallback : L"";
}

inline std::wstring SanitizeId(const std::wstring& value) {
    std::wstring out;
    for (wchar_t c : value) {
        if ((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || c == L'-' || c == L'_') out.push_back(c);
        else if (c == L' ' || c == L'.') out.push_back(L'-');
        if (out.size() >= 48) break;
    }
    if (out.empty()) out = L"profile";
    return out;
}

inline bool IsSafeProfileId(const std::wstring& id) {
    if (id.empty() || id.size() > 48 || id == L"." || id == L"..") return false;
    for (wchar_t c : id) {
        if (!((c >= L'a' && c <= L'z') || (c >= L'A' && c <= L'Z') || (c >= L'0' && c <= L'9') || c == L'-' || c == L'_')) return false;
    }
    return true;
}

inline std::string ToUtf8(const std::wstring& value) { return v3data::ToUtf8(value); }
inline std::wstring FromUtf8(const std::string& value) { return v3data::FromUtf8(value); }
inline std::string Encode(const std::wstring& value) { return v3data::Encode(value); }
inline std::wstring Decode(const std::string& value) { return v3data::Decode(value); }

inline std::wstring ProfileDataDirectory(const ProfileInfo& profile) {
    std::wstring base = BaseDirectory();
    if (_wcsicmp(profile.id.c_str(), L"personal") == 0) return base; // preserves v3 personal data layout
    return Join(Join(base, L"profiles"), profile.id);
}

inline bool DeleteDirectoryTree(const std::wstring& path) {
    if (path.empty()) return false;
    DWORD attrs = GetFileAttributesW(path.c_str());
    if (attrs == INVALID_FILE_ATTRIBUTES) return GetLastError() == ERROR_FILE_NOT_FOUND || GetLastError() == ERROR_PATH_NOT_FOUND;
    if (attrs & FILE_ATTRIBUTE_REPARSE_POINT) {
        SetFileAttributesW(path.c_str(), FILE_ATTRIBUTE_NORMAL);
        return (attrs & FILE_ATTRIBUTE_DIRECTORY) ? (RemoveDirectoryW(path.c_str()) != FALSE) : (DeleteFileW(path.c_str()) != FALSE);
    }
    if ((attrs & FILE_ATTRIBUTE_DIRECTORY) == 0) { SetFileAttributesW(path.c_str(), FILE_ATTRIBUTE_NORMAL); return DeleteFileW(path.c_str()) != FALSE; }
    WIN32_FIND_DATAW data{};
    HANDLE find = FindFirstFileW(Join(path, L"*").c_str(), &data);
    if (find != INVALID_HANDLE_VALUE) {
        do {
            if (wcscmp(data.cFileName, L".") == 0 || wcscmp(data.cFileName, L"..") == 0) continue;
            std::wstring child = Join(path, data.cFileName);
            if (data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) DeleteDirectoryTree(child);
            else { SetFileAttributesW(child.c_str(), FILE_ATTRIBUTE_NORMAL); DeleteFileW(child.c_str()); }
        } while (FindNextFileW(find, &data));
        FindClose(find);
    }
    SetFileAttributesW(path.c_str(), FILE_ATTRIBUTE_NORMAL);
    return RemoveDirectoryW(path.c_str()) != FALSE || GetLastError() == ERROR_PATH_NOT_FOUND || GetLastError() == ERROR_FILE_NOT_FOUND;
}

inline std::wstring WebViewUserDataFolder() {
    return Join(BaseDirectory(), L"WebView2");
}

inline bool EnsureProfileLayout(const ProfileInfo& profile) {
    std::wstring base = BaseDirectory();
    if (base.empty()) return false;
    if (!EnsureDirectory(Join(base, L"profiles"))) return false;
    std::wstring data = ProfileDataDirectory(profile);
    if (!EnsureDirectory(data)) return false;
    if (!EnsureDirectory(Join(data, L"workspaces"))) return false;
    if (!EnsureDirectory(Join(data, L"notes"))) return false;
    if (!EnsureDirectory(Join(data, L"backups"))) return false;
    return EnsureDirectory(WebViewUserDataFolder());
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

inline bool SaveProfiles(const std::vector<ProfileInfo>& profiles) {
    std::wstring path = Join(BaseDirectory(), L"profiles.pbr");
    std::vector<std::string> lines;
    for (const auto& p : profiles) {
        if (!IsSafeProfileId(p.id) || p.kind == L"private" || p.temporary) continue;
        lines.push_back(Encode(p.id) + "\t" + Encode(p.name) + "\t" + Encode(p.kind));
    }
    return WriteLines(path, lines);
}

inline void LoadProfiles(std::vector<ProfileInfo>& profiles) {
    profiles.clear();
    for (const auto& line : ReadLines(Join(BaseDirectory(), L"profiles.pbr"))) {
        size_t a = line.find('\t');
        size_t b = a == std::string::npos ? std::string::npos : line.find('\t', a + 1);
        if (a == std::string::npos || b == std::string::npos) continue;
        ProfileInfo p{Decode(line.substr(0, a)), Decode(line.substr(a + 1, b - a - 1)), Decode(line.substr(b + 1)), false};
        bool duplicate = false;
        for (const auto& existing : profiles) if (_wcsicmp(existing.id.c_str(), p.id.c_str()) == 0) { duplicate = true; break; }
        if (IsSafeProfileId(p.id) && !p.name.empty() && (p.kind == L"normal" || p.kind == L"guest") && !duplicate) profiles.push_back(p);
    }
    bool personal = false;
    for (const auto& p : profiles) if (_wcsicmp(p.id.c_str(), L"personal") == 0) personal = true;
    if (!personal) profiles.insert(profiles.begin(), {L"personal", L"Personal", L"normal", false});
    std::sort(profiles.begin(), profiles.end(), [](const ProfileInfo& a, const ProfileInfo& b) { return a.id < b.id; });
}

inline bool SaveActiveProfile(const std::wstring& id) {
    return WriteLines(Join(BaseDirectory(), L"active-profile.pbr"), {Encode(id)});
}

inline std::wstring LoadActiveProfile() {
    auto lines = ReadLines(Join(BaseDirectory(), L"active-profile.pbr"));
    return lines.empty() ? L"personal" : Decode(lines.front());
}

inline bool IsWithin(const std::wstring& root, const std::wstring& path) {
    std::wstring r = root, p = path;
    std::transform(r.begin(), r.end(), r.begin(), towlower);
    std::transform(p.begin(), p.end(), p.begin(), towlower);
    if (!r.empty() && r.back() != L'\\') r.push_back(L'\\');
    return p.rfind(r, 0) == 0;
}

} // namespace v4data
#endif
