import subprocess

from rich import print
from rich.table import Table


def run_gen_true_pareto(graph_fname: str, intents_fname: str) -> None:
    print("[bold yellow]Generating true pareto front...[/bold yellow]")
    config_table = Table(title="Configuration")
    config_table.add_column("Parameter")
    config_table.add_column("Value")
    config_table.add_row("Graph", graph_fname)
    config_table.add_row("Intents", intents_fname)
    print(config_table)

    # TODO replace with C++ program
    result = subprocess.run(["echo", "hello from true pareto program :)"])
    if result.returncode != 0:
        print(
            f"[bold red]Error running true pareto front generation.[/bold red] Error code: {result.returncode}"
        )
    else:
        print(
            "[bold green]Success:[/bold green] true pareto front was generated. It was saved to [TODO] in 'intents' folder"
        )
