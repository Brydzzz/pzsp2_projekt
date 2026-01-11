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

    return f"{g_name}__{i_name}__pf.csv"


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

    return f"alg_compare_results__{g_name}__{i_name}__{iterations}it__{timestamp}.csv"


def generate_alg_comp_plots_fname(results_fname) -> list[str]:
    plot_names = ["objectives", "gdmetrics"]

    return [
        f"{results_fname.replace('alg_compare_results', f'alg_compare_plot_{plot_name}').replace('.csv', '.png')}"
        for plot_name in plot_names
    ]
