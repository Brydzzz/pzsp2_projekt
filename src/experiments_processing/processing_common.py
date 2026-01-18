import os

from rich import print
from rich.table import Table


def print_config_table(params_values: dict[str, str]):
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    for param, val in params_values.items():
        config_table.add_row(param, val)
    print(config_table)


def executable_exists(exe_path: str) -> bool:
    return os.path.exists(exe_path)
