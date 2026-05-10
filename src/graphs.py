import matplotlib.pyplot as plt
import csv
import os
import glob

print("Generating Graph 1: Temp vs TWL (CR=0.95) for ALL designs...")
plt.figure(figsize=(12, 6))

files_095 = glob.glob("output/*CR0.95.csv")

for file in files_095:
    temps = []
    best_hpwls = []
    design_name = os.path.basename(file).replace("_CR0.95.csv", "")
    
    with open(file, 'r') as f:
        reader = csv.reader(f)
        next(reader) # Skip header
        for row in reader:
            temps.append(float(row[0])) # Temp
            best_hpwls.append(float(row[2])) # Best HPWL
            
    plt.plot(temps, best_hpwls, label=design_name)

plt.title('Temperature vs. Total Wire Length (CR=0.95)', fontsize=14, fontweight='bold')
plt.xlabel('Temperature')
plt.ylabel('HPWL')
plt.gca().invert_xaxis() # Temperature drops from right to left
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('output/Graph1_Temp_vs_TWL.png')
plt.clf()
print("Saved: output/Graph1_Temp_vs_TWL.png")

print("\nGenerating Graph 2: Cooling Rate vs Final TWL for ALL designs...")
designs = ["design_1_small", "design_2_medium", "design_3_large", "design_4_dense", "design_5_extreme"]
rates = ["0.75", "0.80", "0.85", "0.90", "0.95"]

plt.figure(figsize=(12, 6))

for design in designs:
    final_hpwls = []
    valid_rates = []
    
    for rate in rates:
        file = f"output/{design}_CR{rate}.csv"
        if os.path.exists(file):
            with open(file, 'r') as f:
                lines = f.readlines()
                if len(lines) > 1:
                    last_line = lines[-1].strip().split(',')
                    final_hpwls.append(float(last_line[2]))
                    valid_rates.append(rate)
                    
    if final_hpwls:
        plt.plot(valid_rates, final_hpwls, marker='o', linewidth=2, label=design)

plt.title('Cooling Rate vs Final TWL (All Designs)', fontsize=14, fontweight='bold')
plt.xlabel('Cooling Rate')
plt.ylabel('Final HPWL')
plt.legend()
plt.grid(True, linestyle='--', alpha=0.7)
plt.tight_layout()
plt.savefig('output/Graph2_CR_vs_TWL.png')
print("Saved: output/Graph2_CR_vs_TWL.png")