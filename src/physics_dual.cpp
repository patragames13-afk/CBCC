#include "physics.h"
#include <cmath>

DualPitch findBothPitches(double hDist, double dY, double speed,
                          const BallisticProperties& ammo,
                          const DimensionProps& dim)
{
    DualPitch dp;

    auto refine = [&](double angle) -> PitchResult {
        PitchResult best;
        for (int iter = 0; iter < 30; ++iter) {
            double pRad = angle * M_PI / 180.0;
            double vx = std::cos(pRad) * speed;
            double vy = std::sin(pRad) * speed;
            double x = 0, y = 0, px = 0, py = 0;

            for (int t = 0; t < MAX_TICKS; ++t) {
                px = x; py = y;
                double vel = std::sqrt(vx*vx + vy*vy);
                if (vel < 1e-12) break;
                double drag = std::min(ammo.drag * dim.dragMultiplier * vel, vel);
                double ax = -vx/vel * drag;
                double ay = -vy/vel * drag + ammo.gravity * dim.gravityMultiplier;
                x += vx + 0.5*ax;
                y += vy + 0.5*ay;
                vx += ax; vy += ay;

                if (x >= hDist) {
                    double f = (hDist - px) / (x - px);
                    double hitY = py + (y - py) * f;
                    double err = hitY - dY;
                    if (std::abs(err) < std::abs(best.error)) {
                        best.error = err;
                        best.pitch = angle;
                        best.found = true;
                    }
                    if (err < 0) angle += 0.5; else angle -= 0.5;
                    break;
                }
            }
        }
        return best;
    };

    PitchResult candidates[91];
    for (int a = 0; a <= 89; ++a) {
        double pRad = a * M_PI / 180.0;
        double vx = std::cos(pRad) * speed;
        double vy = std::sin(pRad) * speed;
        double x = 0, y = 0, px = 0, py = 0;

        for (int t = 0; t < MAX_TICKS; ++t) {
            px = x; py = y;
            double vel = std::sqrt(vx*vx + vy*vy);
            if (vel < 1e-12) break;
            double drag = std::min(ammo.drag * dim.dragMultiplier * vel, vel);
            double ax = -vx/vel * drag;
            double ay = -vy/vel * drag + ammo.gravity * dim.gravityMultiplier;
            x += vx + 0.5*ax;
            y += vy + 0.5*ay;
            vx += ax; vy += ay;

            if (x >= hDist) {
                double f = (hDist - px) / (x - px);
                double hitY = py + (y - py) * f;
                candidates[a].error = hitY - dY;
                candidates[a].pitch = a;
                candidates[a].found = true;
                break;
            }
        }
    }

    int bestLow = -1;
    for (int a = 0; a <= 44; ++a) {
        if (!candidates[a].found) continue;
        if (bestLow < 0 || std::abs(candidates[a].error) < std::abs(candidates[bestLow].error))
            bestLow = a;
    }
    if (bestLow >= 0) dp.low = refine(candidates[bestLow].pitch);

    int bestHigh = -1;
    for (int a = 46; a <= 89; ++a) {
        if (!candidates[a].found) continue;
        if (bestHigh < 0 || std::abs(candidates[a].error) < std::abs(candidates[bestHigh].error))
            bestHigh = a;
    }
    if (bestHigh >= 0) dp.high = refine(candidates[bestHigh].pitch);

    if (std::abs(dp.high.pitch - dp.low.pitch) < 3.0)
        dp.high.found = false;

    return dp;
}
