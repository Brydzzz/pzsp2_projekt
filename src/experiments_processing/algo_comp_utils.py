import numpy as np
import pandas as pd
from pymoo.indicators.gd import GD
from pymoo.indicators.gd_plus import GDPlus
from pymoo.indicators.igd import IGD
from pymoo.indicators.igd_plus import IGDPlus

from src.cli.folder_and_fnames import ALGO_COMPARE_FOLDER


def get_fronts_by_run(df: pd.DataFrame, objective_names: list[str]):
    """
    Groups raw algorithm data into fronts while ensuring objective order.
    """

    fronts = (
        df.groupby(["algo", "run_id"])[objective_names]
        .apply(lambda x: x.values.tolist())
        .reset_index(name="points")
    )

    return fronts


def calculate_metrics(
    grouped_df: pd.DataFrame,
    true_pf_df: pd.DataFrame,
    objective_names: list[str],
) -> pd.DataFrame:
    """
    :param grouped_df: DataFrame with columns ['algo', 'run_id', 'points']
    :param true_pf: 2D array shaped of the true pareto front
    :param objective_names: List of objectives names defining the order of values in 'points' column in grouped_df
    """
    true_pf_as_array = true_pf_df[objective_names].to_numpy()
    gd_ind = GD(true_pf_as_array)
    gdp_ind = GDPlus(true_pf_as_array)
    igd_ind = IGD(true_pf_as_array)
    igdp_ind = IGDPlus(true_pf_as_array)

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
        {
            "loss": ["mean", "std"],
            "jitter": ["mean", "std"],
            "delay": ["mean", "std"],
        }
    )


def is_algo_compare_data_format_valid(data: pd.DataFrame) -> bool:
    expected = {"algo", "loss", "delay", "jitter", "run_id"}
    if set(data.columns) != expected:
        return False

    expected_dtypes = {
        "algo": "object",
        "loss": "int64",
        "delay": "int64",
        "jitter": "int64",
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
