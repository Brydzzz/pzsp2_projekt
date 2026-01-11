import os
from datetime import datetime

# folders
GRAPH_FOLDER = "graph_csv_files"
ALGO_COMPARE_FOLDER = "algo_compare"
PF_FOLDER = "pareto_fronts"


# filename generation
def generate_true_pareto_fname(graph_fname: str, intents_fname: str) -> str:
    g_name = os.path.splitext(os.path.basename(graph_fname))[0]
    i_name = os.path.splitext(os.path.basename(intents_fname))[0]

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    return f"{g_name}__{i_name}__pf__{timestamp}.csv"


def generate_alg_comp_raw_data_fname(
    graph_fname: str, intents_fname: str, iterations: int
) -> str:
    g_name = os.path.splitext(os.path.basename(graph_fname))[0]
    i_name = os.path.splitext(os.path.basename(intents_fname))[0]

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    return f"{g_name}__{i_name}__{iterations}it__alg_compare_raw__{timestamp}.csv"


def generate_alg_comp_results_fname(
    graph_fname: str, intents_fname: str, iterations: int
) -> str:
    g_name = os.path.splitext(os.path.basename(graph_fname))[0]
    i_name = os.path.splitext(os.path.basename(intents_fname))[0]

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    return f"{g_name}__{i_name}__{iterations}it__alg_compare_results__{timestamp}.csv"


def generate_alg_comp_plots_fname(
    graph_fname: str, intents_fname: str, iterations: int
) -> list[str]:
    g_name = os.path.splitext(os.path.basename(graph_fname))[0]
    i_name = os.path.splitext(os.path.basename(intents_fname))[0]

    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")

    plot_names = ["delay", "loss", "jitter", "gdmetrics"]

    return [
        f"{g_name}__{i_name}__{iterations}it__alg_compare_plot_{plot_name}__{timestamp}.csv"
        for plot_name in plot_names
    ]
