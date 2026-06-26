#include "gui_layout.h"
#include "gui_state.h"
#include "gui_css.h"
#include "data.h"

static GtkWidget* make_frame(GtkWidget* box, const char* title)
{
    GtkWidget* frame = gtk_frame_new(title);
    gtk_box_pack_start(GTK_BOX(box), frame, FALSE, TRUE, 0);
    GtkWidget* inner = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_container_add(GTK_CONTAINER(frame), inner);
    return inner;
}

static void add_entry_row(GtkWidget* inner, const char* label,
                           GtkWidget*& out, const char* def)
{
    GtkWidget* lbl = gtk_label_new(label);
    add_class(lbl, "input-label");
    gtk_box_pack_start(GTK_BOX(inner), lbl, FALSE, FALSE, 0);
    out = gtk_entry_new();
    gtk_entry_set_width_chars(GTK_ENTRY(out), 7);
    gtk_widget_set_hexpand(out, TRUE);
    gtk_entry_set_text(GTK_ENTRY(out), def);
    g_signal_connect(out, "insert-text", G_CALLBACK(on_paste_coords), NULL);
    gtk_box_pack_start(GTK_BOX(inner), out, FALSE, FALSE, 0);
}

void build_input_grid(GtkWidget* box)
{
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, TRUE, 0);

    GtkWidget* cbox = make_frame(hbox, "Cannon");
    add_entry_row(cbox, "X", entry_cx, "0");
    add_entry_row(cbox, "Y", entry_cy, "64");
    add_entry_row(cbox, "Z", entry_cz, "0");

    GtkWidget* tbox = make_frame(hbox, "Target");
    add_entry_row(tbox, "X", entry_tx, "150");
    add_entry_row(tbox, "Y", entry_ty, "64");
    add_entry_row(tbox, "Z", entry_tz, "100");

    // Second row: config
    GtkWidget* cfg = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(box), cfg, FALSE, TRUE, 0);

    auto add_combo = [&](const char* lbl, GtkWidget*& out) {
        GtkWidget* l = gtk_label_new(lbl);
        add_class(l, "input-label");
        gtk_box_pack_start(GTK_BOX(cfg), l, FALSE, FALSE, 0);
        out = gtk_combo_box_text_new();
        gtk_widget_set_hexpand(out, TRUE);
        gtk_box_pack_start(GTK_BOX(cfg), out, TRUE, TRUE, 0);
    };

    add_combo("Ammo", combo_ammo);
    for (const auto& [k, v] : AMMO_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ammo), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_ammo), 0);

    add_combo("Barrel", combo_barrel);
    for (const auto& [k, v] : BARREL_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_barrel), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_barrel), 0);
    g_signal_connect(combo_barrel, "changed", G_CALLBACK(on_barrel_changed), NULL);

    add_combo("Dim", combo_dim);
    for (const auto& [k, v] : DIMENSION_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_dim), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_dim), 0);

    // Third row: numbers
    GtkWidget* nums = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(box), nums, FALSE, TRUE, 0);

    auto add_spin = [&](const char* lbl, GtkWidget*& out, double min, double max, double def) {
        GtkWidget* l = gtk_label_new(lbl);
        add_class(l, "input-label");
        gtk_box_pack_start(GTK_BOX(nums), l, FALSE, FALSE, 0);
        out = gtk_spin_button_new_with_range(min, max, 1);
        gtk_spin_button_set_value(GTK_SPIN_BUTTON(out), def);
        gtk_widget_set_hexpand(out, TRUE);
        gtk_box_pack_start(GTK_BOX(nums), out, FALSE, FALSE, 0);
    };

    add_spin("Barrel len", spin_barrel_len, 1, 64, 4);
    add_spin("Charges", spin_charges, 0, 50, 3);
}
