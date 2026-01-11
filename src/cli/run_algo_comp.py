import os
import subprocess
from pathlib import Path

import numpy as np
import pandas as pd
from rich import print
from rich.table import Table

from src.experiments_data_processing.algo_comp_utils import (
    calculate_metrics,
    calculate_objectives_stats,
    get_fronts_by_run,
    is_raw_data_format_valid,
    save_algo_comp_results,
)

from .folder_and_fnames import (
    ALGO_COMPARE_FOLDER,
    PF_FOLDER,
    generate_alg_comp_raw_data_fname,
    generate_alg_comp_results_fname,
)


def run_algo_comp(
    graph_fname: str,
    intents_fname: str,
    true_pareto_fname: str,
    iterations: int,
    plot_data: bool,
) -> None:
    print("[bold yellow]Running algorithm comparison...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    config_table.add_row("Intents", intents_fname)
    config_table.add_row("True Pareto Front", true_pareto_fname)
    config_table.add_row("Number of iterations", str(iterations))
    config_table.add_row("Plot data", str(plot_data))
    print(config_table)

    output_dir = Path.cwd() / ALGO_COMPARE_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    raw_data_fname = generate_alg_comp_raw_data_fname(
        graph_fname, intents_fname, iterations
    )
    raw_data_output_path = output_dir / raw_data_fname
    raw_data_abs_path = raw_data_output_path.resolve()

    # TODO replace with C++ program - remember pass the absolute output path to it
    result = subprocess.run(
        [
            "echo",
            "hello from algorithm comparison program :)",
            "Path to raw data output: ",
            raw_data_abs_path,
        ]
    )
    if result.returncode != 0:
        print(
            f"[bold red]Error while comparing algorithms.[/bold red] Error code: {result.returncode}"
        )
    else:
        print(
            "[bold green]Success:[/bold green] compare algorithms experiment has finished. "
            f"Results were saved to {raw_data_fname} in {ALGO_COMPARE_FOLDER} folder"
        )

    # temporary example data for testing purposes
    raw_data_fname = f"{ALGO_COMPARE_FOLDER}/example_algo_comp_raw_data.csv"
    raw_data = pd.read_csv(raw_data_fname)
    # raw_data = pd.read_csv(abs_path)
    if not is_raw_data_format_valid(raw_data):
        print("[bold red]Incorrect raw data format[/bold red].")

    pf_path = Path.cwd() / PF_FOLDER / true_pareto_fname
    true_pf = np.loadtxt(pf_path, delimiter=",", skiprows=1)

    fronts_df = get_fronts_by_run(raw_data)
    metrics_summary = calculate_metrics(fronts_df, true_pf)
    objectives_summary = calculate_objectives_stats(raw_data)

    results = pd.concat([metrics_summary, objectives_summary], axis=1)
    print(results)
    results_output_fname = generate_alg_comp_results_fname(
        graph_fname, intents_fname, iterations
    )
    save_algo_comp_results(results, results_output_fname)

    if plot_data:
        create_plots()


def create_plots():
    pass
