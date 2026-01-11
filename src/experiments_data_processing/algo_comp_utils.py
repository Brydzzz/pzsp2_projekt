import numpy as np
import pandas as pd
from pymoo.indicators.gd import GD
from pymoo.indicators.gd_plus import GDPlus
from pymoo.indicators.igd import IGD
from pymoo.indicators.igd_plus import IGDPlus

from src.cli.folder_and_fnames import ALGO_COMPARE_FOLDER


def get_fronts_by_run(df: pd.DataFrame):
    objectives = ["loss", "delay", "jitter"]

    fronts = (
        df.groupby(["algo", "run_id"])[objectives]
        .apply(lambda x: x.values.tolist())
        .reset_index(name="points")
    )

    return fronts


def calculate_metrics(grouped_df: pd.DataFrame, true_pf: np.ndarray) -> pd.DataFrame:
    """
    :param grouped_df: DataFrame with columns ['algo', 'run_id', 'points']
    :param true_pf: 2D array shaped of the true pareto front
    """
    gd_ind = GD(true_pf)
    gdp_ind = GDPlus(true_pf)
    igd_ind = IGD(true_pf)
    igdp_ind = IGDPlus(true_pf)

    results = []

    for _, row in grouped_df.iterrows():
        front = np.array(row["points"])

        metrics = {
            "algo": row["algo"],
            "run_id": row["run_id"],
            "gd": gd_ind(front),
            "gd_plus": gdp_ind(front),
            "igd": igd_ind(front),
            "igd_plus": igdp_ind(front),
        }
        results.append(metrics)

    return (
        pd.DataFrame(results)
        .groupby("algo")
        .agg(
            {
                "gd": ["mean", "std"],
                "gd_plus": ["mean", "std"],
                "igd": ["mean", "std"],
                "igd_plus": ["mean", "std"],
            }
        )
    )


def calculate_objectives_stats(experiments_data: pd.DataFrame) -> pd.DataFrame:
    return experiments_data.groupby("algo").agg(
        {"loss": ["mean", "std"], "jitter": ["mean", "std"], "delay": ["mean", "std"]}
    )


def is_raw_data_format_valid(data: pd.DataFrame) -> bool:
    expected = {"algo", "paths", "loss", "delay", "jitter", "valid", "run_id"}
    if set(data.columns) != expected:
        return False

    expected_dtypes = {
        "algo": "object",
        "paths": "object",
        "loss": "int64",
        "delay": "int64",
        "jitter": "int64",
        "valid": "bool",
        "run_id": "int64",
    }
    actual_dtypes = data.dtypes.astype(str).to_dict()
    return actual_dtypes == expected_dtypes


def save_algo_comp_results(results_df: pd.DataFrame, output_fname: str):
    results_df.columns = [f"{col}_{stat}" for col, stat in results_df.columns]
    results_df = results_df.reset_index()

    results_df.to_csv(
        f"{ALGO_COMPARE_FOLDER}/{output_fname}",
        index=False,
    )


if __name__ == "__main__":
    rng = np.random.default_rng(40)
    true_pf = rng.normal(loc=[20, 30, 5], scale=1.0, size=(100, 3))
    pareto_fronts = np.array(
        [
            rng.normal(loc=[21, 31, 6], scale=1.5, size=(20, 3)),
            rng.normal(loc=[23, 33, 7], scale=1.5, size=(20, 3)),
            rng.normal(loc=[25, 35, 8], scale=1.5, size=(20, 3)),
        ]
    )
    metrics_results = calculate_metrics(pareto_fronts, true_pf)
    for i, res in enumerate(metrics_results):
        print(f"--- Run {i + 1} ---")
        print(f"GD:  {res.gd:.4f}")
        print(f"GD+:  {res.gd_plus:.4f}")
        print(f"IGD: {res.igd:.4f}")
        print(f"IGD+: {res.igd_plus:.4f}")
