import os
import subprocess
from pathlib import Path

import pandas as pd
from matplotlib import pyplot as plt
from pymoo.indicators.gd import GD
from pymoo.indicators.gd_plus import GDPlus
from pymoo.indicators.igd import IGD
from pymoo.indicators.igd_plus import IGDPlus
from rich import print

from src.experiments_processing.conv_check_utils import (
    is_raw_data_format_valid_conv_check,
)
from src.experiments_processing.processing_common import executable_exists, print_config_table

from .folder_and_fnames import (
    CONV_CHECK_FOLDER,
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
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
    bin_path = "./build/gen_check_conv"
    if not executable_exists(bin_path):
        print(
            "[bold red]Error: c++ program `gen_check_conv` hasn't been built.[/bold red]"
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
            "Max population": str(max_pop),
            "Step": str(step),
            "Plot data": str(plot_data),
        }
    )

    output_dir = Path.cwd() / CONV_CHECK_FOLDER
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
            bin_path,
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
    results_output_fname = generate_conv_check_result_fname(
        graph_fname, intents_fname, iterations, runs, mut_prob, max_pop, step
    )
    output_dir = (
        Path.cwd() / CONV_CHECK_FOLDER / "".join(results_output_fname.split(".")[:-1])
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
    output_folder: str = "plots_convergence",
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

    print(f"[bold green]Plotting convergence charts to {output_folder}...[/bold green]")
    for pop in pop_sizes:
        pop_data = metrics_history_df[metrics_history_df["popsize"] == pop]
        for metric in metrics_list:
            plt.figure(figsize=(5, 5))
            plt.title(
                f"Convergence of {metric} for popsize: {pop}",
            )
            for algo in algos:
                algo_data = pop_data[pop_data["algo"] == algo]
                if algo_data.empty:
                    continue
                agg = algo_data.groupby("iteration")[metric].agg(["mean", "std"])
                agg["std"] = agg["std"].fillna(0)
                plt.plot(
                    agg.index,
                    agg["mean"],
                    label=algo,
                    color=algo_color_map[algo],
                    linewidth=2,
                )
                plt.fill_between(
                    agg.index,
                    agg["mean"] - agg["std"],
                    agg["mean"] + agg["std"],
                    color=algo_color_map[algo],
                    alpha=0.15,
                )

            plt.xlabel("Iteration")
            plt.ylabel("Metric value")
            plt.yscale("log")
            plt.legend(title="Algorithm")

            fname = f"conv_pop_{pop}_{metric}.png"
            output_path = os.path.join(output_folder, fname)
            plt.tight_layout()
            plt.savefig(output_path, dpi=300)
            plt.close()
            print(f"Saved: {fname}")
