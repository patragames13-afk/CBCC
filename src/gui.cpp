#include "gui.h"
#include "gui_state.h"
#include "gui_css.h"
#include "gui_layout.h"
#include "gui_output.h"
#include <gtk/gtk.h>

CalcOutput last_result;
CannonSetup last_setup;

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

    GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "CBC Ballistics");
    gtk_window_set_default_size(GTK_WINDOW(window), 860, 800);
    gtk_widget_set_size_request(window, 620, 600);

    GtkWidget* main_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_container_set_border_width(GTK_CONTAINER(main_box), 10);
    gtk_container_add(GTK_CONTAINER(window), main_box);

    GtkWidget* title = gtk_label_new(NULL);
    gtk_label_set_markup(GTK_LABEL(title),
        "<span size='x-large' weight='bold' foreground='#569cd6'>"
        "CBC Ballistics Calculator</span>");
    gtk_box_pack_start(GTK_BOX(main_box), title, FALSE, FALSE, 0);

    build_input_grid(main_box);

    // Toolbar
    GtkWidget* tb = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 6);
    gtk_box_pack_start(GTK_BOX(main_box), tb, FALSE, TRUE, 0);

    GtkWidget* btn_calc = gtk_button_new_with_label("Calculate");
    gtk_box_pack_start(GTK_BOX(tb), btn_calc, FALSE, FALSE, 0);
    g_signal_connect(btn_calc, "clicked", G_CALLBACK(on_calculate), NULL);

    GtkWidget* btn_copy = gtk_button_new_with_label("Copy");
    gtk_box_pack_start(GTK_BOX(tb), btn_copy, FALSE, FALSE, 0);
    g_signal_connect(btn_copy, "clicked", G_CALLBACK(on_copy), NULL);

    GtkWidget* btn_save = gtk_button_new_with_label("Save");
    gtk_box_pack_start(GTK_BOX(tb), btn_save, FALSE, FALSE, 0);
    g_signal_connect(btn_save, "clicked", G_CALLBACK(on_save_preset), NULL);

    GtkWidget* btn_load = gtk_button_new_with_label("Load");
    gtk_box_pack_start(GTK_BOX(tb), btn_load, FALSE, FALSE, 0);
    g_signal_connect(btn_load, "clicked", G_CALLBACK(on_load_preset), NULL);

    label_status = gtk_label_new("");
    gtk_widget_set_margin_bottom(label_status, 2);
    gtk_box_pack_start(GTK_BOX(main_box), label_status, FALSE, FALSE, 0);

    build_output_area(main_box);

    text_warnings = gtk_label_new("");
    gtk_widget_set_name(text_warnings, "warnings");
    gtk_widget_set_margin_top(text_warnings, 4);
    gtk_box_pack_start(GTK_BOX(main_box), text_warnings, FALSE, FALSE, 0);

    drawing_area = gtk_drawing_area_new();
    gtk_widget_set_hexpand(drawing_area, TRUE);
    gtk_widget_set_vexpand(drawing_area, TRUE);
    gtk_widget_set_size_request(drawing_area, -1, 180);
    g_signal_connect(G_OBJECT(drawing_area), "draw",
                     G_CALLBACK(on_draw), NULL);
    gtk_box_pack_start(GTK_BOX(main_box), drawing_area, TRUE, TRUE, 0);

    g_signal_connect(window, "destroy",
                     G_CALLBACK(gtk_main_quit), NULL);

    on_calculate(NULL, NULL);

    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
