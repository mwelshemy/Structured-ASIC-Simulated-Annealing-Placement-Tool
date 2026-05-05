import sys

def parse_netlist(filename):
    f = open(filename, 'r')
    lines = f.readlines()
    f.close()
    lines = [l.strip() for l in lines if l.strip()]

    first = lines[0].split()
    num_cells = int(first[0])
    num_nets  = int(first[1])
    ny        = int(first[2])
    nx        = int(first[3])
    num_pins  = int(first[4])

    components = {}
    for i in range(1, num_cells + 1):
        parts = lines[i].split()
        cid = int(parts[0])
        if parts[-1] == 'P':
            components[cid] = {
                'type': 'P',
                'x': int(parts[1]),
                'y': int(parts[2]),
                'fixed': True
            }
        else:
            components[cid] = {
                'type': parts[1],
                'x': None,
                'y': None,
                'fixed': False
            }

    return num_cells, num_nets, ny, nx, num_pins, components, []
