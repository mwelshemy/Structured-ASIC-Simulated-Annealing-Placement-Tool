import math
import random

grid_components = {}
grid_nets = {}

def calc_net_hpwl(net_id):
    x_coords = []
    y_coords = []
    for comp_id in grid_nets[net_id]:
        x_coords.append(grid_components[comp_id]['x'])
        y_coords.append(grid_components[comp_id]['y'])
    return (max(x_coords) - min(x_coords)) + (max(y_coords) - min(y_coords))

def get_total_wire_length():
    total = 0
    for net_id in grid_nets:
        total += calc_net_hpwl(net_id)
    return total

def can_swap(comp_a, comp_b):
    if grid_components[comp_a]['is_fixed'] or grid_components[comp_b]['is_fixed']:
        return False
    if grid_components[comp_a]['type'] != grid_components[comp_b]['type']:
        return False
    return True

def swap_cells(comp_a, comp_b):
    temp_x = grid_components[comp_a]['x']
    grid_components[comp_a]['x'] = grid_components[comp_b]['x']
    grid_components[comp_b]['x'] = temp_x
    
    temp_y = grid_components[comp_a]['y']
    grid_components[comp_a]['y'] = grid_components[comp_b]['y']
    grid_components[comp_b]['y'] = temp_y

def run_simulated_annealing(cooling_rate, components_data, nets_data):

    global grid_components, grid_nets
    grid_components = components_data
    grid_nets = nets_data
    
    print(f"Starting Simulated Annealing (CR = {cooling_rate}):")
    
    current_cost = get_total_wire_length()
    num_nets = len(grid_nets)
    num_cells = len(grid_components)
    
    current_temp = 500 * current_cost
    final_temp = 500000 * (current_cost / num_nets)
    moves_per_temp = 20 * num_cells
    
    print(f"Initial TWL: {current_cost}")
    history = []
    
    while current_temp > final_temp:
        history.append({"temp": current_temp, "twl": current_cost})
        
        for _ in range(moves_per_temp):
            comp_a, comp_b = random.sample(list(grid_components.keys()), 2)
            if can_swap(comp_a, comp_b):
                swap_cells(comp_a, comp_b)
                new_cost = get_total_wire_length()
                cost_diff = current_cost - new_cost
                
                if cost_diff > 0:
                    current_cost = new_cost
                else:
                    probability = math.exp(cost_diff / current_temp)
                    if random.random() < probability:
                        current_cost = new_cost
                    else:
                        swap_cells(comp_a, comp_b)
                        
        current_temp = current_temp * cooling_rate
        
    print(f"Final TWL: {current_cost}")
    return history, grid_components

# --- TEST BLOCK ---
if __name__ == "__main__":
    test_components = {
        0: {'type': 'P',  'x': 0, 'y': 0, 'is_fixed': True},
        1: {'type': 'P',  'x': 2, 'y': 0, 'is_fixed': True},
        2: {'type': 'T0', 'x': 1, 'y': 1, 'is_fixed': False},
        3: {'type': 'T1', 'x': 2, 'y': 1, 'is_fixed': False},
        4: {'type': 'T3', 'x': 3, 'y': 2, 'is_fixed': False}
    }

    test_nets = {
        1: [0, 2, 3],
        2: [1, 3, 4]
    }
    
    run_simulated_annealing(cooling_rate=0.85, components_data=test_components, nets_data=test_nets)
    