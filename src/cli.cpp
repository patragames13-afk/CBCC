#include "cli.h"
#include "data.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

void printResult(const CannonSetup& s, const CalcOutput& o)
{
    const int W = 34;
    auto row = [&](const std::string& label, const std::string& val) {
        std::cout << "  " << std::left << std::setw(W) << label << val << "\n";
    };

    std::cout << "\n╔══════════════════════════════════════════╗\n";
    std::cout << "║     CBC Ballistics — Trajectory Report  ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n\n";

    if (!o.warnings.empty()) {
        for (const auto& w : o.warnings)
            std::cout << "  [WARN] " << w << "\n";
        std::cout << "\n";
    }

    if (!o.valid) {
        std::cout << "  [ERROR] " << o.errorMsg << "\n\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);

    std::cout << "  ── Aim ──────────────────────────────────\n";
    row("Pitch (low)",            std::to_string(o.low.pitchDeg).substr(0,6) + "°");
    if (o.high.valid)
        row("Pitch (high/mortar)", std::to_string(o.high.pitchDeg).substr(0,6) + "°");
    row("Yaw (CW from N)",        std::to_string(o.yawDeg).substr(0,6)  + "°");

    std::cout << "\n  ── Range ────────────────────────────────\n";
    row("3D distance",            std::to_string(o.distance3D).substr(0,7) + " m");
    row("Horizontal distance",    std::to_string(o.hDist).substr(0,7)      + " m");
    row("Height delta",
        (o.heightDelta >= 0 ? "+" : "") + std::to_string(o.heightDelta).substr(0,6) + " m");

    std::cout << "\n  ── Ballistics ───────────────────────────\n";
    row("Muzzle velocity",
        std::to_string(o.muzzleVelocity).substr(0,5) + " bl/tick  ("
        + std::to_string((int)(o.muzzleVelocity * TICKS_PER_SEC)) + " bl/s)");
    row("Flight time (low)",      std::to_string(o.low.flightTimeSec).substr(0,5) + " s");
    if (o.high.valid)
        row("Flight time (high)", std::to_string(o.high.flightTimeSec).substr(0,5) + " s");
    row("Arc apex (low)",         std::to_string(o.low.apexHeight).substr(0,6)    + " m");
    if (o.high.valid)
        row("Arc apex (high)",   std::to_string(o.high.apexHeight).substr(0,6)   + " m");
    row("Spread (est.)",          std::to_string(o.spread).substr(0,5)        + "°");

    // Ammo comparison
    std::cout << "\n  ── Ammo Comparison ───────────────────────\n";
    for (const auto& r : o.comparison) {
        std::cout << "  " << r.key;
        if (r.valid)
            std::cout << " → pitch " << std::to_string(r.pitch).substr(0,5)
                      << "° flight " << std::to_string(r.flight).substr(0,4) << "s"
                      << " apex " << std::to_string(r.apex).substr(0,4) << "m";
        else
            std::cout << " → " << r.error;
        std::cout << "\n";
    }

    auto print_arc = [&](const SimResult& sim, const std::string& label) {
        if (sim.arc.empty()) return;
        std::cout << "\n  ── " << label << " ───────────\n";
        const int COLS = 60, ROWS = 8;
        std::vector<std::string> grid(ROWS, std::string(COLS, ' '));

        auto& pts = sim.arc;
        double maxH = std::max_element(pts.begin(), pts.end(),
            [](const auto& a, const auto& b){ return a.height < b.height; })->height;
        double minH = std::min_element(pts.begin(), pts.end(),
            [](const auto& a, const auto& b){ return a.height < b.height; })->height;
        double maxH2 = sim.arc.back().height;
        double yLo = std::min(minH, maxH2) - 1.0;
        double yHi = std::max(maxH, 0.0)   + 1.0;
        double xMax = pts.back().dist;

        if (xMax < 0.01) return;

        auto toCol = [&](double x) -> int {
            return (int)std::round((x / xMax) * (COLS - 1));
        };
        auto toRow = [&](double y) -> int {
            int r = (int)std::round((1.0 - (y - yLo) / (yHi - yLo)) * (ROWS - 1));
            return std::max(0, std::min(ROWS - 1, r));
        };

        for (const auto& p : pts) {
            int c = toCol(p.dist), r = toRow(p.height);
            if (c >= 0 && c < COLS && r >= 0 && r < ROWS)
                grid[r][c] = '*';
        }
        grid[toRow(0)][0] = 'C';
        grid[toRow(pts.back().height)][COLS-1] = 'T';

        std::cout << "  +" << std::string(COLS, '-') << "+\n";
        for (int r = 0; r < ROWS; ++r) {
            double yVal = yHi - r * (yHi - yLo) / (ROWS - 1);
            std::cout << std::setw(5) << std::setprecision(0) << yVal
                      << " |" << grid[r] << "|\n";
        }
        std::cout << "  +" << std::string(COLS, '-') << "+\n";
    };

    print_arc(o.low.sim, "Trajectory arc (low)");
    if (o.high.valid)
        print_arc(o.high.sim, "Trajectory arc (high)");
    std::cout << "\n  C = Cannon   T = Target   * = Arc\n\n";
}

void listOptions()
{
    std::cout << "\nAvailable ammo:\n";
    for (const auto& [k,v] : AMMO_TABLE)
        std::cout << "  " << k << " — " << v.name << "\n";
    std::cout << "\nAvailable barrels:\n";
    for (const auto& [k,v] : BARREL_TABLE)
        std::cout << "  " << k << " — " << v.name
                  << " (max charges: " << (int)v.maxSafePropellantStress << ")\n";
    std::cout << "\nAvailable dimensions:\n";
    for (const auto& [k,v] : DIMENSION_TABLE)
        std::cout << "  " << k << " — " << v.name << "\n";
    std::cout << "\n";
}
