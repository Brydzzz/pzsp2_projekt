import os
import subprocess
from pathlib import Path

from rich import print

from src.experiments_processing.processing_common import (
    executable_exists,
    print_config_table,
)

from .folder_and_fnames import GRAPH_FOLDER, INTENTS_FOLDER, generate_intents_fname


def run_gen_intents(
    graph_fname: str, intents_fname: str | None = None, should_print_config: bool = True
):
    bin_path = "./build/gen_intents"
    if not executable_exists(bin_path):
        print(
            "[bold red]Error: c++ program `gen_intents` hasn't been built.[/bold red]"
        )
        print(
            f"Please build it first and make sure it is placed in {Path(bin_path).resolve()}"
        )
        return
    print("[bold yellow]Generating intents...[/bold yellow]")
    if should_print_config:
        print_config_table({"Graph": graph_fname})

    output_dir = Path.cwd() / INTENTS_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    fname = generate_intents_fname(graph_fname) if not intents_fname else intents_fname
    output_path = output_dir / fname
    abs_output_path = output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()

    result = subprocess.run(
        [
            bin_path,
            abs_graph_path,
            abs_output_path,
        ]
    )
    if result.returncode != 0:
        print("[bold red]Error while generating intents. See message above.[/bold red]")
    else:
        print(
            f"[bold green]Success:[/bold green] intents file was generated. It was saved to {fname} in {INTENTS_FOLDER} folder"
        )
