#include "data.h"

const std::map<std::string, BallisticProperties> AMMO_TABLE = {
    {"ap_shell",    {"AP Shell",          -0.05, 0.01, false, 6.0, 2.0, 0.5, 0.70}},
    {"he_shell",    {"HE Shell",          -0.05, 0.01, false, 2.0, 1.0, 1.0, 0.70}},
    {"ap_shot",     {"AP Shot",           -0.05, 0.01, false, 8.0, 2.0, 1.0, 0.70}},
    {"shot",        {"Shot (solid)",      -0.05, 0.01, false, 3.0, 1.1, 2.0, 0.86}},
    {"shrapnel",    {"Shrapnel Shell",    -0.05, 0.01, false, 2.0, 1.0, 1.0, 0.70}},
    {"smoke",       {"Smoke Shell",       -0.05, 0.01, false, 2.0, 1.0, 1.0, 0.70}},
    {"drop_mortar", {"Drop Mortar Shell", -0.05, 0.01, false, 2.0, 1.0, 1.0, 0.70}},
    {"mortar_stone",{"Mortar Stone",      -0.025,0.01, false, 4.0, 0.0, 0.0, 0.86}},
    {"traffic_cone",{"Traffic Cone",      -0.05, 0.01, false,30.0, 4.0, 4.0, 0.70}},
};

const std::map<std::string, BarrelMaterial> BARREL_TABLE = {
    {"steel",        {"Steel",        0.025, 1.4,   1.0,   8}},
    {"nethersteel",  {"Nethersteel",  0.02,  1.15,  0.667, 10}},
    {"bronze",       {"Bronze",       0.03,  1.4,   1.333, 4}},
    {"cast_iron",    {"Cast Iron",    0.05,  2.0,   2.0,   2}},
    {"wrought_iron", {"Wrought Iron", 0.1,   1.0,   2.0,   1}},
    {"log",          {"Log",          1.5,   1.0,  -1.0,   0}},
};

const std::map<std::string, DimensionProps> DIMENSION_TABLE = {
    {"overworld", {"Overworld", 1.0, 1.0}},
    {"nether",    {"Nether",    1.1, 1.1}},
    {"end",       {"The End",   0.8, 0.0}},
};
