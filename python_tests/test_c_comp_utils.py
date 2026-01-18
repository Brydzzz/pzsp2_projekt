from src.experiments_processing.c_comp_utils import is_c_comp_data_format_valid
import pytest
import pandas as pd

@pytest.fixture
def valid_df():
    return pd.DataFrame({
        "node_count": [10, 20, 30],
        "algorithm": ["A", "B", "A"],
        "execution_time": [0.5, 1.2, 0.8],
        "run_id": [1, 1, 2]
    })

class TestCCompDataValidFormat:
    def test_valid_data(self, valid_df):
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is True
        assert error is None

    def test_missing_columns(self, valid_df):
        invalid_df = valid_df.drop(columns=["algorithm"])
        is_valid, error = is_c_comp_data_format_valid(invalid_df)
        assert is_valid is False
        assert error == "missing required columns"

    def test_missing_multiple_columns(self,valid_df):
        invalid_df = valid_df.drop(columns=["algorithm", "run_id"])
        is_valid, error = is_c_comp_data_format_valid(invalid_df)
        assert is_valid is False
        assert error == "missing required columns"

    def test_extra_columns_ignored(self, valid_df):
        valid_df["extra_col"] = "some data"
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is True
        assert error is None

    def test_invalid_node_count_type_float(self, valid_df):
        valid_df["node_count"] = valid_df["node_count"].astype(float)
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is False
        assert error == "non-integer data in node_count column"

    def test_invalid_node_count_type_string(self, valid_df):
        valid_df["node_count"] = ["10", "20", "30"]
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is False
        assert error == "non-integer data in node_count column"

    def test_invalid_execution_time_type(self, valid_df):
        valid_df["execution_time"] = ["0.5s", "1.2s", "0.8s"]
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is False
        assert error == "non-numeric data in execution_time column"

    def test_execution_time_int_is_valid(self, valid_df):
        valid_df["execution_time"] = [1, 2, 3]
        is_valid, error = is_c_comp_data_format_valid(valid_df)
        assert is_valid is True
        assert error is None