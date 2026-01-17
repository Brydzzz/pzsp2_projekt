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
from src.networks_processor.fullmesh_generator import generate_and_save_fullmesh


def run_c_comp(
    load_data_filename: str,
    iterations: int,
    runs: int,
    mutation_prob: float,
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
                f"Error: File '{load_data_filename}' not found in '{C_COMP_DATA_FOLDER}/'."
            )
            return

        try:
            df = pd.read_csv(file_path)
            required_columns = {"node_count", "algorithm", "execution_time", "run_id"}

            if not required_columns.issubset(df.columns):
                raise ValueError("Missing required columns")

            if not pd.api.types.is_numeric_dtype(
                df["node_count"]
            ) or not pd.api.types.is_numeric_dtype(df["execution_time"]):
                raise ValueError(
                    "Non-numeric data in node_count or execution_time columns"
                )

        except Exception as e:
            print(f"Invalid data format. [bold red]Error:[/bold red]{e}")
            return

        _plot_complexity_data(df, load_data_filename, None, None, None)
        return

    else:
        print(
            f"Starting Complexity Tests (Iter: {iterations}, Runs: {runs}, Mut: {mutation_prob})..."
        )

        NODE_COUNTS = [5, 10, 15, 20, 25, 30, 35, 40, 45, 50]
        final_graph_paths = []
        final_intent_paths = []

        print("Generating network topologies and intents...")
        for node_count in NODE_COUNTS:
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
            "./build/gen_c_comp_data",
            abs_output_path,
            str(iterations),
            str(runs),
            str(mutation_prob),
        ] + file_args

        try:
            subprocess.run(cmd, check=True)

            if os.path.exists(abs_output_path):
                final_df = pd.read_csv(abs_output_path)
                _plot_complexity_data(
                    final_df, output_fname, iterations, runs, mutation_prob
                )
            else:
                print("Error: Output file was not created by the c++ program.")

        except subprocess.CalledProcessError as e:
            print(f"Error running C++ program: {e}")


def _plot_complexity_data(
    df: pd.DataFrame,
    results_fname: str,
    iterations: int | None,
    runs: int | None,
    mut_prob: float | None,
):
    output_path = Path(C_COMP_DATA_FOLDER) / generate_c_comp_plot_fname(results_fname)
    fig, ax = plt.subplots(figsize=(10, 6))
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
