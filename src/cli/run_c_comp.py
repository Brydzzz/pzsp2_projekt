import os
import subprocess
from pathlib import Path

import matplotlib.pyplot as plt
import pandas as pd
from rich import print

from src.cli.folder_and_fnames import (
    C_COMP_DATA_FOLDER,
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    generate_c_comp_fname,
    generate_c_comp_plot_fname,
    generate_intents_fname,
)
from src.cli.run_gen_intents import run_gen_intents
from src.experiments_processing.c_comp_utils import is_c_comp_data_format_valid
from src.experiments_processing.processing_common import executable_exists
from src.networks_processor.fullmesh_generator import generate_and_save_fullmesh


def run_c_comp(
    load_data_filename: str | None,
    iterations: int,
    runs: int,
    mutation_prob: float,
    min_nodes: int,
    max_nodes: int,
    step: int,
):
    base_dir = Path.cwd()
    data_dir = base_dir / C_COMP_DATA_FOLDER
    graph_dir = base_dir / GRAPH_FOLDER
    intents_dir = base_dir / INTENTS_FOLDER
    os.makedirs(data_dir, exist_ok=True)

    if load_data_filename:
        file_path = data_dir / load_data_filename

        if not os.path.exists(file_path):
            print(
                f"[bold red]Error:[/bold red] File '{load_data_filename}' not found in '{C_COMP_DATA_FOLDER}/'."
            )
            return

        df = pd.read_csv(file_path)
        is_valid, err_msg = is_c_comp_data_format_valid(df)
        if not is_valid:
            print(f"[bold red]Error: invalid data format, {err_msg} [/bold red]")
            return

        plot_complexity_data(df, load_data_filename, None, None, None)

    else:
        bin_path = "./build/gen_c_comp_data"
        if not executable_exists(bin_path):
            print(
                "[bold red]Error: c++ program `gen_c_comp_data` hasn't been built.[/bold red]"
            )
            print(
                f"Please build it first and make sure it is placed in {Path(bin_path).resolve()}"
            )
            return
        node_counts = [i for i in range(min_nodes, max_nodes + 1, step)]
        print(f"Number of nodes in network that will be tested: {node_counts}")
        print(
            f"Starting Complexity Tests (Iter: {iterations}, Runs: {runs}, Mut: {mutation_prob})..."
        )

        final_graph_paths = []
        final_intent_paths = []

        print("[bold yellow]Generating network topologies and intents...[/bold yellow]")
        for node_count in node_counts:
            base_name = f"full_mesh_{node_count}_c_comp"
            csv_name = f"{base_name}.csv"
            intent_name = generate_intents_fname(csv_name)

            generate_and_save_fullmesh(node_count, base_name)
            run_gen_intents(csv_name, intent_name, should_print_config=False)

            final_graph_paths.append((graph_dir / csv_name).resolve())
            final_intent_paths.append((intents_dir / intent_name).resolve())

        output_fname = generate_c_comp_fname(iterations, runs, mutation_prob)
        abs_output_path = (data_dir / output_fname).resolve()
        file_args = []
        for g_path, i_path in zip(final_graph_paths, final_intent_paths):
            file_args.append(str(g_path))
            file_args.append(str(i_path))
        cmd = [
            bin_path,
            abs_output_path,
            str(iterations),
            str(runs),
            str(mutation_prob),
        ] + file_args

        try:
            subprocess.run(cmd, check=True)

            if os.path.exists(abs_output_path):
                final_df = pd.read_csv(abs_output_path)
                plot_complexity_data(
                    final_df, output_fname, iterations, runs, mutation_prob
                )
            else:
                print(
                    "[bold red]Error: [/bold red]Output file was not created by the C++ program."
                )

        except subprocess.CalledProcessError as e:
            print(f"[bold red]Error running C++ program:[/bold red] {e}")


def plot_complexity_data(
    df: pd.DataFrame,
    results_fname: str,
    iterations: int | None,
    runs: int | None,
    mut_prob: float | None,
):
    output_path = Path(C_COMP_DATA_FOLDER) / generate_c_comp_plot_fname(results_fname)
    _, ax = plt.subplots(figsize=(10, 6))
    df = df.sort_values(by="node_count")
    avg_df = (
        df.groupby(["node_count", "algorithm"])["execution_time"].mean().reset_index()
    )

    algorithms = avg_df["algorithm"].unique()
    for algo in algorithms:
        subset = avg_df[avg_df["algorithm"] == algo]
        ax.plot(
            subset["node_count"],
            subset["execution_time"],
            marker="o",
            linestyle="-",
            label=algo,
        )

    ax.set_xlabel("Number of Nodes (Fullmesh Network Size)")
    ax.set_ylabel("Execution Time (s)")
    ax.set_title(
        "Computational Complexity Comparison \n"
        f"Iterations: {iterations if iterations else 'n/a'} Runs: {runs if runs else 'n/a'} "
        f"Mutation Prob: {mut_prob if mut_prob else 'n/a'}"
    )
    ax.grid(True, linestyle="--", alpha=0.7)

    ax.legend(title="Algorithms")

    plt.tight_layout()
    plt.savefig(output_path)
    print(
        f"[bold blue]Computational Complexity plot saved successfully to {output_path}[/bold blue]"
    )
