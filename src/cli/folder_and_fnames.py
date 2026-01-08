import os
from datetime import datetime

# folders
GRAPH_FOLDER = "graph_csv_files"
ALGO_COMPARE_FOLDER = "algo-compare-results"


# filename generation
def generate_true_pareto_fname(graph_fname: str, intents_fname: str) -> str:
    g_name = os.path.splitext(os.path.basename(graph_fname))[0]
    i_name = os.path.splitext(os.path.basename(intents_fname))[0]

    timestamp = datetime.now().strftime("%Y%m%d_%H%M")

    return f"{g_name}__{i_name}__pf__{timestamp}.csv"


def generate_alg_comp_results_fname(
    graph_fname: str, intents_fname: str, iterations: int
) -> str:
    pass


def generate_alg_comp_plots_fname(
    graph_fname: str, intents_fname: str, iterations: int
) -> tuple[str]:
    pass
