# PZSP2 project
This is a repo for PZSP2 project.

## Table of contents
1. [Project Requirements](./documentation/requirements.md)
2. [Branch Structure](./documentation/branch-structure.md)
3. [Source Code Navigation](./src/description.md)

## How to run CLI client?

### Setup:

Create python environment with required libraries:
```bash
python -m venv .venv/
source .venv/bin/activate
pip install -r ./src/requirements.txt
```

Build required C++ programs (make sure you are in project folder)
```bash
cmake -S . -B build && cmake --build build
```

### Usage:

Open project folder and run `run.py` as module:
```bash
cd <project folder>
python -m src.run [subcommand] [subcommand options and flags]
```

For help and additional information run:
```bash
python -m src.run -h
# or for subcommand
python -m src.run [subcommand] -h
```

## Example Use Cases

### Graph Generation

1. Parsing graph from sndlib
```bash
# python -m src.run parse-sndlib network_name
python -m src.run parse-sndlib polska
```

2. Generating fullmesh graph
```bash
# python -m src.run gen-full-mesh node_count
python -m src.run gen-full-mesh 20
```

### Intents Generation

```bash
# python -m src.run gen-intents graph
python -m src.run gen-intents polska.csv
```

### True Pareto Front Approximation Generation

```bash
# python -m src.run gen-true-pareto graph intents iterations runs mutation_probability
python -m src.run gen-true-pareto polska.csv polska_intents.csv 1000 20 0.4
```

### Algorithm Comparison

```bash
# python -m src.run algo-compare graph intents true_pareto_frton iterations runs mutation_probability --plot-data(optiona flag)
python -m src.run algo-compare polska.csv polska_intents.csv polsk_tpf.csv 1000 15 0.4 --plot-data
```

### Computational Complexity Tests

```bash
# python -m src.run c-comp [--load-data FILENAME] [--iterations ITERATIONS] [--runs RUNS] [--mutation MUTATION] [--min_nodes MIN_NODES] [--max_nodes MAX_NODES] [--step STEP]
python -m src.run c-comp --iterations 100 --runs 1 --mutation 0.1 --min_nodes 5 --max_nodes 20 --step 2
```

### Convergence of Metrics

```bash
# python -m src.run check-conv [--plot-data] graph intents true_pareto iterations runs mutation_probability max_population step
python -m src.run check-conv polska.csv polska_intents.csv polsk_tpf.csv 1000 15 0.4 30 10 --plot-data
```
