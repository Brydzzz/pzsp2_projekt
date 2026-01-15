from pathlib import Path

import numpy as np
import pandas as pd

from cli.folder_and_fnames import PF_FOLDER


def generate_distribution_data(config, num_runs=5, sols_per_run=10):
    data = []

    for algo, metrics in config.items():
        for run in range(1, num_runs + 1):
            for _ in range(sols_per_run):
                loss = int(
                    round(np.random.normal(metrics["loss"][0], metrics["loss"][1]))
                )
                delay = int(
                    round(np.random.normal(metrics["delay"][0], metrics["delay"][1]))
                )
                jitter = int(
                    round(np.random.normal(metrics["jitter"][0], metrics["jitter"][1]))
                )

                data.append(
                    {
                        "algo": algo,
                        "run_id": run,
                        "paths": f"[{algo}_path_sample]",
                        "loss": loss,
                        "delay": delay,
                        "jitter": jitter,
                        "valid": True,
                    }
                )

    return pd.DataFrame(data)


def get_non_dominated_points(points: np.ndarray) -> np.ndarray:
    """
    Returns the points that are not dominated by any other point.
    Assumes minimization for all objectives.
    """
    is_efficient = np.ones(points.shape[0], dtype=bool)
    for i, p in enumerate(points):
        if is_efficient[i]:
            # Keep points that are NOT worse than 'p' in all objectives
            # and ARE better in at least one objective.
            is_efficient[is_efficient] = np.any(points[is_efficient] < p, axis=1)
            is_efficient[i] = True  # Keep self
    return points[is_efficient]


def generate_true_pf(config, num_samples=1000):
    all_points = []

    for algo, metrics in config.items():
        # Generate a large pool of points to find the theoretical boundary
        loss = np.random.normal(metrics["loss"][0], metrics["loss"][1], num_samples)
        delay = np.random.normal(metrics["delay"][0], metrics["delay"][1], num_samples)
        jitter = np.random.normal(
            metrics["jitter"][0], metrics["jitter"][1], num_samples
        )

        # Combine into a (Samples, 3) array
        algo_points = np.vstack((loss, delay, jitter)).T
        all_points.append(algo_points)

    # Stack all algorithms together
    combined_points = np.vstack(all_points)

    # Extract the True Pareto Front
    true_pf = get_non_dominated_points(combined_points)

    # Sort by loss for easier visualization/debugging
    true_pf = true_pf[true_pf[:, 0].argsort()]

    return true_pf


def save_true_pf(
    true_pf: np.ndarray, folder: str = PF_FOLDER, fname: str = "example_true_pf.csv"
):
    # 1. Prepare the absolute path
    output_dir = Path(folder).resolve()
    file_path = output_dir / fname

    # 2. Ensure the directory exists (as discussed, better to do in Python)
    output_dir.mkdir(parents=True, exist_ok=True)

    # 3. Save the matrix
    # header: adds column names
    # fmt: %.2f for floats, or %d if you converted them to integers
    # delimiter: comma is standard for CSV
    np.savetxt(
        file_path,
        true_pf,
        delimiter=",",
        header="loss,delay,jitter",
        comments="",
        fmt="%.2f",
    )

    print(f"True Pareto Front saved to: {file_path}")


if __name__ == "__main__":
    config = {
        "NSGA2": {"loss": (65.0, 5.0), "delay": (12.0, 2.0), "jitter": (-4.0, 1.0)},
        "SPEA2": {"loss": (30.0, 3.0), "delay": (15.0, 2.5), "jitter": (2.0, 0.5)},
        "INSGA": {"loss": (15.0, 2.0), "delay": (55.0, 8.0), "jitter": (18.0, 4.0)},
    }

    df = generate_distribution_data(config)
    df.to_csv("algo_compare/example_algo_comp_raw_data.csv", index=False)
    print("Data generated. Example of the first few rows:")
    print(df.head())

    true_pf_matrix = generate_true_pf(config)
    print(f"Generated a True PF with {len(true_pf_matrix)} non-dominated points.")
    print(true_pf_matrix[:5])  # Show first 5 points
    # Usage
    # true_pf_matrix was generated in the previous step
    save_true_pf(true_pf_matrix)
