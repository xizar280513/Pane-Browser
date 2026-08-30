#include <gtk/gtk.h>
#include <webkit2/webkit2.h>
#include <algorithm>
#include <cctype>
#include <iomanip>
#include <sstream>
#include <string>

namespace {
GtkWidget* window = nullptr;
GtkWidget* address = nullptr;
WebKitWebView* view = nullptr;
bool fullscreen = false;

std::string UrlEncode(const std::string& value) {
    std::ostringstream out; out.fill('0'); out << std::hex;
    for (unsigned char c : value) {
        if (std::isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') out << c;
        else out << '%' << std::uppercase << std::setw(2) << static_cast<int>(c) << std::nouppercase;
    }
    return out.str();
}

std::string NormalizeInput(const std::string& raw) {
    auto first = raw.find_first_not_of(" \t\r\n"), last = raw.find_last_not_of(" \t\r\n");
    if (first == std::string::npos) return {};
    std::string value = raw.substr(first, last - first + 1), lower = value;
    std::transform(lower.begin(), lower.end(), lower.begin(), [](unsigned char c) { return static_cast<char>(std::tolower(c)); });
    if (lower.rfind("http://", 0) == 0 || lower.rfind("https://", 0) == 0) return value;
    bool host = value.find('.') != std::string::npos && value.find(' ') == std::string::npos;
    return host ? "https://" + value : "https://www.startpage.com/sp/search?query=" + UrlEncode(value);
}

const char* HomeHtml() {
    return "<!doctype html><html><head><meta charset='utf-8'><style>:root{color-scheme:light;--bg:#f4f5f7;--text:#1f2328;--muted:#5f6368;--field:#fff;--border:#c7cbd1}html,body{height:100%;margin:0}body{display:grid;place-items:center;background:var(--bg);color:var(--text);font:16px system-ui,sans-serif}.wrap{width:min(620px,88vw);text-align:center}.brand{font-size:34px;font-weight:600;margin-bottom:22px}.search{height:54px;display:flex;border:1px solid var(--border);border-radius:6px;background:var(--field);overflow:hidden}.search input{flex:1;border:0;outline:0;padding:0 16px;background:transparent;color:var(--text);font:16px system-ui,sans-serif}.search button{width:56px;border:0;background:#e5e7eb;color:var(--text);font-size:22px;cursor:pointer}.note{margin-top:18px;color:var(--muted);font-size:12px}</style></head><body><main class='wrap'><div class='brand'>Pane Browser</div><form class='search' onsubmit='go();return false'><input id='q' autocomplete='off' autofocus><button aria-label='Search'>⌕</button></form><div class='note'>Single tab · no history · no persistent browser data</div></main><script>function go(){const q=document.getElementById('q').value.trim();if(q)window.webkit.messageHandlers.pane.postMessage('search:'+q)}</script></body></html>";
}

void ShowHome() { gtk_entry_set_text(GTK_ENTRY(address), ""); webkit_web_view_load_html(view, HomeHtml(), "about:blank"); }
void Navigate(const std::string& raw) { std::string uri = NormalizeInput(raw); if (!uri.empty()) webkit_web_view_load_uri(view, uri.c_str()); }
void OnAddressActivate(GtkEntry*, gpointer) { Navigate(gtk_entry_get_text(GTK_ENTRY(address))); }
void OnBack(GtkButton*, gpointer) { webkit_web_view_go_back(view); }
void OnForward(GtkButton*, gpointer) { webkit_web_view_go_forward(view); }
void OnRefresh(GtkButton*, gpointer) { webkit_web_view_reload(view); }
void OnHome(GtkButton*, gpointer) { ShowHome(); }
void OnMessage(WebKitUserContentManager*, WebKitJavascriptResult* result, gpointer) {
    JSCValue* value = webkit_javascript_result_get_js_value(result); if (!jsc_value_is_string(value)) return;
    char* raw = jsc_value_to_string(value); if (!raw) return; std::string message(raw); g_free(raw);
    if (message.rfind("search:", 0) == 0) Navigate(message.substr(7));
}
void OnLoadChanged(WebKitWebView* web, WebKitLoadEvent event, gpointer) {
    if (event != WEBKIT_LOAD_COMMITTED && event != WEBKIT_LOAD_FINISHED) return;
    const char* uri = webkit_web_view_get_uri(web);
    if (uri && *uri && std::string(uri) != "about:blank") gtk_entry_set_text(GTK_ENTRY(address), uri);
}
gboolean OnKey(GtkWidget*, GdkEventKey* event, gpointer) {
    if (event->keyval == GDK_KEY_F11) { fullscreen = !fullscreen; if (fullscreen) gtk_window_fullscreen(GTK_WINDOW(window)); else gtk_window_unfullscreen(GTK_WINDOW(window)); return TRUE; }
    if ((event->state & GDK_CONTROL_MASK) && event->keyval == GDK_KEY_L) { gtk_widget_grab_focus(address); gtk_editable_select_region(GTK_EDITABLE(address), 0, -1); return TRUE; }
    return FALSE;
}
void BuildUi() {
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL); gtk_window_set_title(GTK_WINDOW(window), "Pane Browser Mini Edition — Linux"); gtk_window_set_default_size(GTK_WINDOW(window), 1100, 760); g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), nullptr); g_signal_connect(window, "key-press-event", G_CALLBACK(OnKey), nullptr);
    GtkWidget* root = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0); gtk_container_add(GTK_CONTAINER(window), root);
    GtkWidget* toolbar = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 4); gtk_box_pack_start(GTK_BOX(root), toolbar, FALSE, FALSE, 4);
    auto button = [&](const char* label, GCallback callback) { GtkWidget* b = gtk_button_new_with_label(label); gtk_box_pack_start(GTK_BOX(toolbar), b, FALSE, FALSE, 0); g_signal_connect(b, "clicked", callback, nullptr); };
    button("Back", G_CALLBACK(OnBack)); button("Forward", G_CALLBACK(OnForward)); button("Refresh", G_CALLBACK(OnRefresh)); button("Home", G_CALLBACK(OnHome));
    address = gtk_entry_new(); gtk_entry_set_placeholder_text(GTK_ENTRY(address), "Enter URL or search"); gtk_box_pack_start(GTK_BOX(toolbar), address, TRUE, TRUE, 0); g_signal_connect(address, "activate", G_CALLBACK(OnAddressActivate), nullptr);
    WebKitWebContext* context = webkit_web_context_new_ephemeral(); view = WEBKIT_WEB_VIEW(webkit_web_view_new_with_context(context)); g_object_unref(context); WebKitUserContentManager* manager = webkit_web_view_get_user_content_manager(view); webkit_user_content_manager_register_script_message_handler(manager, "pane"); g_signal_connect(manager, "script-message-received::pane", G_CALLBACK(OnMessage), nullptr); g_signal_connect(view, "load-changed", G_CALLBACK(OnLoadChanged), nullptr); gtk_box_pack_start(GTK_BOX(root), GTK_WIDGET(view), TRUE, TRUE, 0); gtk_widget_show_all(window); ShowHome();
}
}

int main(int argc, char** argv) { gtk_init(&argc, &argv); BuildUi(); gtk_main(); return 0; }
