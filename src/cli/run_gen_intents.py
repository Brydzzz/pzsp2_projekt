import os
import subprocess
from pathlib import Path

from rich import print
from rich.table import Table

from .folder_and_fnames import GRAPH_FOLDER, INTENTS_FOLDER, generate_intents_fname


def run_gen_intents(graph_fname: str, intents_fname: str | None = None):
    print("[bold yellow]Generating intents...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    print(config_table)

    output_dir = Path.cwd() / INTENTS_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    fname = generate_intents_fname(graph_fname) if not intents_fname else intents_fname
    output_path = output_dir / fname
    abs_output_path = output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()

    result = subprocess.run(
        [
            "./build/gen_intents",
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
