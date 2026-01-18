import pandas as pd
import pytest

from src.experiments_data_processing.algo_comp_utils import (
    calculate_metrics,
    calculate_objectives_stats,
    get_fronts_by_run,
    is_algo_compare_data_format_valid,
    save_algo_comp_results,
)


@pytest.fixture
def valid_raw_data():
    data = {
        "algo": ["algo1", "algo1", "algo2"],
        "run_id": [1, 1, 1],
        "loss": [10, 20, 15],
        "delay": [100, 200, 150],
        "jitter": [5, 10, 7],
    }
    return pd.DataFrame(data)


class TestRunAlgoComp:
    pass


class TestIsAlgoCompareDataFormatValid:
    def test_valid_format(self, valid_raw_data):
        assert is_algo_compare_data_format_valid(valid_raw_data) is True

    def test_missing_column(self, valid_raw_data):
        invalid_data = valid_raw_data.drop(columns=["jitter"])
        assert is_algo_compare_data_format_valid(invalid_data) is False

    def test_extra_column(self, valid_raw_data):
        invalid_data = valid_raw_data.copy()
        invalid_data["extra_col"] = 0
        assert is_algo_compare_data_format_valid(invalid_data) is False

    def test_wrong_dtype(self, valid_raw_data):
        invalid_data = valid_raw_data.copy()
        invalid_data["loss"] = invalid_data["loss"].astype(float)
        assert is_algo_compare_data_format_valid(invalid_data) is False

    def test_wrong_algo_dtype(self, valid_raw_data):
        invalid_data = valid_raw_data.copy()
        invalid_data["algo"] = ["3", "10", "18"]
        invalid_data["algo"] = invalid_data["algo"].astype(int)
        assert is_algo_compare_data_format_valid(invalid_data) is False

    def test_empty_dataframe_invalid(self):
        empty_df = pd.DataFrame()
        assert is_algo_compare_data_format_valid(empty_df) is False

    def test_empty_dataframe_valid_structure(self):
        empty_df = pd.DataFrame(
            {
                "algo": pd.Series(dtype="object"),
                "run_id": pd.Series(dtype="int64"),
                "loss": pd.Series(dtype="int64"),
                "delay": pd.Series(dtype="int64"),
                "jitter": pd.Series(dtype="int64"),
            }
        )
        assert is_algo_compare_data_format_valid(empty_df) is True


class TestAlgoCompareDataProcessing:
    def test_get_fronts_by_run_simple(self, valid_raw_data):
        objectives = ["loss", "delay", "jitter"]
        result = get_fronts_by_run(valid_raw_data, objectives)

        assert "points" in result.columns
        assert len(result) == 2

        points_algo1 = result[result["algo"] == "algo1"]["points"].iloc[0]
        assert len(points_algo1) == 2
        assert points_algo1[0] == [10, 100, 5]

    def test_get_fronts_by_runsingle_algorithm_single_run(self):
        data = pd.DataFrame(
            {
                "algo": ["NSGA2"],
                "run_id": [1],
                "loss": [10],
                "delay": [100],
                "jitter": [5],
            }
        )

        objective_names = ["loss", "delay", "jitter"]
        result = get_fronts_by_run(data, objective_names)

        assert len(result) == 1
        assert result.iloc[0]["algo"] == "NSGA2"
        assert result.iloc[0]["run_id"] == 1

    def test_get_fronts_by_run_multiple_runs_per_algorithm(self):
        data = pd.DataFrame(
            {
                "algo": ["NSGA2"] * 6 + ["SPEA2"] * 6,
                "run_id": [1, 1, 2, 2, 3, 3, 1, 1, 2, 2, 3, 3],
                "loss": list(range(12)),
                "delay": list(range(12)),
                "jitter": list(range(12)),
            }
        )

        result = calculate_objectives_stats(data)

        assert len(result) == 2
        assert result.loc["NSGA2", ("loss", "mean")] == 2.5
        assert result.loc["SPEA2", ("loss", "mean")] == 8.5
        assert result.loc["NSGA2", ("delay", "mean")] == 2.5
        assert result.loc["SPEA2", ("delay", "mean")] == 8.5
        assert result.loc["NSGA2", ("jitter", "mean")] == 2.5
        assert result.loc["SPEA2", ("jitter", "mean")] == 8.5

    def test_calculate_objectives_format(self, valid_raw_data):
        result = calculate_objectives_stats(valid_raw_data)

        assert "algo1" in result.index
        assert "algo2" in result.index
        assert ("loss", "mean") in result.columns
        assert ("loss", "std") in result.columns
        assert ("delay", "mean") in result.columns
        assert ("delay", "std") in result.columns
        assert ("jitter", "mean") in result.columns
        assert ("jitter", "std") in result.columns

    def test_calculate_objectives_stats_values(self, valid_raw_data):
        stats = calculate_objectives_stats(valid_raw_data)

        algo1_stats = stats.loc["algo1"]
        assert algo1_stats[("loss", "mean")] == 15.0
        assert algo1_stats[("loss", "std")] > 0

    def test_calculate_metrics_output_format(self):
        true_pf = pd.DataFrame(
            {"loss": [10, 20], "delay": [100, 200], "jitter": [5, 10]}
        )

        grouped_data = {
            "algo": ["algo1", "algo2", "algo2"],
            "run_id": [1, 2, 1],
            "points": [
                [[13, 110, 5], [25, 200, 10]],
                [[10, 100, 5], [20, 200, 10]],
                [[10, 100, 5], [20, 200, 10]],
            ],
        }
        grouped_df = pd.DataFrame(grouped_data)

        objs = ["loss", "delay", "jitter"]
        result = calculate_metrics(grouped_df, true_pf, objs)

        assert len(result) == 2
        assert "algo1" in result.index
        assert "algo2" in result.index
        assert ("gd", "mean") in result.columns
        assert ("gd", "std") in result.columns
        assert ("igd", "mean") in result.columns
        assert ("igd", "std") in result.columns

    def test_calculate_metrics_output_values(self):
        true_pf = pd.DataFrame(
            {"loss": [10, 20], "delay": [100, 200], "jitter": [5, 10]}
        )

        grouped_data = {
            "algo": ["algo1", "algo2", "algo2"],
            "run_id": [1, 2, 1],
            "points": [
                [[13, 110, 5], [25, 200, 10]],
                [[10, 100, 5], [20, 200, 10]],
                [[10, 100, 5], [20, 200, 10]],
            ],
        }
        grouped_df = pd.DataFrame(grouped_data)

        objs = ["loss", "delay", "jitter"]
        result = calculate_metrics(grouped_df, true_pf, objs)

        assert result.loc["algo1"][("gd", "mean")] > 0.0
        assert result.loc["algo1"][("igd", "mean")] > 0.0
        assert result.loc["algo1"][("gd_plus", "mean")] > 0.0
        assert result.loc["algo1"][("igd_plus", "mean")] > 0.0

        assert result.loc["algo2"][("gd", "mean")] == 0.0
        assert result.loc["algo2"][("igd", "mean")] == 0.0
        assert result.loc["algo2"][("gd_plus", "mean")] == 0.0
        assert result.loc["algo2"][("igd_plus", "mean")] == 0.0


class TestSavingAlgoCompResults:
    def test_save_algo_comp_results(self, tmp_path, monkeypatch):
        df = pd.DataFrame(
            {("gd", "mean"): [0.1], ("gd", "std"): [0.01]}, index=["algo1"]
        )

        import src.experiments_data_processing.algo_comp_utils as utils_module

        monkeypatch.setattr(utils_module, "ALGO_COMPARE_FOLDER", str(tmp_path))

        output_fname = "test_results.csv"
        save_algo_comp_results(df, output_fname)

        expected_file = tmp_path / output_fname
        assert expected_file.exists()

        saved_df = pd.read_csv(expected_file)
        assert "gd_mean" in saved_df.columns
        assert "gd_std" in saved_df.columns
