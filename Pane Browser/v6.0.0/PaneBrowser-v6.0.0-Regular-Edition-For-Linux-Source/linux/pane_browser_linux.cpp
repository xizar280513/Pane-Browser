#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <json-glib/json-glib.h>
#include <glib.h>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cctype>
#include <cstdlib>
#include <cstring>
#include "../shared/v6_password_store.h"

#ifndef PB_MINI
#define PB_MINI 0
#endif

namespace {
struct Tab {
    GtkWidget* page = nullptr;
    WebKitWebView* view = nullptr;
    bool internal = true;
    bool source = false;
    std::string last_uri = "about:blank";
};

GtkWidget* window = nullptr;
GtkWidget* notebook = nullptr;
GtkWidget* address = nullptr;
GtkWidget* settings_button = nullptr;
GtkWidget* passwords_button = nullptr;
std::vector<Tab*> tabs;
pane_v6::PasswordStore password_store;

std::string ConfigDir() {
    const char* base = g_get_user_config_dir();
    std::string dir = base ? base : ".";
    dir += "/pane-browser-v6/";
#if PB_MINI
    dir += "mini";
#else
    dir += "regular";
#endif
    g_mkdir_with_parents(dir.c_str(), 0700);
    return dir;
}

std::string HtmlEscape(const std::string& s) {
    std::string out;
    for (char c : s) {
        switch (c) { case '&': out += "&amp;"; break; case '<': out += "&lt;"; break; case '>': out += "&gt;"; break; case '\"': out += "&quot;"; break; case '\'': out += "&#39;"; break; default: out += c; }
    }
    return out;
}

std::string UrlEncode(const std::string& value) {
    std::ostringstream out;
    out.fill('0'); out << std::hex;
    for (unsigned char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') out << c;
        else out << '%' << std::uppercase << std::setw(2) << static_cast<int>(c) << std::nouppercase;
    }
    return out.str();
}

std::string NormalizeInput(const std::string& raw) {
    auto first = raw.find_first_not_of(" \t\r\n"), last = raw.find_last_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    std::string value = raw.substr(first, last - first + 1);
    std::string lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (lower.rfind("http://", 0) == 0 || lower.rfind("https://", 0) == 0 || lower.rfind("view-source:", 0) == 0) return value;
    bool host = value.find('.') != std::string::npos && value.find(' ') == std::string::npos;
    return host ? "https://" + value : "https://www.startpage.com/sp/search?query=" + UrlEncode(value);
}

std::string Origin(const std::string& uri) {
    auto scheme = uri.find("://");
    if (scheme == std::string::npos) return {};
    auto end = uri.find_first_of("/?#", scheme + 3);
    std::string out = uri.substr(0, end == std::string::npos ? uri.size() : end);
    std::transform(out.begin(), out.end(), out.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    return out;
}

Tab* ActiveTab() {
#if PB_MINI
    return tabs.empty() ? nullptr : tabs.front();
#else
    if (!notebook) return nullptr;
    int page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
    return page >= 0 && page < static_cast<int>(tabs.size()) ? tabs[static_cast<size_t>(page)] : nullptr;
#endif
}

void UpdateAddress(Tab* tab) {
    if (tab != ActiveTab() || !address) return;
    gtk_entry_set_text(GTK_ENTRY(address), tab && !tab->internal ? tab->last_uri.c_str() : "");
}

const char* DetectorScript() {
    return R"JS((()=>{if(window.__paneV6Detector)return;window.__paneV6Detector=true;function offer(f){try{const p=f.querySelector('input[type=password]');if(!p||!p.value)return;const u=f.querySelector('input[type=email],input[name*=user i],input[name*=login i],input[type=text]');if(!u||!u.value)return;const o=location.origin;if(!/^https?:$/.test(location.protocol))return;window.webkit.messageHandlers.pane.postMessage(JSON.stringify({type:'password-offer',origin:o,username:u.value,password:p.value}));}catch(e){}}document.addEventListener('submit',e=>offer(e.target),true);})())JS";
}

void InjectDetector(Tab*) {
    // The detector is registered once in ConnectView; WebKitGTK injects it on each document.
}

std::string PasswordManagerHtml() {
    const std::string openEye = R"SVG(<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M2 12s3-7 10-7 10 7 10 7-3 7-10 7-10-7-10-7z"/><circle cx="12" cy="12" r="3"/></svg>)SVG";
    const std::string closedEye = R"SVG(<svg xmlns="http://www.w3.org/2000/svg" width="18" height="18" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2"><path d="M17.94 17.94A10.07 10.07 0 0 1 12 20c-7 0-11-8-11-8a18.45 18.45 0 0 1 5.06-5.94M9.9 4.24A9.12 9.12 0 0 1 12 4c7 0 11 8 11 8a18.5 18.5 0 0 1-2.16 3.19m-6.72-1.07a3 3 0 1 1-4.24-4.24"/><line x1="1" y1="1" x2="23" y2="23"/></svg>)SVG";
    std::string html = R"HTML(<!doctype html><html><head><meta charset='utf-8'><style>html,body{min-height:100%}body{margin:0;background:#202124;color:#f1f3f4;font:16px system-ui,sans-serif}.container{max-width:1250px;margin:auto;padding:34px 28px}.card{background:#292a2d;border-radius:16px;padding:28px 26px 36px;box-shadow:0 0 36px rgba(0,0,0,.35)}h1{margin:0 0 22px;font-size:34px;font-weight:600}.search{width:100%;box-sizing:border-box;background:#303134;color:#f1f3f4;border:1px solid #5f6368;border-radius:26px;padding:13px 20px;font-size:17px;outline:none}.form{display:grid;grid-template-columns:1.05fr 1.05fr 1fr 1fr auto;gap:10px;margin:20px 0 10px}.pwfield{display:flex;align-items:center;border:1px solid #5f6368;border-radius:8px;background:#202124;overflow:hidden}.pwfield input{border:0!important;flex:1;min-width:0;outline:none!important;box-shadow:none!important}.eye{width:40px;height:40px;border:0;background:transparent;color:#f1f3f4;cursor:pointer;font-size:0;display:inline-flex;align-items:center;justify-content:center}.eye svg{width:18px;height:18px;display:block;pointer-events:none}input[type=password]::-ms-reveal,input[type=password]::-ms-clear{display:none}input[type=password]::-webkit-credentials-auto-fill-button{display:none!important;visibility:hidden;pointer-events:none}.form input{box-sizing:border-box;width:100%;padding:11px 12px;border-radius:8px;border:1px solid #5f6368;background:#202124;color:#f1f3f4;font:15px system-ui,sans-serif;outline:none}.button{border:0;border-radius:8px;padding:10px 13px;background:#3c4043;color:#f1f3f4;cursor:pointer;font:14px system-ui,sans-serif}.save{background:#8ab4f8;color:#202124}.hint,.muted{color:#bdc1c6;font-size:13px}.selection{display:flex;align-items:center;gap:12px;padding:14px 2px 8px;color:#f1f3f4}.selection .bulk-action{display:none}.selection.has-selection .bulk-action{display:inline-block}.selection button{background:transparent;color:#8ab4f8;padding:8px 10px;border:0;border-radius:6px;cursor:pointer}.selection button:hover{background:#3c4043}.row{display:grid;grid-template-columns:28px 34px minmax(130px,1fr) minmax(170px,1.15fr) minmax(150px,1fr) minmax(150px,1fr) 42px 72px 72px;align-items:center;gap:10px;min-height:74px;border-bottom:1px solid #3c4043}.pick{width:18px;height:18px;accent-color:#8ab4f8}.icon{width:28px;height:28px;border-radius:50%;display:flex;align-items:center;justify-content:center;background:#5f6368;color:#fff;font-weight:700}.value{white-space:nowrap;overflow:hidden;text-overflow:ellipsis}.secretwrap{min-width:0}.secret{width:100%;box-sizing:border-box;background:transparent;color:#f1f3f4;border:0;font:16px system-ui,sans-serif;min-width:0;outline:none!important;box-shadow:none!important}.secret:focus{outline:none!important;box-shadow:none!important}.empty{color:#bdc1c6;padding:22px 0;font-size:16px}.cancel{display:none}@media(max-width:1000px){.form{grid-template-columns:1fr 1fr}.row{grid-template-columns:28px 30px 1fr 1fr 1fr;gap:8px;padding:12px 0}.row .secretwrap{grid-column:3/5}.row .edit,.row .delete{grid-column:auto}}</style></head><body><main class='container'><section class='card'><h1>Saved Passwords</h1><input id='filter' class='search' aria-label='Search saved passwords' placeholder='Search saved passwords' oninput='filterRows()'><form class='form' onsubmit='saveManual();return false'><input id='origin' required placeholder='https://example.com'><input id='name' required placeholder='Password name'><input id='username' required placeholder='Username'><div class='pwfield'><input id='password' required type='password' placeholder='Password'><button class='eye' type='button' aria-label='Show password' onclick='togglePassword(this,document.getElementById("password"))'>)HTML";
    html += closedEye;
    html += R"HTML(</button></div><button id='saveButton' class='button save' type='submit'>Save password</button><button id='cancelButton' class='button cancel' type='button' onclick='cancelEdit()'>Cancel</button></form><p class='hint'>Add a password manually or save one after a detected login. Passwords are stored in the desktop Secret Service.</p><div id='selection' class='selection'><span id='count'>0 selected</span><button type='button' onclick='selectAll()'>Select all</button><button class='bulk-action' type='button' onclick='deselectAll()'>Deselect all</button><button class='bulk-action' type='button' onclick='deleteSelected()'>Delete selected</button></div><div id='rows'>)HTML";
    auto records = password_store.List();
    if (records.empty()) html += "<div class='empty'>No saved passwords yet, or no desktop Secret Service is available.</div>";
    for (size_t i = 0; i < records.size(); ++i) {
        const auto& r = records[i];
        std::string password = r.password.empty() ? "(password unavailable)" : r.password;
        html += "<div class='row' data-row data-index='" + std::to_string(i) + "' data-origin='" + HtmlEscape(r.origin) + "' data-name='" + HtmlEscape(r.password_name) + "' data-username='" + HtmlEscape(r.username) + "' data-password='" + HtmlEscape(password) + "' data-search='" + HtmlEscape(r.password_name + " " + r.origin + " " + r.username) + "'><input type='checkbox' class='pick' onchange='syncSelection()'><div class='icon'>P</div><div class='value' title='" + HtmlEscape(r.password_name) + "'><strong>" + HtmlEscape(r.password_name) + "</strong><div class='muted'>Password name</div></div><div class='value' title='" + HtmlEscape(r.origin) + "'><strong>" + HtmlEscape(r.origin) + "</strong><div class='muted'>Website</div></div><div class='value' title='" + HtmlEscape(r.username) + "'><strong>" + HtmlEscape(r.username) + "</strong><div class='muted'>Username</div></div><div class='secretwrap'><input class='secret' type='password' readonly value='" + HtmlEscape(password) + "'><div class='muted'>Password</div></div><button class='button eye' aria-label='Show password' type='button' onclick='toggleSavedPassword(this)'>" + closedEye + "</button><button class='button edit' type='button' onclick='editRow(this)'>Edit</button><button class='button delete' type='button' onclick=\"send('password-delete:" + std::to_string(i) + "')\">Delete</button></div>";
    }
    html += R"HTML(</div></section></main><script>const send=m=>window.webkit.messageHandlers.pane.postMessage(m);const OPEN_EYE_SVG=')HTML" + openEye + R"HTML(';const CLOSED_EYE_SVG=')HTML" + closedEye + R"HTML(';let editingIndex=-1;function setEye(b,visible){b.innerHTML=visible?OPEN_EYE_SVG:CLOSED_EYE_SVG;b.setAttribute('aria-label',visible?'Hide password':'Show password')}function togglePassword(b,p){const visible=p.type==='password';p.type=visible?'text':'password';setEye(b,visible)}function toggleSavedPassword(b){togglePassword(b,b.parentElement.querySelector('.secret'))}function rows(){return [...document.querySelectorAll('[data-row]')]}function visibleRows(){return rows().filter(r=>r.style.display!=='none')}function syncSelection(){const picks=document.querySelectorAll('.pick:checked');document.getElementById('count').textContent=picks.length+' selected';document.getElementById('selection').classList.toggle('has-selection',picks.length>0)}function selectAll(){visibleRows().forEach(r=>r.querySelector('.pick').checked=true);syncSelection()}function deselectAll(){visibleRows().forEach(r=>r.querySelector('.pick').checked=false);syncSelection()}function deleteSelected(){const ids=rows().filter(r=>r.querySelector('.pick').checked).map(r=>r.dataset.index);if(ids.length)send('password-delete-selected:'+ids.join(','))}function filterRows(){const q=document.getElementById('filter').value.toLowerCase();rows().forEach(r=>{const visible=r.dataset.search.toLowerCase().includes(q);r.style.display=visible?'grid':'none';if(!visible)r.querySelector('.pick').checked=false});syncSelection()}function editRow(b){const r=b.closest('[data-row]');editingIndex=Number(r.dataset.index);document.getElementById('origin').value=r.dataset.origin;document.getElementById('name').value=r.dataset.name;document.getElementById('username').value=r.dataset.username;const p=document.getElementById('password');p.value=r.dataset.password;p.type='password';document.getElementById('saveButton').textContent='Update password';document.getElementById('cancelButton').style.display='inline-block';document.getElementById('origin').focus();window.scrollTo(0,0)}function cancelEdit(){editingIndex=-1;document.querySelector('.form').reset();document.getElementById('password').type='password';document.getElementById('saveButton').textContent='Save password';document.getElementById('cancelButton').style.display='none'}function saveManual(){const o=document.getElementById('origin').value.trim(),n=document.getElementById('name').value.trim(),u=document.getElementById('username').value,p=document.getElementById('password').value;if(!o||!n||!u||!p)return;const payload={type:editingIndex>=0?'password-update':'password-save',origin:o,passwordName:n,username:u,password:p};if(editingIndex>=0){const r=rows().find(x=>Number(x.dataset.index)===editingIndex);if(r){payload.oldOrigin=r.dataset.origin;payload.oldUsername=r.dataset.username}}send(JSON.stringify(payload))}</script></body></html>)HTML";
    return html;
}

void ShowPasswordManager() {
    Tab* tab = ActiveTab(); if (!tab || !tab->view) return;
    tab->internal = true; tab->source = false; tab->last_uri = "about:passwords"; UpdateAddress(tab);
    webkit_web_view_load_html(tab->view, PasswordManagerHtml().c_str(), "about:blank");
}

void HandlePasswordOffer(const std::string& message, Tab* tab) {
    JsonParser* parser = json_parser_new(); GError* error = nullptr;
    if (!json_parser_load_from_data(parser, message.c_str(), -1, &error)) { if (error) g_error_free(error); g_object_unref(parser); return; }
    JsonObject* object = json_node_get_object(json_parser_get_root(parser));
    const char* type = json_object_get_string_member_with_default(object, "type", "");
    const char* origin = json_object_get_string_member_with_default(object, "origin", "");
    const char* username = json_object_get_string_member_with_default(object, "username", "");
    const char* password = json_object_get_string_member_with_default(object, "password", "");
    if (std::string(type) == "password-offer" && tab && !tab->internal && origin == Origin(tab->last_uri) && std::string(origin).rfind("https://", 0) == 0 && *username && *password) {
        std::string prompt = "Pane Browser detected a login password for:\n" + std::string(origin) + "\n\nSave this password to Password Manager?";
        GtkWidget* dialog = gtk_dialog_new_with_buttons("Save password", GTK_WINDOW(window), GTK_DIALOG_MODAL, "Save", GTK_RESPONSE_YES, "Do not save", GTK_RESPONSE_NO, nullptr);
        GtkWidget* area = gtk_dialog_get_content_area(GTK_DIALOG(dialog));
        GtkWidget* label = gtk_label_new(prompt.c_str());
        gtk_label_set_line_wrap(GTK_LABEL(label), TRUE);
        gtk_container_add(GTK_CONTAINER(area), label);
        gtk_widget_show_all(area);
        gint response = gtk_dialog_run(GTK_DIALOG(dialog));
        gtk_widget_destroy(dialog);
        if (response == GTK_RESPONSE_YES && !password_store.Save(origin, pane_v6::DefaultPasswordName(origin), username, password)) {
            GtkWidget* failed = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "The desktop Secret Service could not save this password.");
            gtk_window_set_title(GTK_WINDOW(failed), "Password Manager"); gtk_dialog_run(GTK_DIALOG(failed)); gtk_widget_destroy(failed);
        }
    }
    g_object_unref(parser);
}

std::string SettingsHtml() {
    std::string html = "<!doctype html><html><head><meta charset='utf-8'><style>body{font:16px system-ui;margin:0;background:#f8f9fa;color:#202124}.card{max-width:820px;margin:30px auto;background:#fff;border:1px solid #dadce0;border-radius:16px;padding:28px;box-shadow:0 4px 20px #0002}.row{display:flex;justify-content:space-between;align-items:center;padding:15px 0;border-bottom:1px solid #e8eaed}select,button{font:inherit;padding:8px 12px;border-radius:6px;border:1px solid #c7cbd1;background:#fff}button{cursor:pointer}</style></head><body><main class='card'><h1>Settings</h1><div class='row'><span>Password Manager</span><button onclick=\"send('passwords')\">View saved passwords</button></div></main><script>function send(m){window.webkit.messageHandlers.pane.postMessage(m)}</script></body></html>";
    return html;
}

void ShowSettings() {
    Tab* tab = ActiveTab(); if (!tab || !tab->view) return;
    tab->internal = true; tab->source = false; tab->last_uri = "about:settings"; UpdateAddress(tab);
    webkit_web_view_load_html(tab->view, SettingsHtml().c_str(), "about:blank");
}

void Navigate(Tab* tab, const std::string& raw) {
    if (!tab || !tab->view) return;
    std::string uri = NormalizeInput(raw); if (uri.empty()) return;
    tab->internal = false; tab->source = uri.rfind("view-source:", 0) == 0; tab->last_uri = uri; UpdateAddress(tab);
    webkit_web_view_load_uri(tab->view, uri.c_str());
}

void ShowHome(Tab* tab) {
    if (!tab || !tab->view) return;
    tab->internal = true; tab->source = false; tab->last_uri = "about:blank"; UpdateAddress(tab);
    const char* html = "<!doctype html><html><body style='font:16px system-ui;display:grid;place-items:center;height:100%;margin:0;background:#f8f9fa;color:#202124'><main style='width:min(640px,88vw);text-align:center'><h1>Pane Browser</h1><form onsubmit=\"go();return false\" style='display:flex;height:54px;border:1px solid #c7cbd1;background:#fff;border-radius:6px;overflow:hidden'><input id=q autofocus style='flex:1;border:0;padding:0 16px;font:inherit;outline:0'><button style='width:58px;border:0;font-size:20px'>⌕</button></form><p style='font-size:12px;color:#5f6368'>Linux Edition · WebKitGTK · password manager available in Settings</p></main><script>function go(){const q=document.getElementById('q').value.trim();if(q)window.webkit.messageHandlers.pane.postMessage('search:'+q)}</script></body></html>";
    webkit_web_view_load_html(tab->view, html, "about:blank");
}

#if !PB_MINI
Tab* NewTab(const std::string& uri);
#endif

static void OnMessage(WebKitUserContentManager*, WebKitJavascriptResult* result, gpointer data) {
    Tab* tab = static_cast<Tab*>(data); if (!tab) tab = ActiveTab(); if (!tab) return;
    JSCValue* value = webkit_javascript_result_get_js_value(result);
    if (!jsc_value_is_string(value)) return;
    char* raw = jsc_value_to_string(value); if (!raw) return;
    std::string message(raw); g_free(raw);
    if (message.rfind("search:", 0) == 0) { Navigate(tab, message.substr(7)); return; }
    if (message == "passwords") { ShowPasswordManager(); return; }
    if (!message.empty() && message.front() == '{') {
        JsonParser* parser = json_parser_new(); GError* error = nullptr;
        if (!json_parser_load_from_data(parser, message.c_str(), static_cast<gssize>(message.size()), &error)) { if (error) g_error_free(error); g_object_unref(parser); return; }
        JsonNode* root = json_parser_get_root(parser); JsonObject* object = root && JSON_NODE_HOLDS_OBJECT(root) ? json_node_get_object(root) : nullptr;
        const char* type = object ? json_object_get_string_member_with_default(object, "type", "") : "";
        if ((std::string(type) == "password-save" || std::string(type) == "password-update") && tab && tab->internal && object) {
            const char* origin = json_object_get_string_member_with_default(object, "origin", "");
            const char* name = json_object_get_string_member_with_default(object, "passwordName", "");
            const char* username = json_object_get_string_member_with_default(object, "username", "");
            const char* password = json_object_get_string_member_with_default(object, "password", "");
            const char* oldOrigin = json_object_get_string_member_with_default(object, "oldOrigin", "");
            const char* oldUsername = json_object_get_string_member_with_default(object, "oldUsername", "");
            std::string canonicalOrigin = Origin(origin);
            bool valid = message.size() <= 16384 && std::strlen(origin) <= 2048 && std::strlen(name) <= 512 && std::strlen(username) <= 512 && std::strlen(password) <= 4096 && !canonicalOrigin.empty() && canonicalOrigin.rfind("https://", 0) == 0 && *name && *username && *password && std::string(origin).find_first_of("\r\n\t") == std::string::npos && std::string(name).find_first_of("\r\n\t") == std::string::npos && std::string(username).find_first_of("\r\n\t") == std::string::npos && std::string(password).find_first_of("\r\n") == std::string::npos;
            if (std::string(type) == "password-update") valid = valid && *oldOrigin && *oldUsername;
            if (valid && password_store.Save(canonicalOrigin, name, username, password)) {
                if (std::string(type) == "password-update" && (Origin(oldOrigin) != canonicalOrigin || std::string(oldUsername) != username)) password_store.Remove(Origin(oldOrigin), "", oldUsername);
                g_object_unref(parser); ShowPasswordManager(); return;
            }
            GtkWidget* dialog = gtk_message_dialog_new(GTK_WINDOW(window), GTK_DIALOG_MODAL, GTK_MESSAGE_WARNING, GTK_BUTTONS_OK, "The password could not be saved. Use an HTTPS origin, password name, username, and password."); gtk_window_set_title(GTK_WINDOW(dialog), "Password Manager"); gtk_dialog_run(GTK_DIALOG(dialog)); gtk_widget_destroy(dialog);
        } else if (std::string(type) == "password-offer") {
            HandlePasswordOffer(message, tab);
        }
        g_object_unref(parser); return;
    }
    if (message.rfind("password-delete-selected:", 0) == 0) {
        std::vector<int> indices; size_t start = 25;
        while (start < message.size()) { size_t end = message.find(',', start); try { indices.push_back(std::stoi(message.substr(start, end == std::string::npos ? std::string::npos : end - start))); } catch (...) {} if (end == std::string::npos) break; start = end + 1; }
        auto records = password_store.List(); std::sort(indices.begin(), indices.end(), std::greater<int>());
        for (int index : indices) if (index >= 0 && index < static_cast<int>(records.size())) password_store.Remove(records[static_cast<size_t>(index)].origin, records[static_cast<size_t>(index)].password_name, records[static_cast<size_t>(index)].username);
        ShowPasswordManager(); return;
    }
    if (message.rfind("password-delete:", 0) == 0) {
        char* end = nullptr; long index = std::strtol(message.c_str() + 16, &end, 10); auto records = password_store.List(); if (end && *end == '\0' && index >= 0 && index < static_cast<long>(records.size())) password_store.Remove(records[static_cast<size_t>(index)].origin, records[static_cast<size_t>(index)].password_name, records[static_cast<size_t>(index)].username); ShowPasswordManager(); return;
    }
    if (message.rfind("password-offer", 0) == 0 || (!message.empty() && message.front() == '{')) { HandlePasswordOffer(message, tab); return; }
}

static void OnLoadChanged(WebKitWebView* view, WebKitLoadEvent event, gpointer data) {
    Tab* tab = static_cast<Tab*>(data); if (!tab || tab->view != view) return;
    if (event == WEBKIT_LOAD_COMMITTED || event == WEBKIT_LOAD_FINISHED) {
        const char* uri = webkit_web_view_get_uri(view);
        if (uri && *uri && tab->last_uri.rfind("about:", 0) != 0) { tab->last_uri = uri; tab->internal = false; UpdateAddress(tab); }
        if (event == WEBKIT_LOAD_FINISHED) InjectDetector(tab);
    }
}

static gboolean OnDecidePolicy(WebKitWebView* view, WebKitPolicyDecision* decision, WebKitPolicyDecisionType type, gpointer data) {
    (void)view;
    if (type != WEBKIT_POLICY_DECISION_TYPE_NAVIGATION_ACTION) return FALSE;
    WebKitNavigationAction* action = webkit_navigation_policy_decision_get_navigation_action(WEBKIT_NAVIGATION_POLICY_DECISION(decision));
    WebKitURIRequest* request = webkit_navigation_action_get_request(action);
    const char* uri = request ? webkit_uri_request_get_uri(request) : nullptr;
    if (uri && *uri) { Tab* tab = static_cast<Tab*>(data); tab->last_uri = uri; tab->internal = false; tab->source = std::string(uri).rfind("view-source:", 0) == 0; UpdateAddress(tab); }
    return FALSE;
}

#if !PB_MINI
static WebKitWebView* OnCreate(WebKitWebView*, WebKitNavigationAction* action, gpointer data) {
    WebKitURIRequest* request = action ? webkit_navigation_action_get_request(action) : nullptr;
    const char* uri = request ? webkit_uri_request_get_uri(request) : nullptr;
    Tab* opener = static_cast<Tab*>(data); if (!opener) opener = ActiveTab();
    gboolean user = action ? webkit_navigation_action_is_user_gesture(action) : FALSE;
    if (uri && opener && user && opener->source) { Tab* tab = NewTab(uri); return tab ? tab->view : nullptr; }
    if (uri && opener && user) { Navigate(opener, uri); return opener->view; }
    return nullptr;
}
#endif

static void ConnectView(Tab* tab) {
    WebKitUserContentManager* manager = webkit_web_view_get_user_content_manager(tab->view);
    WebKitUserScript* detector = webkit_user_script_new(DetectorScript(), WEBKIT_USER_CONTENT_INJECT_TOP_FRAME, WEBKIT_USER_SCRIPT_INJECT_AT_DOCUMENT_END, nullptr, nullptr);
    webkit_user_content_manager_add_script(manager, detector);
    webkit_user_script_unref(detector);
    g_signal_connect(manager, "script-message-received::pane", G_CALLBACK(OnMessage), tab);
    g_signal_connect(tab->view, "load-changed", G_CALLBACK(OnLoadChanged), tab);
    g_signal_connect(tab->view, "decide-policy", G_CALLBACK(OnDecidePolicy), tab);
#if !PB_MINI
    g_signal_connect(tab->view, "create", G_CALLBACK(OnCreate), tab);
#endif
}

#if !PB_MINI
Tab* NewTab(const std::string& uri) {
    Tab* tab = new Tab();
    WebKitUserContentManager* manager = webkit_user_content_manager_new();
    webkit_user_content_manager_register_script_message_handler(manager, "pane");
    tab->view = WEBKIT_WEB_VIEW(webkit_web_view_new_with_user_content_manager(manager));
    g_object_unref(manager);
    tab->page = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_box_pack_start(GTK_BOX(tab->page), GTK_WIDGET(tab->view), TRUE, TRUE, 0);
    gtk_widget_show_all(tab->page);
    gtk_notebook_append_page(GTK_NOTEBOOK(notebook), tab->page, nullptr);
    tabs.push_back(tab);
    ConnectView(tab);
    gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), static_cast<gint>(tabs.size() - 1));
    if (uri == "dashboard" || uri.empty()) ShowHome(tab); else Navigate(tab, uri);
    return tab;
}
#else
Tab* NewTab(const std::string& uri) {
    Tab* tab = new Tab();
    WebKitUserContentManager* manager = webkit_user_content_manager_new();
    webkit_user_content_manager_register_script_message_handler(manager, "pane");
    tab->view = WEBKIT_WEB_VIEW(webkit_web_view_new_with_user_content_manager(manager));
    g_object_unref(manager);
    tab->page = GTK_WIDGET(tab->view);
    gtk_box_pack_start(GTK_BOX(gtk_bin_get_child(GTK_BIN(window))), tab->page, TRUE, TRUE, 0);
    tabs.push_back(tab); ConnectView(tab); gtk_widget_show_all(tab->page);
    if (uri == "dashboard" || uri.empty()) ShowHome(tab); else Navigate(tab, uri);
    return tab;
}
#endif

static void OnBack(GtkButton*, gpointer) { Tab* t = ActiveTab(); if (t) webkit_web_view_go_back(t->view); }
static void OnForward(GtkButton*, gpointer) { Tab* t = ActiveTab(); if (t) webkit_web_view_go_forward(t->view); }
static void OnRefresh(GtkButton*, gpointer) { Tab* t = ActiveTab(); if (t) webkit_web_view_reload(t->view); }
static void OnHome(GtkButton*, gpointer) { ShowHome(ActiveTab()); }
static void OnSettings(GtkButton*, gpointer) { ShowSettings(); }
static void OnPasswords(GtkButton*, gpointer) { ShowPasswordManager(); }
static void OnAddressActivate(GtkEntry*, gpointer) { Tab* t = ActiveTab(); if (t) Navigate(t, gtk_entry_get_text(GTK_ENTRY(address))); }
#if !PB_MINI
static void OnNewTab(GtkButton*, gpointer) { NewTab("dashboard"); }
#endif

static gboolean OnKey(GtkWidget*, GdkEventKey* event, gpointer) {
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_l) { gtk_widget_grab_focus(address); gtk_editable_select_region(GTK_EDITABLE(address), 0, -1); return TRUE; }
#if !PB_MINI
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_t) { NewTab("dashboard"); return TRUE; }
#endif
    if (event->keyval == GDK_KEY_F11) { if (gtk_window_get_window_type(GTK_WINDOW(window)) == GTK_WINDOW_TOPLEVEL) gtk_window_fullscreen(GTK_WINDOW(window)); return TRUE; }
    return FALSE;
}

static void BuildUi() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), PB_MINI ? "Pane Browser Mini Edition — Linux" : "Pane Browser Regular Edition — Linux");
    gtk_window_set_default_size(GTK_WINDOW(window), 1200, 800);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr);
    g_signal_connect(window, "key-press-event", G_CALLBACK(OnKey), nullptr);
    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_container_add(GTK_CONTAINER(window), root);
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 5);
    gtk_widget_set_margin_top(toolbar, 5); gtk_widget_set_margin_bottom(toolbar, 5); gtk_widget_set_margin_start(toolbar, 5); gtk_widget_set_margin_end(toolbar, 5);
    gtk_box_pack_start(GTK_BOX(root), toolbar, FALSE, FALSE, 0);
    auto add_button = [&](const char* label, GCallback callback) { GtkWidget* b = gtk_button_new_with_label(label); gtk_box_pack_start(GTK_BOX(toolbar), b, FALSE, FALSE, 0); g_signal_connect(b, "clicked", callback, nullptr); return b; };
    add_button("Back", G_CALLBACK(OnBack)); add_button("Forward", G_CALLBACK(OnForward)); add_button("Refresh", G_CALLBACK(OnRefresh)); add_button("Home", G_CALLBACK(OnHome));
    address = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(address), "Enter URL or search"); gtk_box_pack_start(GTK_BOX(toolbar), address, TRUE, TRUE, 0); g_signal_connect(address, "activate", G_CALLBACK(OnAddressActivate), nullptr);
#if !PB_MINI
    add_button("+", G_CALLBACK(OnNewTab));
#endif
    settings_button = add_button("Settings", G_CALLBACK(OnSettings));
    passwords_button = add_button("Passwords", G_CALLBACK(OnPasswords));
#if !PB_MINI
    notebook = gtk_notebook_new(); gtk_box_pack_start(GTK_BOX(root), notebook, TRUE, TRUE, 0);
#else
    notebook = nullptr;
#endif
    gtk_widget_show_all(window);
}
}

int main(int argc, char** argv) {
    gtk_init(&argc, &argv);
    BuildUi();
#if PB_MINI
    NewTab("dashboard");
#else
    NewTab("dashboard");
#endif
    gtk_main();
    for (Tab* tab : tabs) delete tab;
    return 0;
}
