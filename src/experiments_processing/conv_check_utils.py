import pandas as pd


def is_raw_data_format_valid_conv_check(data: pd.DataFrame) -> bool:
    expected = {
        "algo",
        "run_id",
        "iteration",
        "popsize",
        "loss",
        "delay",
        "jitter",
        "run_id",
    }
    if set(data.columns) != expected:
        return False

    expected_dtypes = {
        "algo": "object",
        "run_id": "int64",
        "iteration": "int64",
        "popsize": "int64",
        "loss": "int64",
        "delay": "int64",
        "jitter": "int64",
    }
    actual_dtypes = data.dtypes.astype(str).to_dict()
    return actual_dtypes == expected_dtypes
