import argparse

from networks_processor.sndlib_parser import parse_and_save_sndlib


def bubu(a):
    print(a)


def main():
    parser = argparse.ArgumentParser(
        description="Tool for multi-objective telecommunications network optimization",
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
            " save modified network to CSV format that represents a graph.\n"
            " The CSV file will be saved to graphs folder and can be later used with other tools."
        ),
    )
    parser_snd.add_argument("network_name", type=str, help="Network name from SNDLib")

    parser_full_mesh = subparsers.add_parser(
        "gen-full-mesh", help="Generates full mesh network with given number of nodes"
    )
    parser_full_mesh.add_argument(
        "node_count", type=int, help="Number of nodes in full mesh network"
    )

    args = parser.parse_args()

    match args.command:
        case "parse-sndlib":
            parse_and_save_sndlib(args.network_name)
        case "gen-full-mesh":
            pass  # TODO
        case _:
            parser.print_help()
