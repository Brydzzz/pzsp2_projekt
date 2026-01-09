import subprocess

from rich import print
from rich.table import Table


def run_algo_comp(
    graph_fname: str,
    intents_fname: str,
    true_pareto_fname: str,
    iterations: int,
    plot_data: bool,
) -> None:
    print("[bold yellow]Running algorithm comparison...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    config_table.add_row("Intents", intents_fname)
    config_table.add_row("True Pareto Front", true_pareto_fname)
    config_table.add_row("Number of iterations", str(iterations))
    config_table.add_row("Plot data", str(plot_data))
    print(config_table)

    # TODO replace with C++ program
    result = subprocess.run(["echo", "hello from algorithm comparison program :)"])
    if result.returncode != 0:
        print(
            f"[bold red]Error while comparing algorithms.[/bold red] Error code: {result.returncode}"
        )
    else:
        print(
            "[bold green]Success:[/bold green] compare algorithms experiment has finished. "
            "Results were saved to [TODO] in [TODO] folder"
        )

    # data will be read from file here

    # calculate metrics and average values of optimized parameters

    # save processed results to file

    # plot data here if plot_data True


def create_plots():
    pass
