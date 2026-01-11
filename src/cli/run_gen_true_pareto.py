import os
import subprocess
from pathlib import Path

from rich import print
from rich.table import Table

from src.cli.folder_and_fnames import PF_FOLDER, generate_true_pareto_fname


def run_gen_true_pareto(graph_fname: str, intents_fname: str) -> None:
    print("[bold yellow]Generating true pareto front...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    config_table.add_row("Intents", intents_fname)
    print(config_table)

    output_dir = Path.cwd() / PF_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    fname = generate_true_pareto_fname(graph_fname, intents_fname)
    output_path = output_dir / fname
    abs_output_path = output_path.resolve()

    # TODO replace with C++ program - remember pass the absolute output path to it
    result = subprocess.run(
        [
            "echo",
            "hello from true pareto program :)",
            "Path to tpf output: ",
            abs_output_path,
        ]
    )
    if result.returncode != 0:
        print(
            f"[bold red]Error running true pareto front generation.[/bold red] Error code: {result.returncode}"
        )
    else:
        print(
            f"[bold green]Success:[/bold green] true pareto front was generated. It was saved to {fname} in {PF_FOLDER} folder"
        )
