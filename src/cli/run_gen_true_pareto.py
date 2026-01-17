import os
import subprocess
from pathlib import Path

from rich import print
from rich.table import Table

from src.cli.folder_and_fnames import (
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
    generate_true_pareto_fname,
)


def run_gen_true_pareto(
    graph_fname: str,
    intents_fname: str,
    iterations: int,
    runs: int,
    mut_prob: float,
) -> None:
    print("[bold yellow]Generating true pareto front...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    config_table.add_row("Intents", intents_fname)
    config_table.add_row("Number of iterations", str(iterations))
    config_table.add_row("Number of runs", str(runs))
    config_table.add_row("Mutation probability", str(mut_prob))
    print(config_table)

    output_dir = Path.cwd() / PF_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    fname = generate_true_pareto_fname(graph_fname, intents_fname)
    output_path = output_dir / fname
    abs_output_path = output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()
    abs_intents_path = (Path(INTENTS_FOLDER) / intents_fname).resolve()

    result = subprocess.run(
        [
            "./build/gen_true_pf",
            abs_graph_path,
            abs_intents_path,
            abs_output_path,
            str(iterations),
            str(runs),
            str(mut_prob),
        ]
    )
    if result.returncode != 0:
        print(
            "[bold red]Error running true pareto front generation. See message above.[/bold red]"
        )
    else:
        print(
            f"[bold green]Success:[/bold green] true pareto front was generated. It was saved to {fname} in {PF_FOLDER} folder"
        )
