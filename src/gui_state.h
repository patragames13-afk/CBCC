#pragma once
#include "types.h"
#include <gtk/gtk.h>

extern GtkWidget *entry_cx, *entry_cy, *entry_cz;
extern GtkWidget *entry_tx, *entry_ty, *entry_tz;
extern GtkWidget *combo_ammo, *combo_barrel, *combo_dim;
extern GtkWidget *spin_barrel_len, *spin_charges;
extern GtkWidget *label_status;
extern GtkWidget *label_yaw, *label_dist3d, *label_hdist, *label_hdelta;
extern GtkWidget *label_muzzle, *label_spread;
extern GtkWidget *label_low_pitch, *label_low_flight, *label_low_apex;
extern GtkWidget *label_high_pitch, *label_high_flight, *label_high_apex;
extern GtkWidget *label_high_header;
extern GtkWidget *drawing_area;
extern GtkWidget *text_warnings;
extern GtkWidget *text_comparison;

extern CalcOutput last_result;
extern CannonSetup last_setup;

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer data);
void on_calculate(GtkWidget* widget, gpointer data);
void on_barrel_changed(GtkComboBox* combo, gpointer data);
void on_copy(GtkWidget* widget, gpointer data);
void on_save_preset(GtkWidget* widget, gpointer data);
void on_load_preset(GtkWidget* widget, gpointer data);
