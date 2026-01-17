import argparse

from src.cli.run_c_comp import run_c_comp
from src.cli.run_gen_intents import run_gen_intents
from src.networks_processor.fullmesh_generator import (
    generate_and_save_fullmesh,
)
from src.networks_processor.sndlib_parser import parse_and_save_sndlib

from .folder_and_fnames import (
    ALGO_COMPARE_FOLDER,
    C_COMP_DATA_FOLDER,
    GRAPH_FOLDER,
    PF_FOLDER,
)
from .run_algo_comp import run_algo_comp
from .run_gen_true_pareto import run_gen_true_pareto


def main():
    folder_map = f"""
    DIRECTORY MAPPING:
    The program expects files in specific subdirectories. Use FILENAMES only:
        - Graphs                               ->  /{GRAPH_FOLDER}/
        - Intents                              ->  /intents-files/
        - True Pareto                          ->  /{PF_FOLDER}/
        - Algorithms Comparison Results        ->  /{ALGO_COMPARE_FOLDER}/
        - Computational complexity Results     ->  /{C_COMP_DATA_FOLDER}/
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
        default=None,
        help="[Gen Only] Number of iterations (Default: 1000).",
    )
    parser_c_comp.add_argument(
        "--runs",
        type=int,
        default=None,
        help="[Gen Only] Number of runs per algorithm (Default: 1).",
    )
    parser_c_comp.add_argument(
        "--mutation",
        type=float,
        default=None,
        help="[Gen Only] Mutation probability (Default: 0.1).",
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
                if (
                    args.iterations is not None
                    or args.runs is not None
                    or args.mutation is not None
                ):
                    parser.error(
                        "Arguments --iterations, --runs, and --mutation cannot be used with --load-data."
                    )

                run_c_comp(args.load_data, 0, 0, 0.0)
            else:
                iter_val = args.iterations if args.iterations is not None else 1000
                runs_val = args.runs if args.runs is not None else 1
                mut_val = args.mutation if args.mutation is not None else 0.1

                run_c_comp(None, iter_val, runs_val, mut_val)
        case _:
            parser.print_help()
