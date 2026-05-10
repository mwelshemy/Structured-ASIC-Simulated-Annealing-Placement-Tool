#include "structures.h"
#include <algorithm>
#include <random>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

void initialPlacement() {
    mt19937 rng(42); 

    auto t0_avail = siteTracker.t0_sites; auto t1_avail = siteTracker.t1_sites;
    auto t2_avail = siteTracker.t2_sites; auto t3_avail = siteTracker.t3_sites;

    shuffle(t0_avail.begin(), t0_avail.end(), rng); shuffle(t1_avail.begin(), t1_avail.end(), rng);
    shuffle(t2_avail.begin(), t2_avail.end(), rng); shuffle(t3_avail.begin(), t3_avail.end(), rng);

    int t0_idx = 0, t1_idx = 0, t2_idx = 0, t3_idx = 0;

    for (int i = 0; i < numCells; ++i) {
        if (cells[i].x == -1) { 
            Point p;
            if (cells[i].type == T0) p = t0_avail[t0_idx++];
            else if (cells[i].type == T1) p = t1_avail[t1_idx++];
            else if (cells[i].type == T2) p = t2_avail[t2_idx++];
            else if (cells[i].type == T3) p = t3_avail[t3_idx++];

            cells[i].x = p.x; cells[i].y = p.y;
            grid->cellAt(p.x, p.y) = cells[i].id; 
        }
    }
}

long long simulatedAnnealing(double coolingRate) {
    mt19937 rng(1337); 
    uniform_real_distribution<double> probDist(0.0, 1.0);

    long long currentCost = calculateTotalHPWL();
    long long bestCost = currentCost;

    double T = 500.0 * currentCost; 
    double T_min = (5.0e-5 * currentCost) / static_cast<double>(numNets); 
    int moves_per_temp = 20 * numCells; 

    stringstream filename;
    filename << "output/" << baseDesignName << "_CR" << fixed << setprecision(2) << coolingRate << ".csv";
    ofstream logFile(filename.str());
    logFile << "Temperature,Current_HPWL,Best_HPWL\n";

    vector<int> movable_cells;
    for (int i = 0; i < numCells; ++i) {
        if (cells[i].type != PIN) movable_cells.push_back(i);
    }
    
    if (movable_cells.empty()) return bestCost;
    uniform_int_distribution<int> cellDist(0, movable_cells.size() - 1);

    while (T > T_min) {
        for (int i = 0; i < moves_per_temp; ++i) {
            int cellA_idx = movable_cells[cellDist(rng)];
            Cell& cellA = cells[cellA_idx];

            Point siteB;
            if (cellA.type == T0) siteB = siteTracker.t0_sites[rng() % siteTracker.t0_sites.size()];
            else if (cellA.type == T1) siteB = siteTracker.t1_sites[rng() % siteTracker.t1_sites.size()];
            else if (cellA.type == T2) siteB = siteTracker.t2_sites[rng() % siteTracker.t2_sites.size()];
            else if (cellA.type == T3) siteB = siteTracker.t3_sites[rng() % siteTracker.t3_sites.size()];

            if (cellA.x == siteB.x && cellA.y == siteB.y) continue; 
            int cellB_id = grid->cellAt(siteB.x, siteB.y);

            vector<int> affected_nets = cellA.connected_nets;
            if (cellB_id != -1) {
                for (int netId : cells[cellB_id].connected_nets) {
                    if (find(affected_nets.begin(), affected_nets.end(), netId) == affected_nets.end()) {
                        affected_nets.push_back(netId);
                    }
                }
            }

            long long cost_before = 0;
            for (int netId : affected_nets) cost_before += nets[netId].hpwl;

            int old_xA = cellA.x, old_yA = cellA.y;
            cellA.x = siteB.x; cellA.y = siteB.y;
            grid->cellAt(siteB.x, siteB.y) = cellA.id;

            if (cellB_id != -1) {
                cells[cellB_id].x = old_xA; cells[cellB_id].y = old_yA;
                grid->cellAt(old_xA, old_yA) = cellB_id;
            } else { grid->cellAt(old_xA, old_yA) = -1; }

            long long cost_after = 0;
            for (int netId : affected_nets) {
                int min_x = 999999, max_x = -1, min_y = 999999, max_y = -1;
                for (int cId : nets[netId].connected_cells) {
                    int cx = cells[cId].x; int cy = cells[cId].y;
                    if (cx < min_x) min_x = cx; if (cx > max_x) max_x = cx;
                    if (cy < min_y) min_y = cy; if (cy > max_y) max_y = cy;
                }
                cost_after += (max_x - min_x) + (max_y - min_y);
            }

            long long delta_cost = cost_after - cost_before;
            bool accept = false;

            if (delta_cost < 0) accept = true; 
            else if (probDist(rng) < exp(-delta_cost / T)) accept = true; 

            if (accept) {
                currentCost += delta_cost;
                if (currentCost < bestCost) bestCost = currentCost;
                for (int netId : affected_nets) calculateNetHPWL(netId); 
            } else {
                cellA.x = old_xA; cellA.y = old_yA;
                grid->cellAt(old_xA, old_yA) = cellA.id;
                if (cellB_id != -1) {
                    cells[cellB_id].x = siteB.x; cells[cellB_id].y = siteB.y;
                    grid->cellAt(siteB.x, siteB.y) = cellB_id;
                } else { grid->cellAt(siteB.x, siteB.y) = -1; }
            }
        }
        logFile << T << "," << currentCost << "," << bestCost << "\n";
        T *= coolingRate; 
    }
    
    logFile.close(); 
    return bestCost;
}