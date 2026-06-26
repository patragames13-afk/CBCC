#include "gui_state.h"
#include "calculator.h"
#include <string>
#include <cstdio>

void on_calculate(GtkWidget*, gpointer)
{
    CannonSetup s;

    auto read_double = [](GtkWidget* w) -> double {
        const char* txt = gtk_entry_get_text(GTK_ENTRY(w));
        try { return std::stod(txt); } catch (...) { return 0.0; }
    };

    s.cannonPos.x = read_double(entry_cx);
    s.cannonPos.y = read_double(entry_cy);
    s.cannonPos.z = read_double(entry_cz);
    s.targetPos.x = read_double(entry_tx);
    s.targetPos.y = read_double(entry_ty);
    s.targetPos.z = read_double(entry_tz);

    s.ammoKey   = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_ammo));
    s.barrelKey = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_barrel));
    s.dimKey    = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(combo_dim));

    s.barrelLength = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_barrel_len));
    s.numCharges   = (int)gtk_spin_button_get_value(GTK_SPIN_BUTTON(spin_charges));

    last_setup = s;
    last_result = calculate(s);

    char buf[128];

    if (!last_result.valid) {
        gtk_label_set_text(GTK_LABEL(label_status), last_result.errorMsg.c_str());
        gtk_widget_set_name(label_status, "status-error");
    } else {
        gtk_label_set_text(GTK_LABEL(label_status), "OK");
        gtk_widget_set_name(label_status, "status-ok");
    }

    auto set_or_clear = [&](GtkWidget* w, const char* fmt, auto val) {
        if (last_result.valid) {
            snprintf(buf, sizeof(buf), fmt, val);
            gtk_label_set_text(GTK_LABEL(w), buf);
        } else {
            gtk_label_set_text(GTK_LABEL(w), "-");
        }
    };

    set_or_clear(label_yaw,    "%.1f°", last_result.yawDeg);
    set_or_clear(label_dist3d, "%.1f m", last_result.distance3D);
    set_or_clear(label_hdist,  "%.1f m", last_result.hDist);
    set_or_clear(label_hdelta, "%+.1f m", last_result.heightDelta);

    if (last_result.valid) {
        snprintf(buf, sizeof(buf), "%.1f (%.0f bl/s)", last_result.muzzleVelocity,
                 last_result.muzzleVelocity * TICKS_PER_SEC);
        gtk_label_set_text(GTK_LABEL(label_muzzle), buf);
    } else {
        gtk_label_set_text(GTK_LABEL(label_muzzle), "-");
    }
    set_or_clear(label_spread, "%.2f°", last_result.spread);

    // Low trajectory
    if (last_result.low.valid) {
        snprintf(buf, sizeof(buf), "%.1f°", last_result.low.pitchDeg);
        gtk_label_set_text(GTK_LABEL(label_low_pitch), buf);
        snprintf(buf, sizeof(buf), "%.2f s", last_result.low.flightTimeSec);
        gtk_label_set_text(GTK_LABEL(label_low_flight), buf);
        snprintf(buf, sizeof(buf), "%.1f m", last_result.low.apexHeight);
        gtk_label_set_text(GTK_LABEL(label_low_apex), buf);
    }

    // High trajectory
    if (last_result.high.valid) {
        gtk_widget_set_visible(label_high_header, TRUE);
        snprintf(buf, sizeof(buf), "%.1f°", last_result.high.pitchDeg);
        gtk_label_set_text(GTK_LABEL(label_high_pitch), buf);
        snprintf(buf, sizeof(buf), "%.2f s", last_result.high.flightTimeSec);
        gtk_label_set_text(GTK_LABEL(label_high_flight), buf);
        snprintf(buf, sizeof(buf), "%.1f m", last_result.high.apexHeight);
        gtk_label_set_text(GTK_LABEL(label_high_apex), buf);
    } else {
        gtk_widget_set_visible(label_high_header, FALSE);
    }

    // Warnings
    std::string warn_text;
    for (auto& w : last_result.warnings)
        warn_text += "⚠ " + w + "\n";
    if (warn_text.empty())
        warn_text = last_result.valid ? "No warnings" : "";
    gtk_label_set_text(GTK_LABEL(text_warnings), warn_text.c_str());

    // Comparison text
    std::string comp;
    for (const auto& r : last_result.comparison) {
        if (r.valid)
            snprintf(buf, sizeof(buf), "%-14s → pitch %5.1f° flight %.2fs apex %.1f m\n",
                     r.key.c_str(), r.pitch, r.flight, r.apex);
        else
            snprintf(buf, sizeof(buf), "%-14s → %s\n", r.key.c_str(), r.error.c_str());
        comp += buf;
    }
    gtk_text_buffer_set_text(
        gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_comparison)),
        comp.c_str(), -1);

    gtk_widget_queue_draw(drawing_area);
}
