#pragma once
#include "types.h"

SimResult simulate2D(double pitchDeg, double hDist,
                     double speed,
                     const BallisticProperties& ammo,
                     const DimensionProps& dim);

PitchResult findPitch(double hDist, double dY, double speed,
                      const BallisticProperties& ammo,
                      const DimensionProps& dim);

DualPitch findBothPitches(double hDist, double dY, double speed,
                          const BallisticProperties& ammo,
                          const DimensionProps& dim);

TrajectoryInfo simulateTrajectory(double pitchDeg, double hDist,
                                   double speed,
                                   const BallisticProperties& ammo,
                                   const DimensionProps& dim);
