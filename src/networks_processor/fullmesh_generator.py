from rich import print

from .networks_common import GraphEdge, save_graph_to_csv


def generate_full_mesh(node_count: int) -> list[GraphEdge]:
    if node_count < 2:
        raise ValueError("node_count has to be greater than or equal to 2")
    graph = []
    for source in range(node_count):
        edges = [
            GraphEdge.from_source_and_target(str(source), str(target))
            for target in range(source + 1, node_count)
        ]
        graph.extend(edges)
    return graph


def generate_and_save_fullmesh(node_count: int, network_name: str | None = None) -> None:
    print("[bold yellow]Generating network...[/bold yellow]")
    graph = generate_full_mesh(node_count)
    network_name = f"full_mesh_{node_count}" if (not network_name) else network_name
    result = save_graph_to_csv(graph, network_name)
    if result:
        output_path, clickable_path = result
        print(
            f"[bold green]Success:[/bold green] Full mesh graph saved in graphs folder as [link={clickable_path}]{output_path.name}[/link]"
        )
    else:
        print("Graph failed to save.")
