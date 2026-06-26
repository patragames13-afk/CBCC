#include "gui_layout.h"
#include "gui_state.h"
#include "gui_css.h"
#include "data.h"

static void make_expand(GtkWidget* w)
{
    gtk_widget_set_hexpand(w, TRUE);
    gtk_widget_set_halign(w, GTK_ALIGN_FILL);
}

void build_input_grid(GtkWidget* box)
{
    GtkWidget* grid = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(grid), 8);
    gtk_grid_set_row_spacing(GTK_GRID(grid), 6);
    gtk_box_pack_start(GTK_BOX(box), grid, FALSE, TRUE, 0);

    auto add_lbl = [&](const char* txt, int x, int y) {
        GtkWidget* lbl = gtk_label_new(txt);
        gtk_widget_set_halign(lbl, GTK_ALIGN_END);
        gtk_grid_attach(GTK_GRID(grid), lbl, x, y, 1, 1);
    };
    auto add_ent = [&](GtkWidget*& out, int x, int y) {
        out = gtk_entry_new();
        make_expand(out);
        gtk_entry_set_width_chars(GTK_ENTRY(out), 7);
        gtk_grid_attach(GTK_GRID(grid), out, x, y, 1, 1);
    };
    auto add_combo = [&](GtkWidget*& out, int x, int y) {
        out = gtk_combo_box_text_new();
        make_expand(out);
        gtk_grid_attach(GTK_GRID(grid), out, x, y, 1, 1);
    };

    add_lbl("Cannon X:", 0, 0); add_ent(entry_cx, 1, 0);
    gtk_entry_set_text(GTK_ENTRY(entry_cx), "0");
    add_lbl("Y:", 2, 0); add_ent(entry_cy, 3, 0);
    gtk_entry_set_text(GTK_ENTRY(entry_cy), "64");
    add_lbl("Z:", 4, 0); add_ent(entry_cz, 5, 0);
    gtk_entry_set_text(GTK_ENTRY(entry_cz), "0");

    add_lbl("Target X:", 0, 1); add_ent(entry_tx, 1, 1);
    gtk_entry_set_text(GTK_ENTRY(entry_tx), "150");
    add_lbl("Y:", 2, 1); add_ent(entry_ty, 3, 1);
    gtk_entry_set_text(GTK_ENTRY(entry_ty), "64");
    add_lbl("Z:", 4, 1); add_ent(entry_tz, 5, 1);
    gtk_entry_set_text(GTK_ENTRY(entry_tz), "100");

    add_lbl("Ammo:", 0, 2);
    add_combo(combo_ammo, 1, 2);
    for (const auto& [k, v] : AMMO_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_ammo), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_ammo), 0);

    add_lbl("Barrel:", 2, 2);
    add_combo(combo_barrel, 3, 2);
    for (const auto& [k, v] : BARREL_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_barrel), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_barrel), 0);

    add_lbl("Dim:", 4, 2);
    add_combo(combo_dim, 5, 2);
    for (const auto& [k, v] : DIMENSION_TABLE)
        gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo_dim), k.c_str());
    gtk_combo_box_set_active(GTK_COMBO_BOX(combo_dim), 0);

    add_lbl("Barrel len:", 0, 3);
    spin_barrel_len = gtk_spin_button_new_with_range(1, 64, 1);
    make_expand(spin_barrel_len);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_barrel_len), 4);
    gtk_grid_attach(GTK_GRID(grid), spin_barrel_len, 1, 3, 1, 1);

    add_lbl("Charges:", 2, 3);
    spin_charges = gtk_spin_button_new_with_range(0, 50, 1);
    make_expand(spin_charges);
    gtk_spin_button_set_value(GTK_SPIN_BUTTON(spin_charges), 3);
    gtk_grid_attach(GTK_GRID(grid), spin_charges, 3, 3, 1, 1);

    gtk_grid_set_column_homogeneous(GTK_GRID(grid), TRUE);
}
