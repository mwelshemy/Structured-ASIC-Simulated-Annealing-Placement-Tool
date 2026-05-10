# Structured ASIC Placement via Simulated Annealing

This project implements a high-performance placement tool for a Structured ASIC fabric using the Simulated Annealing algorithm. It successfully places netlists of varying complexities (up to 1200 cells) onto a rigid 52x52 grid adhering to a specific 5x5 Master Tile site constraint. 

By utilizing an **O(1) Delta-Cost Calculation** during the annealing swap phase, the engine achieves massive performance gains, placing the Extreme design in under 10 seconds.

## Repository Structure
```text
project_repo/
├── src/
│   ├── main.cpp         # Main entry point and orchestration
│   ├── structures.h     # Structs, enums, and global definitions
│   ├── globals.cpp      # HPWL calculations, grid printing, and SVG generation
│   └── optimizer.cpp    # Initial placement and Simulated Annealing logic
│   └── graphs.py        # Python script to automatically generate required rubric graphs
├── designs/             # Provided benchmark netlist files (Small to Extreme)
├── output/              # Generated files (CSV logs, Grid TXT, Visual SVGs, Graph PNGs)
└── README.md            # This documentation file
```

## Prerequisites
* **C++ Compiler:** GCC supporting C++11 or higher.
* **Python 3:** Required only for generating the visualization graphs.
    * Requires `matplotlib` (`pip install matplotlib`)

---

## 1. Compilation
To compile the placement engine, open your terminal in the root directory and run:

```bash
g++ -std=c++11 -O3 src/main.cpp src/globals.cpp src/parser.cpp src/optimizer.cpp -o project
```
*(Note: The `-O3` flag is highly recommended to enable compiler optimizations for maximum execution speed).*

---

## 2. Manual Execution
To run the tool on a single design, pass the netlist file path and the desired cooling rate as arguments:

```bash
.\project.exe designs\design_1_small.txt 0.95
```

**Upon completion, the tool will:**
1. Print the required 52x52 grid representation to the console (P for Pins, 0-3 for Core Cells, . for Empty).
2. Print the Initial HPWL, Final HPWL, and Total Execution Time.
3. Save the raw text grid, the data log (.csv), and the Graphical Representation (.svg) to the `output/` directory.

---

## 3. Automated Execution (All Designs)
To automatically test the engine against all benchmark designs across all required cooling rates (0.75 to 0.95), run the following script in **PowerShell**:

```powershell
$designs = @("design_1_small.txt", "design_2_medium.txt", "design_3_large.txt", "design_4_dense.txt", "design_5_extreme.txt")
$rates = @("0.75", "0.80", "0.85", "0.90", "0.95")

foreach ($design in $designs) {
    foreach ($rate in $rates) {
        Write-Host "Running $design with Cooling Rate $rate..."
        .\project.exe "designs\$design" $rate
    }
}
```
This will populate the `output/` folder with all necessary CSV data logs needed for graph generation.

---

## 4. Graph Generation (20% Rubric Requirement)
Once the CSV data logs are generated in the `output/` folder, run the included Python script to automatically generate the two required report graphs:

```bash
python src/graphs.py
```
This script will read the CSV data and output two image files into the `output/` directory:
1. `Graph1_Temp_vs_TWL.png`: Plots the cooling curve (Temperature vs HPWL) for all designs at CR=0.95.
2. `Graph2_CR_vs_TWL.png`: Plots the Final HPWL across cooling rates (0.75 - 0.95) for all designs.

---

## 5. Visual Output (5% Bonus Requirement)
The C++ code natively generates an SVG graphical visualization of the placement progress. 
After running the executable, open the generated `output/<design_name>_CR<rate>_after.svg` file in any modern web browser to view the color-coded 52x52 physical layout of the ASIC fabric.