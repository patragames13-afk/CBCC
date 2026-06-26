#include "gui_actions.h"
#include "gui_state.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

void on_copy(GtkWidget*, gpointer)
{
    if (!last_result.valid) return;
    std::string text = "CBC Ballistics Report\n=====================\n\n";
    char buf[256];

    auto add = [&](const char* fmt, auto... args) {
        snprintf(buf, sizeof(buf), fmt, args...);
        text += buf;
    };

    add("Yaw: %.1f° (CW from N)\n", last_result.yawDeg);
    add("3D Distance: %.1f m\n", last_result.distance3D);
    add("H Distance: %.1f m\n", last_result.hDist);
    add("Height Δ: %+.1f m\n", last_result.heightDelta);
    add("Muzzle V: %.1f (%.0f bl/s)\n", last_result.muzzleVelocity, last_result.muzzleVelocity * TICKS_PER_SEC);
    add("Spread: %.2f°\n", last_result.spread);

    if (last_result.low.valid) {
        text += "\n-- Low trajectory --\n";
        add("Pitch: %.1f°\n", last_result.low.pitchDeg);
        add("Flight: %.2f s\n", last_result.low.flightTimeSec);
        add("Apex: %.1f m\n", last_result.low.apexHeight);
    }
    if (last_result.high.valid) {
        text += "\n-- High trajectory (mortar) --\n";
        add("Pitch: %.1f°\n", last_result.high.pitchDeg);
        add("Flight: %.2f s\n", last_result.high.flightTimeSec);
        add("Apex: %.1f m\n", last_result.high.apexHeight);
    }
    text += "\n-- Ammo Comparison --\n";
    for (const auto& r : last_result.comparison) {
        if (r.valid)
            add("%s: pitch %.1f° flight %.2fs apex %.1fm\n", r.key.c_str(), r.pitch, r.flight, r.apex);
        else
            add("%s: %s\n", r.key.c_str(), r.error.c_str());
    }
    GtkClipboard* clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clip, text.c_str(), -1);
}

static CannonSetup read_ui()
{
    CannonSetup s;
    auto rd = [](GtkWidget* w) {
        const char* t = gtk_entry_get_text(GTK_ENTRY(w));
        try { return std::stod(t); } catch (...) { return 0.0; }
    };
    s.cannonPos.x = rd(entry_cx);
    s.cannonPos.y = rd(entry_cy);
    s.cannonPos.z = rd(entry_cz);
    s.targetPos.x = rd(entry_tx);
    s.targetPos.y = rd(entry_ty);
    s.targetPos.z = rd(entry_tz);
    s.ammoKey   = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_ammo));
    s.barrelKey = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_barrel));
    s.dimKey    = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_dim));
    s.barrelLength = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_barrel_len));
    s.numCharges   = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_charges));
    return s;
}

void on_save_preset(GtkWidget*, gpointer)
{
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Save Preset", GTK_WINDOW(main_window), GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Save", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        CannonSetup s = read_ui();
        std::ofstream f(path);
        if (f.is_open()) {
            f << "cx=" << s.cannonPos.x << "\ncy=" << s.cannonPos.y << "\ncz=" << s.cannonPos.z << "\n";
            f << "tx=" << s.targetPos.x << "\nty=" << s.targetPos.y << "\ntz=" << s.targetPos.z << "\n";
            f << "ammo=" << s.ammoKey << "\nbarrel=" << s.barrelKey << "\ndim=" << s.dimKey << "\n";
            f << "len=" << s.barrelLength << "\ncharges=" << s.numCharges << "\n";
        }
        g_free(path);
    }
    gtk_widget_destroy(dialog);
}
void on_load_preset(GtkWidget*, gpointer)
{
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Load Preset", GTK_WINDOW(main_window), GTK_FILE_CHOOSER_ACTION_OPEN,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Open", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::ifstream f(path);
        if (f.is_open()) {
            CannonSetup s;
            std::string line;
            while (std::getline(f, line)) {
                auto eq = line.find('=');
                if (eq == std::string::npos) continue;
                std::string key = line.substr(0, eq);
                std::string val = line.substr(eq + 1);
                if (key == "cx") s.cannonPos.x = std::stod(val);
                else if (key == "cy") s.cannonPos.y = std::stod(val);
                else if (key == "cz") s.cannonPos.z = std::stod(val);
                else if (key == "tx") s.targetPos.x = std::stod(val);
                else if (key == "ty") s.targetPos.y = std::stod(val);
                else if (key == "tz") s.targetPos.z = std::stod(val);
                else if (key == "ammo") s.ammoKey = val;
                else if (key == "barrel") s.barrelKey = val;
                else if (key == "dim") s.dimKey = val;
                else if (key == "len") s.barrelLength = std::stoi(val);
                else if (key == "charges") s.numCharges = std::stoi(val);
            }
            last_setup = s;
            gtk_entry_set_text(GTK_ENTRY(entry_cx), std::to_string(s.cannonPos.x).c_str());
            gtk_entry_set_text(GTK_ENTRY(entry_cy), std::to_string(s.cannonPos.y).c_str());
            gtk_entry_set_text(GTK_ENTRY(entry_cz), std::to_string(s.cannonPos.z).c_str());
            gtk_entry_set_text(GTK_ENTRY(entry_tx), std::to_string(s.targetPos.x).c_str());
            gtk_entry_set_text(GTK_ENTRY(entry_ty), std::to_string(s.targetPos.y).c_str());
            gtk_entry_set_text(GTK_ENTRY(entry_tz), std::to_string(s.targetPos.z).c_str());
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_barrel_len), s.barrelLength);
            gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_charges), s.numCharges);
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_ammo), 0);
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_barrel), 0);
            gtk_combo_box_set_active(GTK_COMBO_BOX(combo_dim), 0);
            on_calculate(NULL, NULL);
        }
        g_free(path);
    }
    gtk_widget_destroy(dialog);
}
