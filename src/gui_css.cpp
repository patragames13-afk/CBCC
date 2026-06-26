#include "gui_css.h"

void setup_css()
{
    GtkCssProvider* css = gtk_css_provider_new();
    const char* css_data =
        "window { background-color: #1a1a1a; color: #d4d4d4; }\n"
        "label  { color: #d4d4d4; background-color: transparent; }\n"
        "entry, spinbutton entry {\n"
        "  background-color: #252526; color: #e0e0e0; border: 1px solid #3c3c3c;\n"
        "}\n"
        "entry:focus, spinbutton entry:focus {\n"
        "  background-color: #1e1e1e; color: #ffffff; border: 1px solid #0e639c;\n"
        "}\n"
        "entry:selected, spinbutton entry:selected {\n"
        "  background-color: #264f78; color: #ffffff;\n"
        "}\n"
        "combobox button {\n"
        "  background-color: #252526; color: #e0e0e0; border: 1px solid #3c3c3c;\n"
        "}\n"
        "combobox button:focus {\n"
        "  background-color: #1e1e1e; color: #ffffff; border: 1px solid #0e639c;\n"
        "}\n"
        "combobox window, combobox popup {\n"
        "  background-color: #252526; color: #e0e0e0;\n"
        "}\n"
        "spinbutton button {\n"
        "  background-color: #2d2d2d; color: #d4d4d4; border: 1px solid #3c3c3c;\n"
        "}\n"
        "spinbutton button:hover { background-color: #3c3c3c; }\n"
        "button { background-color: #0e639c; color: #fff; border: none; padding: 4px 12px; }\n"
        "button:hover { background-color: #1177bb; }\n"
        "#status-ok    { color: #4ec9b0; font-weight: bold; font-size: 1.1em; }\n"
        "#status-error { color: #f44747; font-weight: bold; font-size: 1.1em; }\n"
        ".input-label { color: #9e9e9e; font-size: 0.95em; }\n"
        ".output-label { color: #9e9e9e; font-size: 1em; }\n"
        ".value-label { color: #e0e0e0; font-family: monospace; font-size: 1.15em; font-weight: bold; }\n"
        "#warnings { color: #ce9178; font-size: 0.95em; background-color: transparent; }\n"
        "frame { color: #569cd6; border: 1px solid #333; border-radius: 4px; }\n"
        "frame > label { color: #569cd6; font-weight: bold; font-size: 0.95em; padding: 0 4px; background-color: transparent; }\n"
        "textview, textview text {\n"
        "  background-color: #1e1e1e; color: #c8c8c8;\n"
        "}\n"
        "textview text:selected { background-color: #264f78; color: #ffffff; }\n"
        "#comparison { background-color: #1e1e1e; color: #c8c8c8; font-family: monospace; font-size: 0.9em; }\n"
        "scrolledwindow { border: 1px solid #3c3c3c; }\n"
        "viewport { background-color: #1a1a1a; }\n"
        "scrolledwindow undershoot.top, scrolledwindow undershoot.bottom,\n"
        "scrolledwindow undershoot.left, scrolledwindow undershoot.right {\n"
        "  background-color: transparent;\n"
        "}\n"
        "scrollbar { background-color: #2d2d2d; border: none; }\n"
        "scrollbar trough { background-color: #2d2d2d; border: none; }\n"
        "scrollbar slider { background-color: #424242; border: none; border-radius: 4px; min-width: 8px; min-height: 8px; }\n"
        "scrollbar slider:hover { background-color: #555555; }\n"
        "tooltip { background-color: #252526; color: #d4d4d4; border: 1px solid #3c3c3c; }\n"
        "tooltip label { color: #d4d4d4; }";
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
