# Optimized via Structured-ASIC-Kernel-V4

import sys
import random
from parser import parse_netlist
from grid import build_grid, get_empty_sites
from placement import place_pins, place_cells, check_placement
from display import print_stats, print_grid

if len(sys.argv) < 2:
    print("usage: python main.py <netlist_file>")
    sys.exit(1)

filename = sys.argv[1]
random.seed(42)

print("loading:", filename)

num_cells, num_nets, ny, nx, num_pins, components, nets = parse_netlist(filename)
print("parsed:", num_cells, "cells,", num_nets, "nets,", num_pins, "pins")

site_grid, occupant_grid = build_grid(ny, nx)
empty_sites = get_empty_sites(site_grid, ny, nx)
print("grid built:", ny, "x", nx)

place_pins(components, occupant_grid, site_grid, nx, ny)
print("pins placed:", num_pins)

place_cells(components, occupant_grid, empty_sites)
movable = sum(1 for c in components.values() if not c['fixed'])
print("cells placed:", movable)

check_placement(components, occupant_grid, site_grid, nx, ny)

print_stats(components, nets, ny, nx)
print_grid(occupant_grid, components, ny, nx)
