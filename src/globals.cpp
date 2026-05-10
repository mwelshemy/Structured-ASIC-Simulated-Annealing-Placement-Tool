#include "structures.h"
#include <fstream>
#include <iomanip>
#include <iostream>

// Actual definitions of the global variables
Fabric* grid = nullptr;
SiteTracker siteTracker;
int numCells = 0, numNets = 0, numFixedPins = 0;
std::vector<Cell> cells;
std::vector<Net> nets;
std::string baseDesignName = "";

const CellType masterTile[5][5] = {
    {T0, T1, T0, T2, T0},
    {T1, T0, T1, T0, T1},
    {T0, T2, T3, T0, T2},
    {T1, T0, T1, T0, T0},
    {T0, T0, T0, T0, T0}
};

CellType stringToType(const std::string& typeStr) {
    if (typeStr == "T0") return T0;
    if (typeStr == "T1") return T1;
    if (typeStr == "T2") return T2;
    if (typeStr == "T3") return T3;
    return EMPTY;
}

CellType getCoreSiteType(int x, int y) {
    return masterTile[(y - 1) % 5][(x - 1) % 5];
}

void calculateNetHPWL(int netId) {
    Net& net = nets[netId];
    if (net.connected_cells.empty()) { net.hpwl = 0; return; }

    int min_x = 999999, max_x = -1, min_y = 999999, max_y = -1;
    for (int cellId : net.connected_cells) {
        int cx = cells[cellId].x; int cy = cells[cellId].y;
        if (cx < min_x) min_x = cx; if (cx > max_x) max_x = cx;
        if (cy < min_y) min_y = cy; if (cy > max_y) max_y = cy;
    }
    net.min_x = min_x; net.max_x = max_x;
    net.min_y = min_y; net.max_y = max_y;
    net.hpwl = (max_x - min_x) + (max_y - min_y);
}

long long calculateTotalHPWL() {
    long long total = 0;
    for (int i = 0; i < numNets; ++i) {
        calculateNetHPWL(i);
        total += nets[i].hpwl;
    }
    return total;
}

void saveGridToFile(const std::string& filename) {
    std::ofstream outFile(filename);
    for (int y = 0; y < grid->ny; ++y) {
        for (int x = 0; x < grid->nx; ++x) {
            int cellId = grid->cellAt(x, y);
            if (cellId == -1) outFile << std::setw(4) << "."; 
            else outFile << std::setw(4) << cellId;           
        }
        outFile << "\n";
    }
    outFile.close();
}

void printGridToConsole() {
    std::cout << "\n--- Final Grid Representation ---\n";
    for (int y = 0; y < grid->ny; ++y) {
        for (int x = 0; x < grid->nx; ++x) {
            int cellId = grid->cellAt(x, y);
            if (cellId == -1) {
                std::cout << ". ";
            } else {
                CellType type = cells[cellId].type;
                if (type == PIN) std::cout << "P ";
                else if (type == T0) std::cout << "0 ";
                else if (type == T1) std::cout << "1 ";
                else if (type == T2) std::cout << "2 ";
                else if (type == T3) std::cout << "3 ";
            }
        }
        std::cout << "\n";
    }
    std::cout << "---------------------------------\n";
}

void saveGridToSVG(const std::string& filename) {
    std::ofstream svg(filename);
    if (!svg.is_open()) return;

    int cellSize = 15; 
    int width = grid->nx * cellSize;
    int height = grid->ny * cellSize;

    svg << "<svg width=\"" << width << "\" height=\"" << height << "\" xmlns=\"http://www.w3.org/2000/svg\">\n";
    svg << "<rect width=\"100%\" height=\"100%\" fill=\"#ffffff\"/>\n";

    for (int y = 0; y < grid->ny; ++y) {
        for (int x = 0; x < grid->nx; ++x) {
            int cellId = grid->cellAt(x, y);
            std::string color = "#FFFFFF"; 

            if (cellId != -1) {
                CellType type = cells[cellId].type;
                if (type == PIN) color = "#333333";       
                else if (type == T0) color = "#A9D0F5";   
                else if (type == T1) color = "#A9F5A9";   
                else if (type == T2) color = "#F5D0A9";   
                else if (type == T3) color = "#D0A9F5";   
            }

            svg << "<rect x=\"" << (x * cellSize) << "\" y=\"" << (y * cellSize) 
                << "\" width=\"" << cellSize << "\" height=\"" << cellSize 
                << "\" fill=\"" << color << "\" stroke=\"#CCCCCC\" stroke-width=\"1\" />\n";
        }
    }
    svg << "</svg>\n";
    svg.close();
}