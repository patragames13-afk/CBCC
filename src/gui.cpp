#include "gui.h"
#include "gui_state.h"
#include "gui_css.h"
#include "gui_layout.h"
#include "gui_output.h"
#include <gtk/gtk.h>

CalcOutput last_result;
CannonSetup last_setup;

GtkWidget *main_window = NULL;
GtkWidget *entry_cx, *entry_cy, *entry_cz;
GtkWidget *entry_tx, *entry_ty, *entry_tz;
GtkWidget *combo_ammo, *combo_barrel, *combo_dim;
GtkWidget *spin_barrel_len, *spin_charges;
GtkWidget *label_status;
GtkWidget *label_yaw, *label_dist3d, *label_hdist, *label_hdelta;
GtkWidget *label_muzzle, *label_spread;
GtkWidget *label_low_pitch, *label_low_flight, *label_low_apex;
GtkWidget *label_high_pitch, *label_high_flight, *label_high_apex;
GtkWidget *label_high_header;
GtkWidget *drawing_area;
GtkWidget *text_warnings;
GtkWidget *text_comparison;

int runGui(int argc, char* argv[])
{
    gtk_init(&argc, &argv);
    setup_css();

    main_window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(main_window), "CBC Ballistics");
    GtkWidget* window = main_window;
    gtk_window_set_default_size(GTK_WINDOW(window), 880, 820);
    gtk_widget_set_size_request(window, 720, 680);

    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget* title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
        "<span size='x-large' weight='bold' foreground='#dcdcaa'>"
        "CBC Ballistics Calculator</span>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 4);

    build_input_grid(main_box);
    on_barrel_changed(GTK_COMBO_BOX(combo_barrel), NULL);

    GtkWidget* tb = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(main_box), tb, FALSE, TRUE, 4);

    auto add_btn = [&](const char* txt, void (*cb)(GtkWidget*, gpointer)) {
        GtkWidget* b = gtk_button_new_with_label(txt);
        gtk_box_pack_start(GTK_BOX(tb), b, FALSE, FALSE, 0);
        g_signal_connect(b, "clicked", G_CALLBACK(cb), NULL);
    };
    add_btn("Calculate", on_calculate);
    add_btn("Copy", on_copy);
    add_btn("Save", on_save_preset);
    add_btn("Load", on_load_preset);

    label_status = gtk_label_new("");
    gtk_box_pack_start(GTK_BOX(main_box), label_status, FALSE, FALSE, 0);

    // Outer paned: top (results) vs bottom (graph)
    GtkWidget* paned_outer = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(main_box), paned_outer, TRUE, TRUE, 0);

    GtkWidget* top_vbox = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_paned_pack1(GTK_PANED(paned_outer), top_vbox, TRUE, TRUE);

    // Inner paned: trajectory/stats vs comparison
    GtkWidget* paned_inner = gtk_paned_new(GTK_ORIENTATION_VERTICAL);
    gtk_box_pack_start(GTK_BOX(top_vbox), paned_inner, TRUE, TRUE, 0);

    GtkWidget* stats_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
    gtk_paned_pack1(GTK_PANED(paned_inner), stats_box, TRUE, TRUE);

    build_traj_stats(stats_box);

    GtkWidget* comp = build_comparison_frame();
    gtk_paned_pack2(GTK_PANED(paned_inner), comp, TRUE, TRUE);
    gtk_paned_set_position(GTK_PANED(paned_inner), 170);

    text_warnings = gtk_label_new("");
    gtk_widget_set_name(text_warnings, "warnings");
    gtk_box_pack_start(GTK_BOX(top_vbox), text_warnings, FALSE, FALSE, 0);

    // Drawing area
    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_widget_set_vexpand(drawing_area, TRUE);
    gtk_widget_set_size_request(drawing_area, -1, 180);
    g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(on_draw), NULL);
    gtk_paned_pack2(GTK_PANED(paned_outer), drawing_area, TRUE, TRUE);
    gtk_paned_set_position(GTK_PANED(paned_outer), 400);

    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

    on_calculate(NULL, NULL);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
