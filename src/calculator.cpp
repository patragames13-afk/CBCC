#include "calculator.h"
#include "data.h"
#include "physics.h"
#include <algorithm>
#include <cmath>

static void fill_common(CalcOutput& out, const CannonSetup& s,
                         const BarrelMaterial& barrel)
{
    double chargesUsed = s.numCharges * CHARGE_STRENGTH;
    out.muzzleVelocity = std::max(chargesUsed, barrel.minSpread);
    out.spread = std::max(
        s.numCharges * 2.0 - barrel.spreadReductionPerBarrel * s.barrelLength,
        barrel.minSpread);
}

static void add_warnings(CalcOutput& out, const CannonSetup& s,
                          const BarrelMaterial& barrel)
{
    double chargesUsed = s.numCharges * CHARGE_STRENGTH;

    if (s.numCharges <= 0)
        out.warnings.push_back("No propellant! Add gunpowder charges.");
    else if (out.muzzleVelocity < 1.0)
        out.warnings.push_back("Muzzle velocity too low.");

    double minVelNeeded = barrel.minVelocityPerBarrel * s.barrelLength;
    if (s.numCharges > 0 && chargesUsed < minVelNeeded)
        out.warnings.push_back("Squib risk: not enough propellant. Add "
            + std::to_string((int)std::ceil(minVelNeeded / CHARGE_STRENGTH) - s.numCharges)
            + " more charge(s).");
    if (s.numCharges > barrel.maxSafePropellantStress)
        out.warnings.push_back("Barrel stress overload! Max safe charges for "
            + barrel.name + ": " + std::to_string((int)barrel.maxSafePropellantStress));
}

CalcOutput calculate(const CannonSetup& s)
{
    CalcOutput out;

    auto ammoIt   = AMMO_TABLE.find(s.ammoKey);
    auto barrelIt = BARREL_TABLE.find(s.barrelKey);
    auto dimIt    = DIMENSION_TABLE.find(s.dimKey);

    if (ammoIt   == AMMO_TABLE.end())   { out.errorMsg = "Unknown ammo: "   + s.ammoKey;   return out; }
    if (barrelIt == BARREL_TABLE.end()) { out.errorMsg = "Unknown barrel: " + s.barrelKey; return out; }
    if (dimIt    == DIMENSION_TABLE.end()){ out.errorMsg = "Unknown dim: "  + s.dimKey;    return out; }

    const auto& ammo   = ammoIt->second;
    const auto& barrel = barrelIt->second;
    const auto& dim    = dimIt->second;

    double dx = s.targetPos.x - s.cannonPos.x;
    double dy = s.targetPos.y - s.cannonPos.y;
    double dz = s.targetPos.z - s.cannonPos.z;
    out.hDist      = std::sqrt(dx*dx + dz*dz);
    out.distance3D = std::sqrt(dx*dx + dy*dy + dz*dz);
    out.heightDelta = dy;

    if (out.hDist < 0.01) {
        out.errorMsg = "Cannon and target share the same XZ position.";
        return out;
    }

    fill_common(out, s, barrel);
    add_warnings(out, s, barrel);

    double yawRad = std::atan2(dx, -dz);
    out.yawDeg = std::fmod(yawRad * 180.0 / M_PI + 360.0, 360.0);

    DualPitch dp = findBothPitches(out.hDist, dy, out.muzzleVelocity, ammo, dim);

    if (!dp.low.found || std::abs(dp.low.error) > 5.0) {
        out.errorMsg = "Target out of range for this configuration.";
        return out;
    }

    out.low = simulateTrajectory(dp.low.pitch, out.hDist, out.muzzleVelocity, ammo, dim);
    if (dp.high.found && std::abs(dp.high.error) <= 5.0)
        out.high = simulateTrajectory(dp.high.pitch, out.hDist, out.muzzleVelocity, ammo, dim);

    out.valid = true;

    // Ammo comparison
    for (const auto& [key, a] : AMMO_TABLE) {
        AmmoRow row;
        row.key = key;
        row.name = a.name;
        DualPitch adp = findBothPitches(out.hDist, dy, out.muzzleVelocity, a, dim);
        if (!adp.low.found || std::abs(adp.low.error) > 5.0) {
            row.valid = false;
            row.error = "out of range";
        } else {
            row.valid = true;
            row.pitch = adp.low.pitch;
            SimResult sr = simulate2D(adp.low.pitch, out.hDist, out.muzzleVelocity, a, dim);
            row.flight = sr.flightTimeSec;
            row.apex = sr.apexHeight;
            row.spread = out.spread;
        }
        out.comparison.push_back(row);
    }

    return out;
}
