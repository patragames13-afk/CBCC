# CBC Ballistics

Ballistics calculator for the **Create Big Cannons** Minecraft mod.
Computes pitch/yaw angles, trajectory arc, flight time, spread,
and muzzle velocity for cannon projectiles.

## Features

- **Dual trajectory** — shows both direct-fire and mortar solutions when available
- **Ammo comparison** — compares pitch/flight/apex across all ammo types
- **360° compass** — yaw in degrees clockwise from North
- **Dark GUI** — GTK3 interface with dark theme and trajectory graph
- **Presets** — save/load cannon configurations to file
- **Copy to clipboard** — one-click report export
- **CLI mode** — interactive terminal mode with `--cli`

## Build

Requires: `cmake`, `g++`, `libgtk-3-dev`

```bash
cd cbc_ballistics
cmake -B build
cmake --build build
```

## Run

```bash
./build/cbc_ballistics       # GUI mode
./build/cbc_ballistics --cli # terminal mode
```

## Controls (GUI)

| Field | Description |
|-------|-------------|
| Cannon X/Y/Z | Cannon position in world |
| Target X/Y/Z | Target position |
| Ammo / Barrel / Dim | Ammo type, barrel material, dimension |
| Barrel len | Number of barrel segments |
| Charges | Gunpowder charges loaded |
| Calculate | Run simulation |
| Copy | Copy report to clipboard |
| Save / Load | Save/load preset file |

## Data Tables

See [BALLISTICS_REFERENCE.md](BALLISTICS_REFERENCE.md) for ammo,
barrel, and dimension properties.

## License

MIT
