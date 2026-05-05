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
    print(num_cells, num_nets, ny, nx, num_pins)
