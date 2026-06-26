# CBC Ballistics Reference

## Overview

Ballistics calculator for the **Create Big Cannons** Minecraft mod.
Computes pitch/yaw angles, trajectory, flight time, and spread for
cannon projectiles.

## Physics Model

Semi-implicit Euler integration at 20 ticks/second (1 game tick).

- **Gravity**: applied each tick per `ammo.gravity * dim.gravityMultiplier`
- **Drag**: `drag * density * velocity` (linear); `quadraticDrag` multiplies by `vel` again
- **Density**: `dim.dragMultiplier + fluidDrag`

## Tables

### Ammo

| Key | Name | Gravity | Drag | Quadratic | Durability | Pen | Tough | Deflect |
|-----|------|---------|------|-----------|------------|-----|-------|---------|
| ap_shell | AP Shell | -0.05 | 0.01 | no | 6 | 2.0 | 0.5 | 0.70 |
| he_shell | HE Shell | -0.05 | 0.01 | no | 2 | 1.0 | 1.0 | 0.70 |
| ap_shot | AP Shot | -0.05 | 0.01 | no | 8 | 2.0 | 1.0 | 0.70 |
| shot | Shot (solid) | -0.05 | 0.01 | no | 3 | 1.1 | 2.0 | 0.86 |
| shrapnel | Shrapnel Shell | -0.05 | 0.01 | no | 2 | 1.0 | 1.0 | 0.70 |
| smoke | Smoke Shell | -0.05 | 0.01 | no | 2 | 1.0 | 1.0 | 0.70 |
| drop_mortar | Drop Mortar Shell | -0.05 | 0.01 | no | 2 | 1.0 | 1.0 | 0.70 |
| mortar_stone | Mortar Stone | -0.025 | 0.01 | no | 4 | 0.0 | 0.0 | 0.86 |
| traffic_cone | Traffic Cone | -0.05 | 0.01 | no | 30 | 4.0 | 4.0 | 0.70 |

### Barrels

| Key | Name | Min Spread | Spread Reduct/Barrel | Min Vel/Barrel | Max Charges |
|-----|------|-----------|---------------------|----------------|------------|
| steel | Steel | 0.025 | 1.4 | 1.0 | 8 |
| nethersteel | Nethersteel | 0.02 | 1.15 | 0.667 | 10 |
| bronze | Bronze | 0.03 | 1.4 | 1.333 | 4 |
| cast_iron | Cast Iron | 0.05 | 2.0 | 2.0 | 2 |
| wrought_iron | Wrought Iron | 0.1 | 1.0 | 2.0 | 1 |
| log | Log | 1.5 | 1.0 | -1.0 | 0 |

### Dimensions

| Key | Name | Gravity Mult | Drag Mult |
|-----|------|-------------|-----------|
| overworld | Overworld | 1.0 | 1.0 |
| nether | Nether | 1.1 | 1.1 |
| end | The End | 0.8 | 0.0 |

## Muzzle Velocity

```
velocity = max(charges * 2.0, barrel.minSpread)
```

## Pitch Search

Binary search between -89° and +89° (60 iterations).
For each candidate pitch it simulates the full 2D trajectory
and compares hit height vs target height.

## Yaw

```
yaw = atan2(dx, dz)   // clockwise from South (+Z)
```

## Spread

```
spread = max(charges * 2.0 - barrelSpreadReduction * barrelLength, minSpread)
```

## Output Fields

- **Pitch**: cannon elevation angle (degrees, up = +)
- **Yaw**: horizontal angle clockwise from South (Minecraft convention)
- **3D distance**: straight-line distance cannon → target
- **Horizontal distance**: XZ-plane distance
- **Height delta**: target Y - cannon Y
- **Muzzle velocity**: initial projectile speed in blocks/tick (and bl/s)
- **Flight time**: simulated time to reach target XZ
- **Arc apex**: peak height above cannon
- **Spread**: estimated random spread angle

## Warnings

- **No propellant**: `numCharges <= 0` — no gunpowder in cannon
- **Muzzle velocity too low**: `muzzleVelocity < 1.0` — projectile barely moves
- **Squib risk**: charge velocity too low for barrel length; suggests how many more charges to add
- **Barrel stress overload**: charges exceed barrel's `maxSafePropellantStress`
