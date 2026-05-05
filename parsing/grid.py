MASTER_TILE = [
    ['T0', 'T1', 'T0', 'T2', 'T0'],
    ['T1', 'T0', 'T1', 'T0', 'T1'],
    ['T0', 'T2', 'T3', 'T0', 'T2'],
    ['T1', 'T0', 'T1', 'T0', 'T0'],
    ['T0', 'T0', 'T0', 'T0', 'T0'],
]

def get_site_type(x, y, ny, nx):
    if x == 0 or x == nx - 1 or y == 0 or y == ny - 1:
        return 'PERIMETER'
    return MASTER_TILE[(y - 1) % 5][(x - 1) % 5]

def build_grid(ny, nx):
    site_grid = []
    occupant_grid = []
    for y in range(ny):
        row_s = []
        row_o = []
        for x in range(nx):
            row_s.append(get_site_type(x, y, ny, nx))
            row_o.append(None)
        site_grid.append(row_s)
        occupant_grid.append(row_o)
    return site_grid, occupant_grid

def get_empty_sites(site_grid, ny, nx):
    empty = {'T0': [], 'T1': [], 'T2': [], 'T3': []}
    for y in range(1, ny - 1):
        for x in range(1, nx - 1):
            t = site_grid[y][x]
            if t in empty:
                empty[t].append((x, y))
    return empty
