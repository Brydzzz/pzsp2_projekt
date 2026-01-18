import pandas as pd


def is_c_comp_data_format_valid(df: pd.DataFrame) -> tuple[bool, str | None]:
    required_columns = {"node_count", "algorithm", "execution_time", "run_id"}
    if not required_columns.issubset(df.columns):
        return False, "missing required columns"

    if not pd.api.types.is_integer_dtype(df["node_count"]):
        return False, "non-integer data in node_count column"

    if not pd.api.types.is_numeric_dtype(df["execution_time"]):
        return False, "non-numeric data in execution_time column"

    return True, None
