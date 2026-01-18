import argparse

from rich import print

from src.cli.run_c_comp import run_c_comp
from src.cli.run_gen_intents import run_gen_intents
from src.networks_processor.fullmesh_generator import (
    generate_and_save_fullmesh,
)
from src.networks_processor.sndlib_parser import parse_and_save_sndlib

from .folder_and_fnames import (
    ALGO_COMPARE_FOLDER,
    C_COMP_DATA_FOLDER,
    CONV_CHECK_FOLDER,
    GRAPH_FOLDER,
    INTENTS_FOLDER,
    PF_FOLDER,
)
from .run_algo_comp import run_algo_comp
from .run_check_conv import run_check_conv
from .run_gen_true_pareto import run_gen_true_pareto


def main():
    folder_map = f"""
    DIRECTORY MAPPING:
    The program expects files in specific subdirectories. Use FILENAMES only:
        - Graphs                               ->  /{GRAPH_FOLDER}/
        - Intents                              ->  /{INTENTS_FOLDER}/
        - True Pareto                          ->  /{PF_FOLDER}/
        - Algorithms Comparison Results        ->  /{ALGO_COMPARE_FOLDER}/
        - Computational complexity Results     ->  /{C_COMP_DATA_FOLDER}/
        - Convergence experiment results       ->  /{CONV_CHECK_FOLDER}/
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
    parser_check_conv = subparsers.add_parser(
        "check-conv",
        help=("Check the convergence for earch algorithm, based on population size"),
    )
    parser_check_conv.add_argument(
        "graph", help=f"CSV file with graph. Filename in '{GRAPH_FOLDER}/'"
    )
    parser_check_conv.add_argument(
        "intents",
        help=f"Intents csv file generated for given graph. Filename in '{INTENTS_FOLDER}/'",
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
        help=f"Intents csv file generated for given graph. Filename in '{INTENTS_FOLDER}/'",
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

    parser_c_comp = subparsers.add_parser(
        "c-comp",
        help=(
            "Run computational complexity tests to measure execution time vs network size."
            " Automatically generates fullmesh networks (small/medium/large) if no data is loaded."
        ),
    )
    parser_c_comp.add_argument(
        "--load-data",
        type=str,
        metavar="FILENAME",
        help=f"Load existing measurement data from '{C_COMP_DATA_FOLDER}/' instead of generating new.",
    )

    parser_c_comp.add_argument(
        "--iterations",
        type=int,
        default=1000,
        help="[Data Generation Only] Number of iterations (Default: 1000).",
    )
    parser_c_comp.add_argument(
        "--runs",
        type=int,
        default=1,
        help="[Data Generation Only] Number of runs per algorithm (Default: 1).",
    )
    parser_c_comp.add_argument(
        "--mutation",
        type=float,
        default=0.1,
        help="[Data Generation Only] Mutation probability (Default: 0.1).",
    )

    parser_c_comp.add_argument(
        "--min_nodes",
        type=int,
        default=5,
        help="[Data Generation Only] Min number of nodes in network (Default: 5).",
    )

    parser_c_comp.add_argument(
        "--max_nodes",
        type=int,
        default=40,
        help="[Data Generation Only] Max number of nodes in network, has to be bigger than min_nodes (Default: 40).",
    )

    parser_c_comp.add_argument(
        "--step",
        type=int,
        default=5,
        help=(
            "[Data Generation Only] Step for nodes count generation "
            "nodes_counts=range(min_nodes, max_nodes+1, step) (Default: 5)."
        ),
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
        case "c-comp":
            if args.load_data:
                print(
                    "[bold blue]INFO: Arguments --iterations, --runs, --mutation, --min_nodes, "
                    "--max_nodes and --step will be ignored for --load-data.[/bold blue]"
                )
            else:
                if args.min_nodes > args.max_nodes:
                    parser.error("Max nodes has to be bigger than min nodes")

            run_c_comp(
                None,
                args.iterations,
                args.runs,
                args.mutation,
                args.min_nodes,
                args.max_nodes,
                args.step,
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
