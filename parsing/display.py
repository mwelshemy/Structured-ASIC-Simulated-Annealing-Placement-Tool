import sys

def print_grid(occupant_grid, components, ny, nx):
    print("\n--- Grid ---")
    for y in range(ny):
        row = ""
        for x in range(nx):
            cid = occupant_grid[y][x]
            if cid is None:
                row += "."
            elif components[cid]['fixed']:
                row += "P"
            else:
                row += components[cid]['type'][1]
        print(row)
    print()

def print_stats(components, nets, ny, nx):
    counts = {'T0': 0, 'T1': 0, 'T2': 0, 'T3': 0, 'P': 0}
    for comp in components.values():
        counts[comp['type']] += 1
    print("\n--- Stats ---")
    print("grid:", ny, "x", nx)
    print("pins:", counts['P'])
    print("T0 cells:", counts['T0'])
    print("T1 cells:", counts['T1'])
    print("T2 cells:", counts['T2'])
    print("T3 cells:", counts['T3'])
    print("nets:", len(nets))
    sizes = [len(n) for n in nets]
    if sizes:
        print("avg net size:", round(sum(sizes)/len(sizes), 2))
    print()
