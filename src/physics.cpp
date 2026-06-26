#include "physics.h"
#include <algorithm>
#include <cmath>
#include <cmath>

SimResult simulate2D(double pitchDeg, double hDist,
                     double speed,
                     const BallisticProperties& ammo,
                     const DimensionProps& dim)
{
    double pRad = pitchDeg * M_PI / 180.0;
    double vx   = std::cos(pRad) * speed;
    double vy   = std::sin(pRad) * speed;
    double x = 0, y = 0;

    SimResult res;
    res.arc.push_back({0.0, 0.0, 0.0});

    for (int t = 0; t < MAX_TICKS; ++t) {
        double vel  = std::sqrt(vx*vx + vy*vy);
        if (vel < 1e-12) break;

        double density = dim.dragMultiplier;
        double drag    = std::min(ammo.drag * density * vel, vel);
        double ax = -vx/vel * drag;
        double ay = -vy/vel * drag + ammo.gravity * dim.gravityMultiplier;

        x += vx + 0.5*ax;
        y += vy + 0.5*ay;
        vx += ax;
        vy += ay;

        double timeSec = (t + 1) / TICKS_PER_SEC;
        res.arc.push_back({x, y, timeSec});
        res.apexHeight = std::max(res.apexHeight, y);

        if (x >= hDist) {
            res.flightTimeSec = timeSec;
            return res;
        }
    }
    res.outOfRange = true;
    return res;
}

PitchResult findPitch(double hDist, double dY, double speed,
                      const BallisticProperties& ammo,
                      const DimensionProps& dim)
{
    double lo = -89.0, hi = 89.0;
    PitchResult best;

    for (int iter = 0; iter < 60; ++iter) {
        double mid  = (lo + hi) / 2.0;
        double pRad = mid * M_PI / 180.0;
        double vx   = std::cos(pRad) * speed;
        double vy   = std::sin(pRad) * speed;
        double x = 0, y = 0, px = 0, py = 0;
        bool reached = false;

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
                double f     = (hDist - px) / (x - px);
                double hitY  = py + (y - py) * f;
                double err   = hitY - dY;

                if (std::abs(err) < std::abs(best.error)) {
                    best.error = err;
                    best.pitch = mid;
                    best.found = true;
                }
                if (err < 0) lo = mid; else hi = mid;
                reached = true;
                break;
            }
        }
        if (!reached) {
            if (mid > 45) hi = mid;
            else          lo = mid;
        }
    }
    return best;
}

TrajectoryInfo simulateTrajectory(double pitchDeg, double hDist,
                                   double speed,
                                   const BallisticProperties& ammo,
                                   const DimensionProps& dim)
{
    TrajectoryInfo ti;
    SimResult s = simulate2D(pitchDeg, hDist, speed, ammo, dim);
    if (!s.outOfRange) {
        ti.valid = true;
        ti.pitchDeg = pitchDeg;
        ti.flightTimeSec = s.flightTimeSec;
        ti.apexHeight = s.apexHeight;
        ti.sim = std::move(s);
    }
    return ti;
}
