#include "gui_state.h"
#include <cmath>

static void draw_arc(cairo_t* cr, const SimResult& sim,
                     double margin, double plotW, double plotH,
                     double yLo, double yHi, double xMax,
                     double r, double g, double b)
{
    if (sim.arc.empty()) return;

    auto toX = [&](double x) { return margin + (x / xMax) * plotW; };
    auto toY = [&](double h) {
        return margin + (1.0 - (h - yLo) / (yHi - yLo)) * plotH;
    };

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
    int width  = gtk_widget_get_allocated_width(widget);
    int height = gtk_widget_get_allocated_height(widget);

    cairo_set_source_rgb(cr, 0.12, 0.12, 0.12);
    cairo_paint(cr);

    if (!last_result.valid) return FALSE;

    const auto& low  = last_result.low.sim;
    const auto& high = last_result.high.sim;

    if (low.arc.empty() && high.arc.empty()) return FALSE;

    double xMax = 1, yLo = -1, yHi = 1;

    auto calc_bounds = [&](const SimResult& sim) {
        if (sim.arc.empty()) return;
        for (auto& p : sim.arc) {
            if (p.height > yHi) yHi = p.height;
            if (p.height < yLo) yLo = p.height;
        }
        if (sim.arc.back().dist > xMax) xMax = sim.arc.back().dist;
    };
    yLo = low.arc.empty() ? high.arc[0].height : low.arc[0].height;
    calc_bounds(low);
    calc_bounds(high);
    yLo = std::min(yLo, low.arc.empty() ? 0 : low.arc.back().height);
    yLo = std::min(yLo, high.arc.empty() ? 0 : high.arc.back().height);
    yLo -= 1.0;
    yHi += 1.0;
    if (yLo > 0) yLo = -1;
    if (xMax < 0.01) return FALSE;

    int margin = 8;
    double plotW = width - margin * 2;
    double plotH = height - margin * 2;

    // Grid
    auto toY = [&](double h) {
        return margin + (1.0 - (h - yLo) / (yHi - yLo)) * plotH;
    };
    cairo_set_source_rgb(cr, 0.2, 0.2, 0.2);
    cairo_set_line_width(cr, 0.5);
    for (int i = 0; i <= 4; ++i) {
        double fy = yLo + (yHi - yLo) * i / 4.0;
        cairo_move_to(cr, margin, toY(fy));
        cairo_line_to(cr, width - margin, toY(fy));
        cairo_stroke(cr);
    }

    // Draw both trajectories
    draw_arc(cr, low, margin, plotW, plotH, yLo, yHi, xMax, 0.0, 0.8, 1.0);
    if (!high.arc.empty())
        draw_arc(cr, high, margin, plotW, plotH, yLo, yHi, xMax, 1.0, 0.6, 0.0);

    // Cannon
    cairo_set_source_rgb(cr, 0.0, 1.0, 0.0);
    cairo_arc(cr, margin, toY(0), 4, 0, 2 * M_PI);
    cairo_fill(cr);

    // Target (use low if available, otherwise high)
    double targetDist = low.arc.empty() ? high.arc.back().dist : low.arc.back().dist;
    double targetH   = low.arc.empty() ? high.arc.back().height : low.arc.back().height;
    auto toX = [&](double x) { return margin + (x / xMax) * plotW; };
    cairo_set_source_rgb(cr, 1.0, 0.2, 0.2);
    cairo_rectangle(cr, toX(targetDist) - 4, toY(targetH) - 4, 8, 8);
    cairo_fill(cr);

    return FALSE;
}
