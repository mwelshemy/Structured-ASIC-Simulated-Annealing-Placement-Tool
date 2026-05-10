#include "structures.h"
#include <iostream>
#include <fstream>
#include <sstream>

void initializeSiteTracker() {
    for (int y = 1; y < grid->ny - 1; ++y) {
        for (int x = 1; x < grid->nx - 1; ++x) {
            CellType type = getCoreSiteType(x, y);
            Point p = {x, y};
            if (type == T0) siteTracker.t0_sites.push_back(p);
            else if (type == T1) siteTracker.t1_sites.push_back(p);
            else if (type == T2) siteTracker.t2_sites.push_back(p);
            else if (type == T3) siteTracker.t3_sites.push_back(p);
        }
    }
}

void parseNetlist(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file " << filename << "\n";
        exit(1);
    }

    std::string line;
    std::getline(file, line);
    std::stringstream headerStream(line);
    int nx, ny;
    headerStream >> numCells >> numNets >> ny >> nx >> numFixedPins;
    
    grid = new Fabric(nx, ny);
    cells.resize(numCells);
    nets.resize(numNets);

    initializeSiteTracker();

    for (int i = 0; i < numCells; ++i) {
        std::getline(file, line);
        std::stringstream lineStream(line);
        int id;
        lineStream >> id;
        cells[id].id = id;
        
        if (i < numFixedPins) {
            int x, y; std::string p_str;
            lineStream >> x >> y >> p_str;
            cells[id].type = PIN; cells[id].x = x; cells[id].y = y;
            grid->cellAt(x, y) = id;
        } else {
            std::string typeStr; lineStream >> typeStr;
            cells[id].type = stringToType(typeStr);
            cells[id].x = -1; cells[id].y = -1;
        }
    }

    for (int i = 0; i < numNets; ++i) {
        std::getline(file, line);
        std::stringstream lineStream(line);
        int numAttached; lineStream >> numAttached;
        nets[i].id = i; nets[i].connected_cells.reserve(numAttached);

        for (int j = 0; j < numAttached; ++j) {
            int cellId; lineStream >> cellId;
            nets[i].connected_cells.push_back(cellId);
            cells[cellId].connected_nets.push_back(i);
        }
    }
    file.close();
}