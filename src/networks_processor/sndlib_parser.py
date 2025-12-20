import xml.etree.ElementTree as ET

import requests
from rich import print

from .networks_common import GraphEdge, save_graph_to_csv

SNDLIB_URL = "https://sndlib.put.poznan.pl/download/sndlib-networks-xml/{name}.xml"


def parse_sndlib_xml(network_name: str) -> list[GraphEdge]:
    url = SNDLIB_URL.format(name=network_name)

    print("[bold yellow]Downloading network from SNDlib...[/bold yellow]")
    response = requests.get(url)

    if response.status_code == 404:
        print(
            "[bold red]Error:[/bold red] Failed to download sndlib network. Couldn't find the network, check if you spelled network name correctly."
        )
        return None

    if response.status_code != 200:
        print(
            f"[bold red]Error:[/bold red] Failed to download sndlib network (Status: {response.status_code})"
        )
        return None

    print("[bold yellow]Parsing network...[/bold yellow]")

    # xml namespace
    ns = {"ns": "http://sndlib.zib.de/network"}

    root = ET.fromstring(response.content)
    network_structure_node = root.find("ns:networkStructure", ns)
    links_root = network_structure_node.find("ns:links", ns)

    graph = [
        GraphEdge.from_source_and_target(
            link.find("ns:source", ns).text,
            link.find("ns:target", ns).text,
        )
        for link in links_root
    ]

    return graph


def parse_and_save_sndlib(network_name: str) -> None:
    graph = parse_sndlib_xml(network_name)
    if graph is None:
        return
    result = save_graph_to_csv(graph, network_name)
    if result:
        output_path, clickable_path = result
        print(
            f"[bold green]Success:[/bold green] Sndlib graph saved in graphs folder as [link={clickable_path}]{output_path.name}[/link]"
        )
    else:
        print("Failed to saved parsed network.")
