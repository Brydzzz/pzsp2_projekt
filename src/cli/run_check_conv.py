import os
import subprocess
from pathlib import Path

import pandas as pd
from matplotlib import pyplot as plt
from rich import print
from rich.table import Table
from pymoo.indicators.gd import GD
from pymoo.indicators.gd_plus import GDPlus
from pymoo.indicators.igd import IGD
from pymoo.indicators.igd_plus import IGDPlus

from src.experiments_data_processing.algo_comp_utils import (
    is_raw_data_format_valid_conv_check,
)

from .folder_and_fnames import (
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
    CONV_CHECK_FOLDER,
    generate_conv_check_raw_fname,
    generate_conv_check_result_fname,
)


OBJECTIVES = ["loss", "delay", "jitter"]


def run_check_conv(
    graph_fname: str,
    intents_fname: str,
    true_pareto_fname: str,
    iterations: int,
    runs: int,
    mut_prob: float,
    max_pop: int,
    step: int,
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
    config_table.add_row("Number of runs", str(runs))
    config_table.add_row("Mutation probability", str(mut_prob))
    config_table.add_row("Max population", str(max_pop))
    config_table.add_row("Step", str(step))
    config_table.add_row("Plot data", str(plot_data))
    print(config_table)
    results_output_fname = generate_conv_check_result_fname(
        graph_fname, intents_fname, iterations, runs, mut_prob, max_pop, step
    )
    output_dir = (
        Path.cwd()
        / CONV_CHECK_FOLDER
        / "".join(results_output_fname.split(".")[:-1])
    )
    os.makedirs(output_dir, exist_ok=True)
    raw_data_fname = generate_conv_check_raw_fname(
        graph_fname, intents_fname, iterations, runs, mut_prob, max_pop, step
    )
    raw_data_output_path = output_dir / raw_data_fname
    raw_data_abs_path = raw_data_output_path.resolve()

    abs_graph_path = (Path(GRAPH_FOLDER) / graph_fname).resolve()
    abs_intents_path = (Path(INTENTS_FOLDER) / intents_fname).resolve()

    result = subprocess.run(
        [
            "./build/gen_check_conv",
            abs_graph_path,
            abs_intents_path,
            raw_data_abs_path,
            str(iterations),
            str(runs),
            str(mut_prob),
            str(max_pop),
            str(step),
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
            f"Results were saved to {raw_data_fname} in {CONV_CHECK_FOLDER} folder"
        )

    raw_data = pd.read_csv(raw_data_abs_path)
    if not is_raw_data_format_valid_conv_check(raw_data):
        print("[bold red]Incorrect raw data format[/bold red].")
        return

    pf_path = Path.cwd() / PF_FOLDER / true_pareto_fname
    true_pf_df = pd.read_csv(pf_path)
    if set(true_pf_df.columns) != set(OBJECTIVES):
        print("[bold red]Incorrect true pareto front data format[/bold red].")
        return

    output_dir = (
        Path.cwd()
        / CONV_CHECK_FOLDER
        / "".join(results_output_fname.split(".")[:-1])
    )
    if plot_data:
        create_convergence_plots(
            raw_data,
            true_pf_df,
            ["loss", "delay", "jitter"],
            output_dir,
        )


def create_convergence_plots(
    raw_data: pd.DataFrame,
    true_pf_df: pd.DataFrame,
    objective_names: list[str],
    output_folder: str = "convergence_check",
) -> None:
    print(
        "[bold yellow]Calculating convergence metrics over iterations...[/bold yellow]"
    )
    true_pf_arr = true_pf_df[objective_names].to_numpy()
    ind_gd = GD(true_pf_arr)
    ind_igd = IGD(true_pf_arr)
    ind_gdp = GDPlus(true_pf_arr)
    ind_igdp = IGDPlus(true_pf_arr)

    results = []

    grouped = raw_data.groupby(["algo", "run_id", "iteration", "popsize"])

    for (algo, run_id, iteration, popsize), group in grouped:
        current_front = group[objective_names].to_numpy()
        metrics_row = {
            "algo": algo,
            "run_id": run_id,
            "iteration": iteration,
            "popsize": popsize,
            "gd": ind_gd(current_front),
            "gd+": ind_gdp(current_front),
            "igd": ind_igd(current_front),
            "igd+": ind_igdp(current_front),
        }
        results.append(metrics_row)
    metrics_history_df = pd.DataFrame(results)
    os.makedirs(output_folder, exist_ok=True)

    metrics_list = ["gd", "gd+", "igd", "igd+"]
    pop_sizes = sorted(metrics_history_df["popsize"].unique())
    algos = ["INSGA", "NSGA2", "SPEA2"]
    colors = ["blue", "orange", "green"]
    algo_color_map = dict(zip(algos, colors))

    print(
        f"[bold green]Plotting convergence charts to {output_folder}...[/bold green]"
    )

    n_rows = len(pop_sizes)
    n_cols = len(metrics_list)

    fig, axes = plt.subplots(
        nrows=n_rows,
        ncols=n_cols,
        figsize=(5 * n_cols, 5 * n_rows),
        squeeze=False,
    )

    for i, pop in enumerate(pop_sizes):
        pop_data = metrics_history_df[metrics_history_df["popsize"] == pop]
        for j, metric in enumerate(metrics_list):
            ax = axes[i, j]
            ax.set_title(f"Popsize: {pop} | Metric: {metric}")

            for algo in algos:
                algo_data = pop_data[pop_data["algo"] == algo]
                if algo_data.empty:
                    continue
                agg = algo_data.groupby("iteration")[metric].agg(
                    ["mean", "std"]
                )
                agg["std"] = agg["std"].fillna(0)

                ax.plot(
                    agg.index,
                    agg["mean"],
                    label=algo,
                    color=algo_color_map[algo],
                    linewidth=2,
                )
                ax.fill_between(
                    agg.index,
                    agg["mean"] - agg["std"],
                    agg["mean"] + agg["std"],
                    color=algo_color_map[algo],
                    alpha=0.15,
                )

            ax.set_xlabel("Iteration")
            ax.set_ylabel("Metric value")
            ax.set_yscale("log")

            if i == 0 and j == 0:
                ax.legend(title="Algorithm")
    output_path = os.path.join(output_folder, "convergence_check.png")
    plt.tight_layout()
    plt.savefig(output_path, dpi=300, bbox_inches="tight")
    plt.close()
    print(f"Saved: {output_path}")
