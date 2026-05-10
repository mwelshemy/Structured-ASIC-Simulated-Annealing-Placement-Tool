#include <iostream>
#include <string>
#include <chrono>
#include <sstream>
#include <iomanip>
#include "structures.h"

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    if (argc < 3) {
        cerr << "Usage: " << argv[0] << " <netlist_file> <cooling_rate>\n";
        return 1;
    }

    string filepath = argv[1];
    size_t slash = filepath.find_last_of("/\\");
    string filename = (slash == string::npos) ? filepath : filepath.substr(slash + 1);
    size_t dot = filename.find_last_of(".");
    baseDesignName = (dot == string::npos) ? filename : filename.substr(0, dot);

    double coolingRate = stod(argv[2]); 
    auto start_time = high_resolution_clock::now();

    parseNetlist(argv[1]);
    cout << "----------------------------------------\n";
    cout << "Successfully parsed " << argv[1] << "\n";
    cout << "Total Cells: " << numCells << " | Total Nets: " << numNets << "\n";

    initialPlacement();
    long long initialCost = calculateTotalHPWL();
    
    stringstream crStream;
    crStream << fixed << setprecision(2) << coolingRate;
    string outPrefix = "output/" + baseDesignName + "_CR" + crStream.str();

    saveGridToFile(outPrefix + "_before.txt");
    saveGridToSVG(outPrefix + "_before.svg");

    cout << "\n[BEFORE] Initial Placement Complete.\n";
    cout << "Initial Total HPWL: " << initialCost << "\n";

    cout << "\n[OPTIMIZING] Starting Simulated Annealing...\n";
    cout << "Cooling Rate: " << coolingRate << "\n";
    
    long long finalCost = simulatedAnnealing(coolingRate);
    
    saveGridToFile(outPrefix + "_after.txt"); 
    saveGridToSVG(outPrefix + "_after.svg"); 

    cout << "\n[AFTER] Optimization Complete.\n";
    cout << "Final Total HPWL: " << finalCost << "\n";
    cout << "Saved: " << outPrefix << "_after.txt / .svg / .csv\n";
    cout << "----------------------------------------\n";

    printGridToConsole();

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    
    cout << "Total Execution Time: " << duration.count() / 1000.0 << " seconds\n";
    cout << "----------------------------------------\n";

    delete grid;
    return 0;
}