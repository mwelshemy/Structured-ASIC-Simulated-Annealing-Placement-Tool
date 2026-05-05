import random
import sys

def place_pins(components, occupant_grid, site_grid, nx, ny):
    for cid, comp in components.items():
        if not comp['fixed']:
            continue
        x = comp['x']
        y = comp['y']
        if site_grid[y][x] != 'PERIMETER':
            print("ERROR: pin", cid, "not on perimeter at", x, y)
            sys.exit(1)
        occupant_grid[y][x] = cid

def place_cells(components, occupant_grid, empty_sites):
    for t in empty_sites:
        random.shuffle(empty_sites[t])
    for cid, comp in components.items():
        if comp['fixed']:
            continue
        t = comp['type']
        if len(empty_sites[t]) == 0:
            print("ERROR: no empty sites left for type", t)
            sys.exit(1)
        x, y = empty_sites[t].pop()
        comp['x'] = x
        comp['y'] = y
        occupant_grid[y][x] = cid

def check_placement(components, occupant_grid, site_grid, nx, ny):
    errors = 0
    for cid, comp in components.items():
        if comp['fixed']:
            continue
        x = comp['x']
        y = comp['y']
        if x == 0 or x == nx-1 or y == 0 or y == ny-1:
            print("ERROR: cell", cid, "on perimeter")
            errors += 1
        if site_grid[y][x] != comp['type']:
            print("ERROR: cell", cid, "type", comp['type'], "on", site_grid[y][x])
            errors += 1
    if errors == 0:
        print("placement OK - no errors")
    else:
        print(errors, "errors found")
    return errors == 0
