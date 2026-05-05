import random
from parser import parse_netlist
from grid import build_grid, get_empty_sites
from placement import place_pins, place_cells

DESIGN_FILE = "design_1_small.txt"
random.seed(42)

num_cells, num_nets, ny, nx, num_pins, components, nets = parse_netlist(DESIGN_FILE)
site_grid, occupant_grid = build_grid(ny, nx)
empty_sites = get_empty_sites(site_grid, ny, nx)
place_pins(components, occupant_grid, site_grid, nx, ny)
place_cells(components, occupant_grid, empty_sites)

MOVES_PER_TEMP = 20 * num_cells
COOLING_RATES  = [0.85, 0.90, 0.95, 0.98]
INITIAL_HPWL   = None
