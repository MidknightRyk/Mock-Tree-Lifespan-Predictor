# Tree Lifespan Predictor

A C program that analyzes tree survival and conflict detection based on environmental factors and spatial relationships.

## Overview

This program simulates tree survival scenarios by analyzing:
- **Water requirements**: Calculates total water consumption across all trees
- **Spatial conflicts**: Detects root system overlaps between trees
- **Stress analysis**: Determines tree mortality based on rainfall patterns
- **Visual mapping**: Generates ASCII grid representations of tree locations

## Features

### Stage 1: Water Analysis
- Calculates total water consumption for all trees
- Outputs results in megaliters for large-scale analysis

### Stage 2: Conflict Detection
- Identifies trees with overlapping root systems
- Analyzes spatial relationships based on root radius and trunk distance

### Stage 3: Spatial Visualization
- Generates ASCII grid showing tree positions
- Visual representation of the ecosystem layout

### Stage 4: Survival Prediction
- Analyzes tree stress based on annual rainfall
- Predicts order of tree mortality under water stress
- Updates visualization to show surviving trees

## Input Format

The program expects tab-separated values (TSV) with the following columns:
```
tree    xloc    yloc    liters    rootrad
```

Where:
- `tree`: Single character tree identifier
- `xloc`: X-coordinate position (0-70 range)
- `yloc`: Y-coordinate position (0-60 range) 
- `liters`: Annual water requirement in liters
- `rootrad`: Root system radius in meters

## Usage

```bash
gcc -o tree_predictor tree_lifespan_predictor.c -lm
./tree_predictor <annual_rainfall> < input_file.tsv
```

Example:
```bash
./tree_predictor 800.0 < test0.tsv
```

## Sample Files

- `test0.tsv`: Basic 3-tree scenario
- `test1.tsv`: Additional test case
- `test2.tsv`: Complex scenario

## Algorithm Details

**Conflict Detection**: Trees conflict when their root systems overlap, calculated using:
- Distance between tree trunks
- Individual root radii
- Overlap threshold analysis

**Stress Calculation**: Tree mortality occurs when:
- Required rainfall exceeds available rainfall
- Stress factor > 1.0 indicates tree death
- Trees die in order of highest stress first

## Output

The program provides structured output for each analysis stage:
1. Water consumption summary
2. Conflict relationship matrix  
3. ASCII spatial grid
4. Survival analysis with updated grid

## Author

Originally developed as Assignment 2 for COMP20005, Semester 1, 2018