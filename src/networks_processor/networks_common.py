import csv
import os
import random
import subprocess
from dataclasses import dataclass
from pathlib import Path
from typing import Optional

from rich import print

from src.cli.folder_and_fnames import GRAPH_FOLDER

from .constants import (
    DELAY_RANGE,
    JITTER_RANGE,
    LOSS_RANGE,
    THROUGHPUT_RANGE,
)


@dataclass(slots=True)
class GraphEdge:
    source: str
    target: str
    throughput: int
    loss: int
    delay: int
    jitter: int

    @classmethod
    def from_source_and_target(cls, source: str, target: str):
        return cls(source, target, *cls._generate_edge_params())

    @staticmethod
    def _generate_edge_params() -> tuple[int, int, int, int]:
        throughput = random.randint(*THROUGHPUT_RANGE)
        loss = random.randint(*LOSS_RANGE)
        delay = random.randint(*DELAY_RANGE)
        jitter = random.randint(*JITTER_RANGE)
        return throughput, loss, delay, jitter

    def to_csv_row(self):
        return [
            self.source,
            self.target,
            self.throughput,
            self.loss,
            self.delay,
            self.jitter,
        ]


def save_graph_to_csv(
    graph: list[GraphEdge],
    graph_name: str,
    output_dir: Path = Path.cwd() / GRAPH_FOLDER,
) -> Optional[tuple[Path, str]]:
    try:
        os.makedirs(output_dir, exist_ok=True)
        output_path = output_dir / f"{graph_name}.csv"
        with output_path.open("w", newline="", encoding="utf-8") as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(
                ["Source", "Target", "Throughput", "Loss", "Delay", "Jitter"]
            )
            writer.writerows([e.to_csv_row() for e in graph])
            return output_path, get_clickable_path(output_path)
    except OSError as e:
        print(f"[bold red]Error saving graph:[/bold red] {e}")
        return None


def get_clickable_path(path: Path) -> str:
    resolved_path = path.resolve()

    if "WSL_DISTRO_NAME" in os.environ:
        try:
            windows_path = subprocess.check_output(
                ["wslpath", "-w", str(resolved_path)], text=True
            )
            return windows_path
        except subprocess.CalledProcessError:
            return str(resolved_path)

    return resolved_path.as_uri()
