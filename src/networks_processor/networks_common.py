import os
import csv
import random
from pathlib import Path


GRAPH_FOLDER = "graphs"

# Edge parametr ranges
THROUGHPUT_RANGE=(1, 100)
LOSS_RANGE=(0, 500)
DELAY_RANGE=(1, 200)
JITTER_RANGE=(-100, 100)

def generate_edge_params() -> tuple[int, int, int, int]:
    throughput = random.randint(1, 100)
    loss = random.randint(0, 500)
    delay = random.randint(1, 200)
    jitter = random.randint(-100, 100)
    return throughput, loss, delay, jitter

def save_graph_to_csv(graph: list[tuple[str]], graph_name: str) -> str | None:
    if not graph:
        return None
    
    cwd = Path.cwd()
    graphs_folder_path = cwd / GRAPH_FOLDER
    os.makedirs(graphs_folder_path, exist_ok=True)
    output_path = graphs_folder_path / f"{graph_name}.csv"
    with open(output_path, "w", newline="") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Source", "Target", "Throughput", "Loss", "Delay", "Jitter"])
        writer.writerows(graph)

    return output_path.name