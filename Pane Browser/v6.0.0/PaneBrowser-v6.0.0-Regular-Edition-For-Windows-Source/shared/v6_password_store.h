#pragma once

#include <string>
#include <vector>
#include <algorithm>
#include <cctype>

#ifdef _WIN32
#include <windows.h>
#include <wincred.h>
#else
#include <libsecret/secret.h>
#endif

namespace pane_v6 {

struct PasswordRecord {
    std::string origin;
    std::string password_name;
    std::string username;
    std::string password;
};

inline std::string NormalizeOrigin(std::string value) {
    auto scheme = value.find("://");
    if (scheme != std::string::npos) {
        auto slash = value.find('/', scheme + 3);
        if (slash != std::string::npos) value.resize(slash);
    }
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return value;
}

inline std::string DefaultPasswordName(const std::string& origin) {
    auto scheme = origin.find("://");
    auto start = scheme == std::string::npos ? 0 : scheme + 3;
    auto slash = origin.find('/', start);
    std::string host = origin.substr(start, slash == std::string::npos ? std::string::npos : slash - start);
    return host.empty() ? "Saved password" : host;
}

#ifdef _WIN32
inline std::wstring ToWide(const std::string& value) {
    if (value.empty()) return {};
    int n = MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), nullptr, 0);
    if (n <= 0) return {};
    std::wstring out(static_cast<size_t>(n), L'\0');
    MultiByteToWideChar(CP_UTF8, 0, value.data(), static_cast<int>(value.size()), out.data(), n);
    return out;
}

inline std::string ToUtf8(const wchar_t* value) {
    if (!value) return {};
    int n = WideCharToMultiByte(CP_UTF8, 0, value, -1, nullptr, 0, nullptr, nullptr);
    if (n <= 1) return {};
    std::string out(static_cast<size_t>(n - 1), '\0');
    WideCharToMultiByte(CP_UTF8, 0, value, -1, out.data(), n - 1, nullptr, nullptr);
    return out;
}

inline std::wstring EncodeTargetPart(const std::wstring& value) {
    static const wchar_t digits[] = L"0123456789ABCDEF";
    std::wstring out;
    out.reserve(value.size() * 4);
    for (wchar_t c : value) {
        unsigned int code = static_cast<unsigned int>(c);
        for (int shift = 12; shift >= 0; shift -= 4) out.push_back(digits[(code >> shift) & 0xF]);
    }
    return out;
}

inline bool DecodeTargetPart(const std::wstring& value, std::wstring& out) {
    if (value.empty() || value.size() % 4 != 0) return false;
    out.clear(); out.reserve(value.size() / 4);
    for (size_t i = 0; i < value.size(); i += 4) {
        unsigned int code = 0;
        for (size_t j = 0; j < 4; ++j) {
            wchar_t c = value[i + j]; code <<= 4;
            if (c >= L'0' && c <= L'9') code += static_cast<unsigned int>(c - L'0');
            else if (c >= L'A' && c <= L'F') code += static_cast<unsigned int>(c - L'A' + 10);
            else if (c >= L'a' && c <= L'f') code += static_cast<unsigned int>(c - L'a' + 10);
            else return false;
        }
        out.push_back(static_cast<wchar_t>(code));
    }
    return true;
}

inline std::wstring CredentialTarget(const std::string& origin, const std::string& username) {
    return L"PaneBrowser/v6/o" + EncodeTargetPart(ToWide(NormalizeOrigin(origin))) + L"/u" + EncodeTargetPart(ToWide(username));
}

class PasswordStore {
public:
    bool Save(const std::string& origin, const std::string& passwordName, const std::string& username, const std::string& password) const {
        if (origin.empty() || username.empty() || password.empty()) return false;
        std::wstring target = CredentialTarget(origin, username);
        std::wstring legacyTarget = L"PaneBrowser/v6/" + ToWide(NormalizeOrigin(origin)) + L"/" + ToWide(username);
        CredDeleteW(legacyTarget.c_str(), CRED_TYPE_GENERIC, 0);
        std::wstring secret = ToWide(password), user = ToWide(username), comment = ToWide(passwordName.empty() ? DefaultPasswordName(origin) : passwordName);
        CREDENTIALW credential{};
        credential.Type = CRED_TYPE_GENERIC;
        credential.TargetName = const_cast<LPWSTR>(target.c_str());
        credential.Persist = CRED_PERSIST_LOCAL_MACHINE;
        credential.CredentialBlobSize = static_cast<DWORD>(secret.size() * sizeof(wchar_t));
        credential.CredentialBlob = reinterpret_cast<LPBYTE>(const_cast<wchar_t*>(secret.data()));
        credential.UserName = const_cast<LPWSTR>(user.c_str());
        credential.Comment = const_cast<LPWSTR>(comment.c_str());
        return CredWriteW(&credential, 0) == TRUE;
    }

    std::vector<PasswordRecord> List() const {
        std::vector<PasswordRecord> records;
        PCREDENTIALW* credentials = nullptr; DWORD count = 0;
        if (!CredEnumerateW(L"PaneBrowser/v6/*", 0, &count, &credentials)) return records;
        for (DWORD i = 0; i < count; ++i) {
            if (!credentials[i] || !credentials[i]->TargetName) continue;
            std::wstring target(credentials[i]->TargetName), prefix = L"PaneBrowser/v6/";
            if (target.rfind(prefix, 0) != 0) continue;
            size_t slash = target.rfind(L'/');
            if (slash == std::wstring::npos || slash <= prefix.size() || slash + 1 >= target.size()) continue;
            std::wstring originWide, usernameWide;
            bool encoded = target.size() > prefix.size() + 2 && target[prefix.size()] == L'o' && target[slash + 1] == L'u' && DecodeTargetPart(target.substr(prefix.size() + 1, slash - prefix.size() - 1), originWide) && DecodeTargetPart(target.substr(slash + 2), usernameWide);
            if (!encoded) {
                originWide = target.substr(prefix.size(), slash - prefix.size());
                usernameWide = target.substr(slash + 1);
            }
            std::string password;
            if (credentials[i]->CredentialBlob && credentials[i]->CredentialBlobSize >= sizeof(wchar_t) && credentials[i]->CredentialBlobSize % sizeof(wchar_t) == 0) {
                std::wstring secret(reinterpret_cast<const wchar_t*>(credentials[i]->CredentialBlob), credentials[i]->CredentialBlobSize / sizeof(wchar_t));
                password = ToUtf8(secret.c_str());
            }
            std::string origin = ToUtf8(originWide.c_str());
            std::string username = ToUtf8(usernameWide.c_str());
            std::string name = credentials[i]->Comment ? ToUtf8(credentials[i]->Comment) : DefaultPasswordName(origin);
            records.push_back({origin, name.empty() ? DefaultPasswordName(origin) : name, username, password});
        }
        CredFree(credentials);
        return records;
    }

    bool Remove(const std::string& origin, const std::string&, const std::string& username) const {
        if (origin.empty() || username.empty()) return false;
        std::wstring target = CredentialTarget(origin, username);
        if (CredDeleteW(target.c_str(), CRED_TYPE_GENERIC, 0) == TRUE) return true;
        std::wstring legacyTarget = L"PaneBrowser/v6/" + ToWide(NormalizeOrigin(origin)) + L"/" + ToWide(username);
        return CredDeleteW(legacyTarget.c_str(), CRED_TYPE_GENERIC, 0) == TRUE;
    }
};
#else
inline const SecretSchema* PasswordSchema() {
    static const SecretSchema schema = [] {
        SecretSchema value{};
        value.name = "org.panebrowser.Password";
        value.flags = SECRET_SCHEMA_NONE;
        value.attributes[0] = {"origin", SECRET_SCHEMA_ATTRIBUTE_STRING};
        value.attributes[1] = {"password_name", SECRET_SCHEMA_ATTRIBUTE_STRING};
        value.attributes[2] = {"username", SECRET_SCHEMA_ATTRIBUTE_STRING};
        return value;
    }();
    return &schema;
}

inline SecretService* TryGetSecretService(GError** error) { return secret_service_get_sync(SECRET_SERVICE_NONE, nullptr, error); }

inline GHashTable* PasswordAttributes(const std::string& origin, const std::string& passwordName, const std::string& username, bool includeName = true) {
    GHashTable* attributes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
    g_hash_table_insert(attributes, g_strdup("origin"), g_strdup(NormalizeOrigin(origin).c_str()));
    if (includeName) g_hash_table_insert(attributes, g_strdup("password_name"), g_strdup((passwordName.empty() ? DefaultPasswordName(origin) : passwordName).c_str()));
    g_hash_table_insert(attributes, g_strdup("username"), g_strdup(username.c_str()));
    return attributes;
}

class PasswordStore {
public:
    bool Save(const std::string& origin, const std::string& passwordName, const std::string& username, const std::string& password) const {
        if (origin.empty() || username.empty() || password.empty()) return false;
        GError* error = nullptr; SecretService* service = TryGetSecretService(&error);
        if (!service) { if (error) g_error_free(error); return false; }
        std::string name = passwordName.empty() ? DefaultPasswordName(origin) : passwordName;
        GHashTable* attributes = PasswordAttributes(origin, name, username);
        SecretValue* value = secret_value_new(password.c_str(), static_cast<gssize>(password.size()), "text/plain");
        std::string label = "Pane Browser: " + name;
        gboolean ok = value && secret_service_store_sync(service, PasswordSchema(), attributes, SECRET_COLLECTION_DEFAULT, label.c_str(), value, nullptr, &error);
        if (value) secret_value_unref(value);
        g_hash_table_unref(attributes);
        g_object_unref(service);
        if (error) g_error_free(error);
        return ok == TRUE;
    }

    std::vector<PasswordRecord> List() const {
        std::vector<PasswordRecord> records; GError* error = nullptr; SecretService* service = TryGetSecretService(&error);
        if (!service) { if (error) g_error_free(error); return records; }
        GHashTable* attributes = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
        GList* items = secret_service_search_sync(service, PasswordSchema(), attributes, static_cast<SecretSearchFlags>(SECRET_SEARCH_ALL | SECRET_SEARCH_UNLOCK), nullptr, &error);
        g_hash_table_unref(attributes); g_object_unref(service);
        if (error) { g_error_free(error); return records; }
        for (GList* node = items; node; node = node->next) {
            SecretItem* item = SECRET_ITEM(node->data); if (!item) continue;
            GHashTable* attrs = secret_item_get_attributes(item);
            const char* origin = attrs ? static_cast<const char*>(g_hash_table_lookup(attrs, "origin")) : nullptr;
            const char* name = attrs ? static_cast<const char*>(g_hash_table_lookup(attrs, "password_name")) : nullptr;
            const char* username = attrs ? static_cast<const char*>(g_hash_table_lookup(attrs, "username")) : nullptr;
            std::string password; GError* secretError = nullptr;
            if (secret_item_load_secret_sync(item, nullptr, &secretError)) {
                SecretValue* secret = secret_item_get_secret(item); gsize length = 0; const char* value = secret ? secret_value_get(secret, &length) : nullptr; if (value && length) password.assign(value, length);
            }
            if (secretError) g_error_free(secretError);
            if (origin && username) { std::string originValue(origin); records.push_back({originValue, name && *name ? name : DefaultPasswordName(originValue), username, password}); }
            if (attrs) g_hash_table_unref(attrs);
        }
        g_list_free_full(items, g_object_unref); return records;
    }

    bool Remove(const std::string& origin, const std::string&, const std::string& username) const {
        if (origin.empty() || username.empty()) return false;
        GError* error = nullptr; SecretService* service = TryGetSecretService(&error);
        if (!service) { if (error) g_error_free(error); return false; }
        GHashTable* attributes = PasswordAttributes(origin, "", username, false);
        gboolean ok = secret_service_clear_sync(service, PasswordSchema(), attributes, nullptr, &error);
        g_hash_table_unref(attributes); g_object_unref(service); if (error) g_error_free(error); return ok == TRUE;
    }
};
#endif

} // namespace pane_v6
