#include "gui_css.h"

void setup_css()
{
    GtkCssProvider* css = gtk_css_provider_new();
    const char* css_data =
        "window { background-color: #1e1e1e; color: #d4d4d4; }\n"
        "label     { color: #d4d4d4; }\n"
        "entry     { background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; }\n"
        "combobox button { background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; }\n"
        "button    { background-color: #0e639c; color: #ffffff; border: none; }\n"
        "button:hover { background-color: #1177bb; }\n"
        "spinbutton button { background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; }\n"
        "spinbutton entry { background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c; }\n"
        "#status-ok    { color: #4ec9b0; font-weight: bold; }\n"
        "#status-error { color: #f44747; font-weight: bold; }\n"
        ".value-label { color: #d4d4d4; font-family: monospace; font-size: 1.05em; }\n"
        ".output-label { color: #dcdcaa; font-weight: bold; }\n"
        "#warnings { color: #ce9178; }\n"
        "frame { color: #569cd6; border: 1px solid #3c3c3c; }\n"
        "#comparison { background-color: #1e1e1e; color: #d4d4d4; font-family: monospace; font-size: 0.95em; }\n"
        "scrolledwindow { border: 1px solid #3c3c3c; }";
    gtk_css_provider_load_from_data(css, css_data, -1, nullptr);
    gtk_style_context_add_provider_for_screen(
        gdk_screen_get_default(),
        GTK_STYLE_PROVIDER(css),
        GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}

void add_class(GtkWidget* w, const char* cls)
{
    gtk_style_context_add_class(
        gtk_widget_get_style_context(w), cls);
}
