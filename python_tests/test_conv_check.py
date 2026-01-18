import pandas as pd
import pytest

from src.experiments_processing.conv_check_utils import (
    is_raw_data_format_valid_conv_check,
)


@pytest.fixture
def valid_conv_data():
    return pd.DataFrame(
        {
            "algo": ["NSGA2", "SPEA2"],
            "run_id": [1, 1],
            "iteration": [100, 100],
            "popsize": [50, 50],
            "loss": [10, 12],
            "delay": [100, 110],
            "jitter": [5, 6],
        }
    ).astype(
        {
            "algo": "object",
            "run_id": "int64",
            "iteration": "int64",
            "popsize": "int64",
            "loss": "int64",
            "delay": "int64",
            "jitter": "int64",
        }
    )


class TestConvValidDataFormat:
    def test_valid_data_returns_true(self, valid_conv_data):
        assert is_raw_data_format_valid_conv_check(valid_conv_data) is True

    def test_missing_column_returns_false(self, valid_conv_data):
        invalid_df = valid_conv_data.drop(columns=["popsize"])
        assert is_raw_data_format_valid_conv_check(invalid_df) is False

    def test_extra_column_returns_false(self, valid_conv_data):
        valid_conv_data["timestamp"] = 12345
        assert is_raw_data_format_valid_conv_check(valid_conv_data) is False

    def test_wrong_dtype_returns_false_float(self, valid_conv_data):
        valid_conv_data["loss"] = valid_conv_data["loss"].astype("float64")
        assert is_raw_data_format_valid_conv_check(valid_conv_data) is False

    def test_wrong_dtype_returns_false_string(self, valid_conv_data):
        valid_conv_data["run_id"] = valid_conv_data["run_id"].astype(str)
        assert is_raw_data_format_valid_conv_check(valid_conv_data) is False
