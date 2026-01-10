import argparse

from src.networks_processor.fullmesh_generator import generate_and_save_fullmesh
from src.networks_processor.sndlib_parser import parse_and_save_sndlib

from .folder_and_fnames import ALGO_COMPARE_FOLDER, GRAPH_FOLDER
from .run_algo_comp import run_algo_comp
from .run_gen_true_pareto import run_gen_true_pareto


def main():
    folder_map = f"""
    DIRECTORY MAPPING:
    The program expects files in specific subdirectories. Use FILENAMES only:
        - Graphs                            ->  /{GRAPH_FOLDER}/
        - Intents                           ->  /intents-files/
        - True Pareto                       ->  /pareto-fronts/
        - Algorithms Comparison Results     ->  /{ALGO_COMPARE_FOLDER}/
        - ... (more coming soon)
    """
    parser = argparse.ArgumentParser(
        description="Tool for multi-objective telecommunications network optimization",
        formatter_class=argparse.RawTextHelpFormatter,
        epilog=folder_map,
    )
    subparsers = parser.add_subparsers(
        title="subcommands",
        description="Available subcommands",
        dest="command",
        help="Only one subcommand can be used at once",
    )

    parser_snd = subparsers.add_parser(
        "parse-sndlib",
        help=(
            "Download SNDLib network and parse it. The program will generate loss, delay, jitter and throughput parameters and"
            " save modified network to CSV format that represents a graph."
            f" The CSV file will be saved to {GRAPH_FOLDER}/ and can be later used with other tools."
        ),
    )
    parser_snd.add_argument("network_name", type=str, help="Network name from SNDLib")

    parser_full_mesh = subparsers.add_parser(
        "gen-full-mesh", help="Generates full mesh network with given number of nodes"
    )
    parser_full_mesh.add_argument(
        "node_count", type=int, help="Number of nodes in full mesh network"
    )

    parser_gen_pareto = subparsers.add_parser(
        "gen-true-pareto",
        help=(
            "Generate True Pareto Front. This will run all algorithms a few times"
            " and save the results from which the non-dominated solutions will be extracted."
            " These solutions will form a front that best represented the actual Pareto front."
        ),
    )
    parser_gen_pareto.add_argument(
        "graph", help="CSV file with graph. Filename in 'graph-files/'"
    )
    parser_gen_pareto.add_argument(
        "intents",
        help="Intents csv file generated for given graph. Filename in 'intents-files/'",
    )

    parser_algo_comp = subparsers.add_parser(
        "algo-compare",
        help=(
            "Run comparison for all algorithms (INSGA, NSGA-II, SPEA2), calculate metrics and average parameters values."
            f" The results will be saved to {ALGO_COMPARE_FOLDER}/"
        ),
    )
    parser_algo_comp.add_argument(
        "graph", help="CSV file with graph. Filename in 'graph-files/'"
    )
    parser_algo_comp.add_argument(
        "intents",
        help="Intents csv file generated for given graph. Filename in 'intents-files/'",
    )
    parser_algo_comp.add_argument(
        "true_pareto",
        help="True pareto front file generated for given graph and intents. Filename in 'pareto-fronts/'",
    )
    parser_algo_comp.add_argument(
        "iterations", type=int, help="Number of iterations each algorithm will be run."
    )

    parser_algo_comp.add_argument(
        "--plot-data",
        action="store_true",
        help="Generate & show 4 bar charts (Loss, Delay, Jitter, Metrics).\n"
        f" All plots are saved to '{ALGO_COMPARE_FOLDER}'.",
    )

    args = parser.parse_args()

    match args.command:
        case "parse-sndlib":
            parse_and_save_sndlib(args.network_name)
        case "gen-full-mesh":
            generate_and_save_fullmesh(args.node_count)
        case "gen-true-pareto":
            run_gen_true_pareto(args.graph, args.intents)
        case "algo-compare":
            run_algo_comp(
                args.graph,
                args.intents,
                args.true_pareto,
                args.iterations,
                args.plot_data,
            )
        case _:
            parser.print_help()
