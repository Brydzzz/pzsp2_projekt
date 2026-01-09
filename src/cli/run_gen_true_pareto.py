import subprocess

from rich import print


def run_gen_true_pareto(graph_fname: str, intents_fname: str) -> None:
    print("[bold yellow]Generating true pareto front...[/bold yellow]")
    result = subprocess.run(["echo", "hello from true pareto program :)"])
    if result.returncode != 0:
        print(
            f"[bold red]Error running true pareto front generation.[/bold red] Error code: {result.returncode}"
        )
    else:
        print(
            "[bold green]Success:[/bold green] true pareto front was generated. It was saved to [TODO] in 'intents' folder"
        )
