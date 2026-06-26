#pragma once
#include <string>
#include <cmath>
#include <vector>

#ifndef M_PI
static constexpr double M_PI = 3.14159265358979323846;
#endif

static constexpr double TICKS_PER_SEC     = 20.0;
static constexpr double CHARGE_STRENGTH   = 2.0;
static constexpr double BASE_BOUNCE_CHANCE = 0.33;
static constexpr int    MAX_TICKS         = 2000;

struct Vec3 {
    double x = 0, y = 0, z = 0;
    double length() const { return std::sqrt(x*x + y*y + z*z); }
    Vec3 normalized() const {
        double l = length();
        if (l < 1e-12) return {0,0,0};
        return {x/l, y/l, z/l};
    }
    Vec3 operator+(const Vec3& o) const { return {x+o.x, y+o.y, z+o.z}; }
    Vec3 operator-(const Vec3& o) const { return {x-o.x, y-o.y, z-o.z}; }
    Vec3 operator*(double s)      const { return {x*s,   y*s,   z*s};   }
};

struct BallisticProperties {
    std::string name;
    double gravity    = -0.05;
    double drag       = 0.01;
    bool   quadraticDrag = false;
    double durabilityMass = 1.0;
    double penetration    = 1.0;
    double toughness      = 1.0;
    double deflection     = 1.0;
};

struct BarrelMaterial {
    std::string name;
    double minSpread            = 0.025;
    double spreadReductionPerBarrel = 1.4;
    double minVelocityPerBarrel = 1.0;
    double maxSafePropellantStress = 8;
};

struct DimensionProps {
    std::string name;
    double gravityMultiplier = 1.0;
    double dragMultiplier    = 1.0;
};

struct TrajectoryPoint {
    double dist;
    double height;
    double t;
};

struct SimResult {
    std::vector<TrajectoryPoint> arc;
    double flightTimeSec = 0;
    double apexHeight    = 0;
    bool   outOfRange    = false;
};

struct PitchResult {
    double pitch = 0;
    double error = 1e9;
    bool   found = false;
};

struct DualPitch {
    PitchResult low;
    PitchResult high;
};

struct TrajectoryInfo {
    double pitchDeg = 0;
    double flightTimeSec = 0;
    double apexHeight = 0;
    SimResult sim;
    bool valid = false;
};

struct CannonSetup {
    Vec3   cannonPos;
    Vec3   targetPos;
    std::string ammoKey    = "ap_shell";
    std::string barrelKey  = "steel";
    std::string dimKey     = "overworld";
    int    barrelLength    = 4;
    int    numCharges      = 3;
};

struct AmmoRow {
    std::string key;
    std::string name;
    double pitch;
    double flight;
    double apex;
    double spread;
    bool valid;
    std::string error;
};

struct CalcOutput {
    bool   valid           = false;
    std::string errorMsg;
    std::vector<std::string> warnings;

    double yawDeg          = 0;
    double distance3D      = 0;
    double hDist           = 0;
    double heightDelta     = 0;
    double muzzleVelocity  = 0;
    double spread          = 0;

    TrajectoryInfo low;
    TrajectoryInfo high;
    std::vector<AmmoRow> comparison;
};
