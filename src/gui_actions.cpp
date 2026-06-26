#include "gui_actions.h"
#include "gui_state.h"
#include <string>
#include <cstdio>
#include <fstream>
#include <sstream>

void on_copy(GtkWidget*, gpointer)
{
    if (!last_result.valid) return;

    std::string text;
    text += "CBC Ballistics Report\n";
    text += "=====================\n\n";

    char buf[256];
    snprintf(buf, sizeof(buf), "Yaw: %.1f° (CW from N)\n", last_result.yawDeg);
    text += buf;
    snprintf(buf, sizeof(buf), "3D Distance: %.1f m\n", last_result.distance3D);
    text += buf;
    snprintf(buf, sizeof(buf), "H Distance: %.1f m\n", last_result.hDist);
    text += buf;
    snprintf(buf, sizeof(buf), "Height Δ: %+.1f m\n", last_result.heightDelta);
    text += buf;
    snprintf(buf, sizeof(buf), "Muzzle V: %.1f (%.0f bl/s)\n",
             last_result.muzzleVelocity,
             last_result.muzzleVelocity * TICKS_PER_SEC);
    text += buf;
    snprintf(buf, sizeof(buf), "Spread: %.2f°\n", last_result.spread);
    text += buf;

    if (last_result.low.valid) {
        text += "\n-- Low trajectory --\n";
        snprintf(buf, sizeof(buf), "Pitch: %.1f°\n", last_result.low.pitchDeg);
        text += buf;
        snprintf(buf, sizeof(buf), "Flight: %.2f s\n", last_result.low.flightTimeSec);
        text += buf;
        snprintf(buf, sizeof(buf), "Apex: %.1f m\n", last_result.low.apexHeight);
        text += buf;
    }
    if (last_result.high.valid) {
        text += "\n-- High trajectory (mortar) --\n";
        snprintf(buf, sizeof(buf), "Pitch: %.1f°\n", last_result.high.pitchDeg);
        text += buf;
        snprintf(buf, sizeof(buf), "Flight: %.2f s\n", last_result.high.flightTimeSec);
        text += buf;
        snprintf(buf, sizeof(buf), "Apex: %.1f m\n", last_result.high.apexHeight);
        text += buf;
    }

    text += "\n-- Ammo Comparison --\n";
    for (const auto& r : last_result.comparison) {
        if (r.valid)
            snprintf(buf, sizeof(buf), "%s: pitch %.1f° flight %.2fs apex %.1fm\n",
                     r.key.c_str(), r.pitch, r.flight, r.apex);
        else
            snprintf(buf, sizeof(buf), "%s: %s\n", r.key.c_str(), r.error.c_str());
        text += buf;
    }

    GtkClipboard* clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
    gtk_clipboard_set_text(clip, text.c_str(), -1);
}

void on_save_preset(GtkWidget*, gpointer)
{
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Save Preset", NULL, GTK_FILE_CHOOSER_ACTION_SAVE,
        "_Cancel", GTK_RESPONSE_CANCEL,
        "_Save", GTK_RESPONSE_ACCEPT, NULL);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char* path = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        std::ofstream f(path);
        if (f.is_open()) {
            const auto& s = last_setup;
            f << "cx=" << s.cannonPos.x << "\n";
            f << "cy=" << s.cannonPos.y << "\n";
            f << "cz=" << s.cannonPos.z << "\n";
            f << "tx=" << s.targetPos.x << "\n";
            f << "ty=" << s.targetPos.y << "\n";
            f << "tz=" << s.targetPos.z << "\n";
            f << "ammo=" << s.ammoKey << "\n";
            f << "barrel=" << s.barrelKey << "\n";
            f << "dim=" << s.dimKey << "\n";
            f << "len=" << s.barrelLength << "\n";
            f << "charges=" << s.numCharges << "\n";
        }
        g_free(path);
    }
    gtk_widget_destroy(dialog);
}

void on_load_preset(GtkWidget*, gpointer)
{
    GtkWidget* dialog = gtk_file_chooser_dialog_new(
        "Load Preset", NULL, GTK_FILE_CHOOSER_ACTION_OPEN,
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
