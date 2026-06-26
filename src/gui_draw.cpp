#include "gui_state.h"
#include <cmath>
#include <cstdio>
#include <algorithm>

static void draw_arc(cairo_t* cr, const SimResult& sim,
                     double ox, double oy, double sx, double sy,
                     double yMin, double r, double g, double b)
{
    if (sim.arc.empty()) return;
    auto toX = [&](double x) { return ox + x * sx; };
    auto toY = [&](double h) { return oy - (h - yMin) * sy; };

    cairo_set_source_rgb(cr, r, g, b);
    cairo_set_line_width(cr, 2.0);
    bool first = true;
    for (auto& p : sim.arc) {
        double px = toX(p.dist);
        double py = toY(p.height);
        if (first) { cairo_move_to(cr, px, py); first = false; }
        else       { cairo_line_to(cr, px, py); }
    }
    cairo_stroke(cr);
}

gboolean on_draw(GtkWidget* widget, cairo_t* cr, gpointer)
{
    int W = gtk_widget_get_allocated_width(widget);
    int H = gtk_widget_get_allocated_height(widget);

    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
    cairo_paint(cr);
    if (!last_result.valid) return FALSE;

    const auto& low  = last_result.low.sim;
    const auto& high = last_result.high.sim;
    if (low.arc.empty() && high.arc.empty()) return FALSE;

    // Bounds
    double xMax = 1, yMin = 0, yMax = 1;
    auto grow = [&](const SimResult& s) {
        if (s.arc.empty()) return;
        for (auto& p : s.arc) {
            if (p.height > yMax) yMax = p.height;
            if (p.height < yMin) yMin = p.height;
        }
        if (s.arc.back().dist > xMax) xMax = s.arc.back().dist;
    };
    grow(low); grow(high);
    yMin = std::min({yMin, low.arc.empty() ? 0.0 : low.arc.back().height,
                     high.arc.empty() ? 0.0 : high.arc.back().height}) - 1;
    yMax += 1;
    if (yMin > 0) yMin = -1;

    // Plot area
    int mL = 55, mR = 10, mT = 10, mB = 22;
    double pw = W - mL - mR;
    double ph = H - mT - mB;
    double sx = pw / xMax;
    double sy = ph / (yMax - yMin);
    double ox = mL, oy = mT + ph;

    auto toX = [&](double x) { return ox + x * sx; };
    auto toY = [&](double h) { return oy - (h - yMin) * sy; };

    // Grid + Y labels
    cairo_set_font_size(cr, 10);
    cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
    cairo_set_line_width(cr, 0.5);
    char buf[32];
    for (int i = 0; i <= 4; ++i) {
        double v = yMin + (yMax - yMin) * i / 4.0;
        double py = toY(v);
        cairo_move_to(cr, ox, py); cairo_line_to(cr, ox + pw, py);
        cairo_stroke(cr);
        cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
        snprintf(buf, sizeof(buf), "%.0f", v);
        cairo_move_to(cr, 2, py - 5);
        cairo_show_text(cr, buf);
        cairo_set_source_rgb(cr, 0.25, 0.25, 0.25);
    }
    // X labels
    cairo_set_source_rgb(cr, 0.55, 0.55, 0.55);
    for (int i = 0; i <= 4; ++i) {
        double v = xMax * i / 4.0;
        double px = toX(v);
        snprintf(buf, sizeof(buf), "%.0f", v);
        cairo_move_to(cr, px - 8, oy + 16);
        cairo_show_text(cr, buf);
    }

    // Axis titles
    cairo_set_source_rgb(cr, 0.6, 0.6, 0.6);
    cairo_move_to(cr, 6, mT + ph/2 + 4);
    cairo_show_text(cr, "H");
    cairo_move_to(cr, ox + pw/2 - 28, H - 4);
    cairo_show_text(cr, "Distance (m)");

    // Arcs
    draw_arc(cr, low, ox, oy, sx, sy, yMin, 0.2, 0.7, 1.0);
    if (!high.arc.empty())
        draw_arc(cr, high, ox, oy, sx, sy, yMin, 1.0, 0.6, 0.1);

    // Markers
    cairo_set_source_rgb(cr, 0.0, 0.9, 0.3);
    cairo_arc(cr, toX(0), toY(0), 5, 0, 2*M_PI);
    cairo_fill(cr);

    double td = low.arc.empty() ? high.arc.back().dist : low.arc.back().dist;
    double th = low.arc.empty() ? high.arc.back().height : low.arc.back().height;
    cairo_set_source_rgb(cr, 0.9, 0.2, 0.2);
    cairo_rectangle(cr, toX(td)-5, toY(th)-5, 10, 10);
    cairo_fill(cr);

    // Legend
    int lx = ox + 8, ly = mT + 6;
    int rows = high.arc.empty() ? 3 : 4;
    cairo_set_source_rgba(cr, 0, 0, 0, 0.55);
    cairo_rectangle(cr, lx, ly, 82, rows * 16 + 4);
    cairo_fill(cr);

    auto legend_item = [&](const char* txt, double r, double g, double b, int row) {
        cairo_set_source_rgb(cr, r, g, b);
        cairo_rectangle(cr, lx+4, ly+4+row*16, 10, 10);
        cairo_fill(cr);
        cairo_set_source_rgb(cr, 0.85, 0.85, 0.85);
        cairo_move_to(cr, lx+18, ly+13+row*16);
        cairo_show_text(cr, txt);
    };
    legend_item("Low",     0.2, 0.7, 1.0, 0);
    legend_item("Cannon",  0.0, 0.9, 0.3, 1);
    legend_item("Target",  0.9, 0.2, 0.2, 2);
    if (!high.arc.empty())
        legend_item("High", 1.0, 0.6, 0.1, 3);

    return FALSE;
}
