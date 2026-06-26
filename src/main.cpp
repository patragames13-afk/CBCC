#include "calculator.h"
#include "cli.h"
#include "gui.h"
#include <iostream>
#include <string>

static double promptDouble(const std::string& label, double def)
{
    std::cout << "  " << label << " [" << def << "]: ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return def;
    try { return std::stod(line); } catch (...) { return def; }
}

static int promptInt(const std::string& label, int def)
{
    std::cout << "  " << label << " [" << def << "]: ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return def;
    try { return std::stoi(line); } catch (...) { return def; }
}

static std::string promptStr(const std::string& label, const std::string& def)
{
    std::cout << "  " << label << " [" << def << "]: ";
    std::string line;
    std::getline(std::cin, line);
    if (line.empty()) return def;
    return line;
}

static int runCli()
{
    std::cout << "╔══════════════════════════════════════════╗\n";
    std::cout << "║   Create Big Cannons — Ballistics Calc  ║\n";
    std::cout << "╚══════════════════════════════════════════╝\n";
    listOptions();

    CannonSetup s;

    std::cout << "Cannon position:\n";
    s.cannonPos.x = promptDouble("  X", 0);
    s.cannonPos.y = promptDouble("  Y", 64);
    s.cannonPos.z = promptDouble("  Z", 0);

    std::cout << "Target position:\n";
    s.targetPos.x = promptDouble("  X", 150);
    s.targetPos.y = promptDouble("  Y", 64);
    s.targetPos.z = promptDouble("  Z", 100);

    s.ammoKey    = promptStr("Ammo key",     "ap_shell");
    s.barrelKey  = promptStr("Barrel key",   "steel");
    s.dimKey     = promptStr("Dimension",    "overworld");
    s.barrelLength = promptInt("Barrel length (blocks)", 4);
    s.numCharges   = promptInt("Number of charges",      3);

    CalcOutput result = calculate(s);
    printResult(s, result);
    return result.valid ? 0 : 1;
}

int main(int argc, char* argv[])
{
    for (int i = 1; i < argc; ++i) {
        if (std::string(argv[i]) == "--cli" || std::string(argv[i]) == "-c")
            return runCli();
    }
    return runGui(argc, argv);
}
