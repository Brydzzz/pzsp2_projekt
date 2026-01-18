import os
import subprocess
from pathlib import Path

import pandas as pd
from matplotlib import pyplot as plt
from rich import print

from src.experiments_processing.algo_comp_utils import (
    calculate_metrics,
    calculate_objectives_stats,
    get_fronts_by_run,
    is_algo_compare_data_format_valid,
    save_algo_comp_results,
)
from src.experiments_processing.processing_common import (
    executable_exists,
    print_config_table,
)

from .folder_and_fnames import (
    ALGO_COMPARE_FOLDER,
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
    generate_alg_comp_plots_fname,
    generate_alg_comp_raw_data_fname,
    generate_alg_comp_results_fname,
)

OBJECTIVES = ["loss", "delay", "jitter"]


def run_algo_comp(
    graph_fname: str,
    intents_fname: str,
    true_pareto_fname: str,
    iterations: int,
    runs: int,
    mut_prob: float,
    plot_data: bool,
) -> None:
    bin_path = "./build/gen_algo_compare_data"
    if not executable_exists(bin_path):
        print(
            "[bold red]Error: c++ program `gen_algo_compare_data` hasn't been built.[/bold red]"
        )
        print(
            f"Please build it first and make sure it is placed in {Path(bin_path).resolve()}"
        )
        return
    print("[bold yellow]Running algorithm comparison...[/bold yellow]")
    print_config_table(
        {
            "Graph": graph_fname,
            "Intents": intents_fname,
            "True Pareto Front": true_pareto_fname,
            "Number of iterations": str(iterations),
            "Number of runs": str(runs),
            "Mutation probability": str(mut_prob),
            "Plot data": str(plot_data),
        }
    )

    output_dir = Path.cwd() / ALGO_COMPARE_FOLDER
    os.makedirs(output_dir, exist_ok=True)
    raw_data_fname = generate_alg_comp_raw_data_fname(
        graph_fname, intents_fname, iterations, runs, mut_prob
    )
    raw_data_output_path = output_dir / raw_data_fname
    raw_data_abs_path = raw_data_output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()
    abs_intents_path = (Path(INTENTS_FOLDER) / intents_fname).resolve()

    result = subprocess.run(
        [
            bin_path,
            abs_graph_path,
            abs_intents_path,
            raw_data_abs_path,
            str(iterations),
            str(runs),
            str(mut_prob),
        ]
    )
    if result.returncode != 0:
        print(
            f"[bold red]Error while comparing algorithms.[/bold red] Error code: {result.returncode}"
        )
        return
    else:
        print(
            "[bold green]Success:[/bold green] compare algorithms experiment has finished. "
            f"Results were saved to {raw_data_fname} in {ALGO_COMPARE_FOLDER} folder"
        )

    raw_data = pd.read_csv(raw_data_abs_path)
    if not is_algo_compare_data_format_valid(raw_data):
        print("[bold red]Incorrect raw data format[/bold red].")
        return

    pf_path = Path.cwd() / PF_FOLDER / true_pareto_fname
    true_pf_df = pd.read_csv(pf_path)
    if set(true_pf_df.columns) != set(OBJECTIVES):
        print("[bold red]Incorrect true pareto front data format[/bold red].")
        return

    fronts_df = get_fronts_by_run(raw_data, OBJECTIVES)
    metrics_summary = calculate_metrics(fronts_df, true_pf_df, OBJECTIVES)
    objectives_summary = calculate_objectives_stats(raw_data)

    results = pd.concat([metrics_summary, objectives_summary], axis=1)
    print(f"\n[bold]Experiment results:[/bold]\n\n{results}\n")
    results_output_fname = generate_alg_comp_results_fname(
        graph_fname, intents_fname, iterations, runs, mut_prob
    )
    save_algo_comp_results(results, results_output_fname)

    if plot_data:
        create_plots(results, results_output_fname)


def create_plots(results_df: pd.DataFrame, results_fname: str) -> None:
    obj_output_path, metrics_output_path = generate_alg_comp_plots_fname(results_fname)
    obj_output_path, metrics_output_path = (
        f"{ALGO_COMPARE_FOLDER}/{obj_output_path}",
        f"{ALGO_COMPARE_FOLDER}/{metrics_output_path}",
    )

    fig1, axes1 = plt.subplots(1, 3, figsize=(18, 5))
    objectives = ["loss", "jitter", "delay"]
    y_labels = {
        "loss": "Loss[%]",
        "jitter": "Jitter[ms]",
        "delay": "Delay[ms]",
    }
    colors = ["#2ecc71", "#e67e22", "#9b59b6"]

    fig1.suptitle(
        f"Objectives Values for \n {results_fname}",
        fontsize=18,
        fontweight="bold",
    )

    for i, obj in enumerate(objectives):
        ax = axes1[i]
        results_df[f"{obj}_mean"].plot(
            kind="bar",
            yerr=results_df[f"{obj}_std"],
            ax=ax,
            color=colors[i],
            capsize=5,
            edgecolor="black",
        )
        ax.set_xlabel("")
        ax.set_ylabel(y_labels[obj], fontweight="bold")
        ax.tick_params(axis="x", labelrotation=0)
        ax.grid(axis="y", linestyle="--", alpha=0.6)

    plt.tight_layout()
    plt.savefig(obj_output_path)
    print(
        f"[bold blue]Objectivs plot saved successfully to {obj_output_path}[/bold blue]"
    )

    fig2, axes2 = plt.subplots(1, 4, figsize=(20, 5))
    metrics = ["gd", "gd_plus", "igd", "igd_plus"]
    colors = ["#3498db", "#2980b9", "#e74c3c", "#c0392b"]

    fig2.suptitle(
        f"Metrics for {results_fname}",
        fontsize=18,
        fontweight="bold",
    )

    for i, metric in enumerate(metrics):
        ax = axes2[i]

        results_df[f"{metric}_mean"].plot(
            kind="bar",
            yerr=results_df[f"{metric}_std"],
            ax=ax,
            color=colors[i],
            capsize=5,
            edgecolor="black",
        )
        ax.set_title(
            f"{metric.replace('_plus', '+').upper()}\n",
            fontsize=12,
            fontweight="bold",
        )

        ax.set_xlabel("")
        ax.tick_params(axis="x", labelrotation=0)
        ax.grid(axis="y", linestyle="--", alpha=0.6)

    plt.tight_layout()
    plt.savefig(metrics_output_path)
    print(
        f"[bold blue]Metrics plot saved successfully to {metrics_output_path}[/bold blue]"
    )
