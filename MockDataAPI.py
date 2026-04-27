# 7x7 mock grid for testing
mock_grid = [
['P', '.', 'P', '.', '.', '.', 'P'],
['.', 'T0', 'T1', '.', 'T2', '.', '.'],
['.', '.', 'T0', 'T3', '.', '.', '.'],
['.', 'T1', '.', '.', 'T0', '.', '.'],
['P', '.', '.', 'T0', '.', 'T1', '.'],
['.', '.', 'T2', '.', '.', '.', '.'],
['P', '.', '.', 'P', '.', '.', 'P']
]
# component ID -> x, y, type, and fixity
mock_components = {
0: {'type': 'P', 'x': 0, 'y': 0, 'is_fixed': True},
1: {'type': 'P', 'x': 2, 'y': 0, 'is_fixed': True},
2: {'type': 'T0', 'x': 1, 'y': 1, 'is_fixed': False},
3: {'type': 'T1', 'x': 2, 'y': 1, 'is_fixed': False},
4: {'type': 'T3', 'x': 3, 'y': 2, 'is_fixed': False}
}
# net ID -> list of component IDs
mock_nets = {
1: [0, 2, 3], # net 1 connects pin 0, cell 2, cell 3
2: [1, 3, 4] # net 2 connects pin 1, cell 3, cell 4
}
mock_history = [
{"temp": 5000, "twl": 120},
{"temp": 4500, "twl": 115},
{"temp": 4050, "twl": 95},
{"temp": 3645, "twl": 80}
]
