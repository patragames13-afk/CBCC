#include "gui_output.h"
#include "gui_state.h"
#include "gui_css.h"

static GtkWidget* add_traj_section(GtkWidget* box, const char* title,
                                    GtkWidget*& pitch, GtkWidget*& flight,
                                    GtkWidget*& apex)
{
    GtkWidget* frame = gtk_frame_new(title);
    gtk_box_pack_start(GTK_BOX(box), frame, TRUE, TRUE, 0);

    GtkWidget* inner = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(inner), 6);
    gtk_grid_set_row_spacing(GTK_GRID(inner), 2);
    gtk_container_add(GTK_CONTAINER(frame), inner);

    auto add_row = [&](const char* lbl, GtkWidget*& val, int row) {
        GtkWidget* l = gtk_label_new(lbl);
        gtk_widget_set_halign(l, GTK_ALIGN_END);
        add_class(l, "output-label");
        gtk_grid_attach(GTK_GRID(inner), l, 0, row, 1, 1);
        val = gtk_label_new("-");
        gtk_widget_set_halign(val, GTK_ALIGN_START);
        gtk_widget_set_hexpand(val, TRUE);
        add_class(val, "value-label");
        gtk_grid_attach(GTK_GRID(inner), val, 1, row, 1, 1);
    };

    add_row("Pitch:", pitch, 0);
    add_row("Flight:", flight, 1);
    add_row("Apex:", apex, 2);
    return frame;
}

void build_traj_stats(GtkWidget* box)
{
    GtkWidget* hbox = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_box_pack_start(GTK_BOX(box), hbox, FALSE, TRUE, 0);

    add_traj_section(hbox, "Low (direct)", label_low_pitch,
                     label_low_flight, label_low_apex);
    label_high_header = add_traj_section(hbox, "High (mortar)", label_high_pitch,
                                          label_high_flight, label_high_apex);

    GtkWidget* stats = gtk_grid_new();
    gtk_grid_set_column_spacing(GTK_GRID(stats), 10);
    gtk_grid_set_row_spacing(GTK_GRID(stats), 3);
    gtk_box_pack_start(GTK_BOX(box), stats, FALSE, TRUE, 0);

    auto add_stat = [&](const char* lbl, GtkWidget*& val, int col, int row) {
        GtkWidget* l = gtk_label_new(lbl);
        gtk_widget_set_halign(l, GTK_ALIGN_END);
        add_class(l, "output-label");
        gtk_grid_attach(GTK_GRID(stats), l, col*2, row, 1, 1);
        val = gtk_label_new("-");
        gtk_widget_set_halign(val, GTK_ALIGN_START);
        gtk_widget_set_hexpand(val, TRUE);
        add_class(val, "value-label");
        gtk_grid_attach(GTK_GRID(stats), val, col*2+1, row, 1, 1);
    };

    add_stat("Yaw:", label_yaw, 0, 0);
    add_stat("3D Dist:", label_dist3d, 1, 0);
    add_stat("H Dist:", label_hdist, 2, 0);
    add_stat("Height Δ:", label_hdelta, 0, 1);
    add_stat("Muzzle V:", label_muzzle, 1, 1);
    add_stat("Spread:", label_spread, 2, 1);
}

GtkWidget* build_comparison_frame()
{
    GtkWidget* frame = gtk_frame_new("Ammo Comparison");

    text_comparison = gtk_text_view_new();
    gtk_text_view_set_editable(GTK_TEXT_VIEW(text_comparison), FALSE);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(text_comparison), FALSE);
    gtk_widget_set_name(text_comparison, "comparison");

    GtkWidget* scroll = gtk_scrolled_window_new(NULL, NULL);
    gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scroll),
        GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);
    gtk_container_add(GTK_CONTAINER(scroll), text_comparison);
    gtk_container_add(GTK_CONTAINER(frame), scroll);
    return frame;
}
