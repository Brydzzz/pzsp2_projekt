from .networks_common import save_graph_to_csv, generate_edge_params, GraphEdge
from rich import print

def generate_full_mesh(node_count: int) -> list[GraphEdge]:
    graph = []
    for source in range(node_count):
        for target in range(source+1, node_count):
            edge_params = generate_edge_params()
            graph.append(GraphEdge(str(source), str(target), *edge_params))
    return graph

def generate_and_save_fullmesh(node_count: int) -> None:
    graph = generate_full_mesh(node_count)
    network_name = f"full_mesh_{node_count}"
    output_fname = save_graph_to_csv(graph, network_name)
    if output_fname:
        print(f"[bold green]Success:[/bold green] Full mesh graph saved in graphs folder as {output_fname}")