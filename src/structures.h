#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>
#include <string>

// Enums and Structs
enum CellType { T0 = 0, T1 = 1, T2 = 2, T3 = 3, PIN = 4, EMPTY = -1 };

struct Point { int x; int y; };

struct Cell {
    int id;
    CellType type;
    int x;
    int y;
    std::vector<int> connected_nets;
};

struct Net {
    int id;
    std::vector<int> connected_cells;
    int min_x, max_x, min_y, max_y;
    int hpwl;
};

struct Fabric {
    int nx, ny;
    std::vector<int> grid;

    Fabric(int width, int height) : nx(width), ny(height) {
        grid.resize(nx * ny, -1);
    }
    inline int& cellAt(int x, int y) { return grid[y * nx + x]; }
};

struct SiteTracker {
    std::vector<Point> t0_sites;
    std::vector<Point> t1_sites;
    std::vector<Point> t2_sites;
    std::vector<Point> t3_sites;
};

// Global Variables
extern Fabric* grid;
extern SiteTracker siteTracker;
extern int numCells, numNets, numFixedPins;
extern std::vector<Cell> cells;
extern std::vector<Net> nets;
extern const CellType masterTile[5][5];
extern std::string baseDesignName;

// Core Function Prototypes
CellType stringToType(const std::string& typeStr);
CellType getCoreSiteType(int x, int y);
void calculateNetHPWL(int netId);
long long calculateTotalHPWL();
void saveGridToFile(const std::string& filename);
void printGridToConsole();
void saveGridToSVG(const std::string& filename);

void parseNetlist(const std::string& filename);
void initialPlacement();
long long simulatedAnnealing(double coolingRate);

#endif