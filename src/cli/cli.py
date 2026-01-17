import argparse

from src.cli.run_gen_intents import run_gen_intents
from src.networks_processor.fullmesh_generator import (
    generate_and_save_fullmesh,
)
from src.networks_processor.sndlib_parser import parse_and_save_sndlib

from .folder_and_fnames import (
    ALGO_COMPARE_FOLDER,
    GRAPH_FOLDER,
    PF_FOLDER,
    CONV_CHECK_FOLDER,
)
from .run_algo_comp import run_algo_comp
from .run_gen_true_pareto import run_gen_true_pareto
from .run_check_conv import run_check_conv


def main():
    folder_map = f"""
    DIRECTORY MAPPING:
    The program expects files in specific subdirectories. Use FILENAMES only:
        - Graphs                            ->  /{GRAPH_FOLDER}/
        - Intents                           ->  /intents-files/
        - True Pareto                       ->  /{PF_FOLDER}/
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
        "gen-full-mesh",
        help="Generates full mesh network with given number of nodes",
    )
    parser_full_mesh.add_argument(
        "node_count", type=int, help="Number of nodes in full mesh network"
    )

    parser_gen_intents = subparsers.add_parser(
        "gen-intents", help="Generate intents for graph."
    )
    parser_gen_intents.add_argument(
        "graph", help=f"CSV file with graph. Filename in '{GRAPH_FOLDER}/'"
    )

    parser_gen_pareto = subparsers.add_parser(
        "gen-true-pareto",
        help=(
            "Generate True Pareto Front. This will run all algorithms a few times"
            " and save the results from which the non-dominated solutions will be extracted."
            " These solutions will form a front that best represented the actual Pareto front."
        ),
    )
    parser_check_conv = subparsers.add_parser(
        "check-conv",
        help=("Check the convergence for earch algorithm, based on population size"),
    )
    parser_check_conv.add_argument(
        "graph", help=f"CSV file with graph. Filename in '{GRAPH_FOLDER}/'"
    )
    parser_check_conv.add_argument(
        "intents",
        help="Intents csv file generated for given graph. Filename in 'intents-files/'",
    )
    parser_check_conv.add_argument(
        "true_pareto",
        help=f"True pareto front file generated for given graph and intents. Filename in '{PF_FOLDER}/'",
    )
    parser_check_conv.add_argument(
        "iterations",
        type=int,
        help="Number of iterations each algorithm will be run.",
    )
    parser_check_conv.add_argument(
        "runs",
        type=int,
        help="Number of runs for each algorithm",
    )
    parser_check_conv.add_argument(
        "mutation_probability",
        type=float,
        help="Mutation probability",
    )
    parser_check_conv.add_argument(
        "max_population",
        type=int,
        help="Max population size",
    )
    parser_check_conv.add_argument(
        "step",
        type=int,
        help="Step size for population size",
    )
    parser_check_conv.add_argument(
        "--plot-data",
        action="store_true",
        help="Generate convergence charts for each metric and Population Size.\n"
        f" All plots are saved to '{CONV_CHECK_FOLDER}'.",
    )
    parser_gen_pareto.add_argument(
        "graph", help=f"CSV file with graph. Filename in '{GRAPH_FOLDER}/'"
    )
    parser_gen_pareto.add_argument(
        "intents",
        help="Intents csv file generated for given graph. Filename in 'intents-files/'",
    )
    parser_gen_pareto.add_argument(
        "iterations",
        type=int,
        help="Number of iterations each algorithm will be run.",
    )
    parser_gen_pareto.add_argument(
        "runs",
        type=int,
        help="Number of runs for each algorithm",
    )
    parser_gen_pareto.add_argument(
        "mutation_probability",
        type=float,
        help="Mutation probability",
    )
    parser_algo_comp = subparsers.add_parser(
        "algo-compare",
        help=(
            "Run comparison for all algorithms (INSGA, NSGA-II, SPEA2), calculate metrics and average parameters values."
            f" The results will be saved to {ALGO_COMPARE_FOLDER}/"
        ),
    )
    parser_algo_comp.add_argument(
        "graph", help=f"CSV file with graph. Filename in '{GRAPH_FOLDER}/'"
    )
    parser_algo_comp.add_argument(
        "intents",
        help="Intents csv file generated for given graph. Filename in 'intents-files/'",
    )
    parser_algo_comp.add_argument(
        "true_pareto",
        help=f"True pareto front file generated for given graph and intents. Filename in '{PF_FOLDER}/'",
    )
    parser_algo_comp.add_argument(
        "iterations",
        type=int,
        help="Number of iterations each algorithm will be run.",
    )
    parser_algo_comp.add_argument(
        "runs",
        type=int,
        help="Number of runs for each algorithm",
    )
    parser_algo_comp.add_argument(
        "mutation_probability",
        type=float,
        help="Mutation probability",
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
        case "gen-intents":
            run_gen_intents(args.graph)
        case "gen-true-pareto":
            run_gen_true_pareto(
                args.graph,
                args.intents,
                args.iterations,
                args.runs,
                args.mutation_probability,
            )
        case "algo-compare":
            run_algo_comp(
                args.graph,
                args.intents,
                args.true_pareto,
                args.iterations,
                args.runs,
                args.mutation_probability,
                args.plot_data,
            )
        case "check-conv":
            run_check_conv(
                args.graph,
                args.intents,
                args.true_pareto,
                args.iterations,
                args.runs,
                args.mutation_probability,
                args.max_population,
                args.step,
                args.plot_data,
            )
        case _:
            parser.print_help()
