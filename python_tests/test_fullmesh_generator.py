from pathlib import Path

import pytest

from src.networks_processor.fullmesh_generator import (
    generate_and_save_fullmesh,
    generate_full_mesh,
)
from src.networks_processor.networks_common import (
    DELAY_RANGE,
    GRAPH_FOLDER,
    JITTER_RANGE,
    LOSS_RANGE,
    THROUGHPUT_RANGE,
    GraphEdge,
)


class TestGenerateFullMesh:
    @pytest.mark.parametrize(
        "node_count",
        [4, 10, 15, 35, 50, 55],
    )
    def test_generates_correct_number_of_edges(self, node_count):
        graph = generate_full_mesh(node_count)

        expected_edges = node_count * (node_count - 1) // 2
        assert len(graph) == expected_edges

    def test_single_node_throws_value_error(self):
        with pytest.raises(ValueError) as excinfo:
            generate_full_mesh(1)

        assert str(excinfo.value) == "node_count has to be greater than or equal to 2"

    def test_zero_nodes_throws_value_error(self):
        with pytest.raises(ValueError) as excinfo:
            generate_full_mesh(0)

        assert str(excinfo.value) == "node_count has to be greater than or equal to 2"

    def test_two_nodes_creates_one_edge(self):
        graph = generate_full_mesh(2)

        assert len(graph) == 1
        assert graph[0].source == "0"
        assert graph[0].target == "1"

    def test_no_self_loops(self):
        graph = generate_full_mesh(5)

        for edge in graph:
            assert edge.source != edge.target

    def test_no_duplicate_edges(self):
        graph = generate_full_mesh(4)

        edge_pairs = {(edge.source, edge.target) for edge in graph}
        reversed_pairs = {(edge.target, edge.source) for edge in graph}

        assert len(edge_pairs.intersection(reversed_pairs)) == 0

    def test_edges_have_valid_parameters(self):
        graph = generate_full_mesh(3)

        for edge in graph:
            assert isinstance(edge, GraphEdge)
            assert THROUGHPUT_RANGE[0] <= edge.throughput <= THROUGHPUT_RANGE[1]
            assert LOSS_RANGE[0] <= edge.loss <= LOSS_RANGE[1]
            assert DELAY_RANGE[0] <= edge.delay <= DELAY_RANGE[1]
            assert JITTER_RANGE[0] <= edge.jitter <= JITTER_RANGE[1]

    def test_correct_node_connections(self):
        graph = generate_full_mesh(3)
        expected_connections = {("0", "1"), ("0", "2"), ("1", "2")}
        actual_connections = {(edge.source, edge.target) for edge in graph}

        assert actual_connections == expected_connections


class TestGenerateAndSaveFullmesh:
    def test_prints_success_message_when_save_succeeds(self, monkeypatch, capsys):
        def mock_save(graph, network_name):
            return (
                Path(f"{GRAPH_FOLDER}/full_mesh_13.csv"),
                "file:///home/spongebob/pzsp2-projekt/src/graph_csv_files/full_mesh_13.csv",
            )

        monkeypatch.setattr(
            "src.networks_processor.fullmesh_generator.save_graph_to_csv",
            mock_save,
        )

        generate_and_save_fullmesh(13)
        captured = capsys.readouterr()
        output = captured.out

        assert "Generating network" in output
        assert "Success" in output
        assert "full_mesh_13.csv" in output

    def test_handles_save_failure(self, monkeypatch, capsys):
        def mock_save(graph, network_name):
            return None

        monkeypatch.setattr(
            "src.networks_processor.fullmesh_generator.save_graph_to_csv",
            mock_save,
        )

        generate_and_save_fullmesh(3)

        captured = capsys.readouterr()
        output = captured.out

        assert "Generating network" in output
        assert "Graph failed to save." in output
