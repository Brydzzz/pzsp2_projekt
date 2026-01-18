import os
import subprocess
from pathlib import Path

from rich import print

from src.cli.folder_and_fnames import (
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
    generate_true_pareto_fname,
)
from src.experiments_processing.processing_common import (
    executable_exists,
    print_config_table,
)


def run_gen_true_pareto(
    graph_fname: str,
    intents_fname: str,
    iterations: int,
    runs: int,
    mut_prob: float,
) -> None:
    bin_path = "./build/gen_true_pf"
    if not executable_exists(bin_path):
        print(
            "[bold red]Error: c++ program `gen_true_pf` hasn't been built.[/bold red]"
        )
        print(
            f"Please build it first and make sure it is placed in {Path(bin_path).resolve()}"
        )
        return
    print("[bold yellow]Generating true pareto front...[/bold yellow]")
    print_config_table(
        {
            "Graph": graph_fname,
            "Intents": intents_fname,
            "Number of iterations": str(iterations),
            "Number of runs": str(runs),
            "Mutation probability": str(mut_prob),
        }
    )

    output_dir = Path.cwd() / PF_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    fname = generate_true_pareto_fname(graph_fname, intents_fname)
    output_path = output_dir / fname
    abs_output_path = output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()
    abs_intents_path = (Path(INTENTS_FOLDER) / intents_fname).resolve()

    result = subprocess.run(
        [
            bin_path,
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
