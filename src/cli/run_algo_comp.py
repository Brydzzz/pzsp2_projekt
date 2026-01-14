import os
import subprocess
from pathlib import Path

import pandas as pd
from matplotlib import pyplot as plt
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

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()
    abs_intents_path = (Path(INTENTS_FOLDER) / intents_fname).resolve()

    result = subprocess.run(
        [
            "./build/gen_algo_compare_data",
            abs_graph_path,
            abs_intents_path,
            raw_data_abs_path,
            str(iterations),
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
    if not is_raw_data_format_valid(raw_data):
        print("[bold red]Incorrect raw data format[/bold red].")
        return

    pf_path = Path.cwd() / PF_FOLDER / true_pareto_fname
    true_pf_df = pd.read_csv(pf_path)
    if set(true_pf_df.columns) != set(OBJECTIVES):
        print("[bold red]Incorrect true pareto front data format[/bold red].")
        return

    # TODO: handle invalid solutions???

    fronts_df = get_fronts_by_run(raw_data, OBJECTIVES)
    metrics_summary = calculate_metrics(fronts_df, true_pf_df, OBJECTIVES)
    objectives_summary = calculate_objectives_stats(raw_data)

    results = pd.concat([metrics_summary, objectives_summary], axis=1)
    print(f"\n[bold]Experiment results:[/bold]\n\n{results}\n")
    results_output_fname = generate_alg_comp_results_fname(
        graph_fname, intents_fname, iterations
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
    y_labels = {"loss": "Loss[%]", "jitter": "Jitter[ms]", "delay": "Delay[ms]"}
    colors = ["#2ecc71", "#e67e22", "#9b59b6"]

    fig1.suptitle(
        f"Objectives Values for {results_fname}",
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
            f"{metric.replace('_plus', '+').upper()}\n", fontsize=12, fontweight="bold"
        )

        ax.set_xlabel("")
        ax.tick_params(axis="x", labelrotation=0)
        ax.grid(axis="y", linestyle="--", alpha=0.6)

    plt.tight_layout()
    plt.savefig(metrics_output_path)
    print(
        f"[bold blue]Metrics plot saved successfully to {metrics_output_path}[/bold blue]"
    )
