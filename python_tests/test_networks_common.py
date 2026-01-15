import csv
from pathlib import Path

from src.networks_processor.constants import (
    DELAY_RANGE,
    JITTER_RANGE,
    LOSS_RANGE,
    THROUGHPUT_RANGE,
)
from src.networks_processor.networks_common import (
    GraphEdge,
    save_graph_to_csv,
)


class TestGraphEdge:
    def test_create_from_source_and_target(self):
        source = "node1"
        target = "node2"
        edge = GraphEdge.from_source_and_target(source, target)

        assert isinstance(edge, GraphEdge)
        assert edge.source == source
        assert edge.target == target
        assert THROUGHPUT_RANGE[0] <= edge.throughput <= THROUGHPUT_RANGE[1]
        assert LOSS_RANGE[0] <= edge.loss <= LOSS_RANGE[1]
        assert DELAY_RANGE[0] <= edge.delay <= DELAY_RANGE[1]
        assert JITTER_RANGE[0] <= edge.jitter <= JITTER_RANGE[1]

    def test_to_csv_row(self):
        source = "node1"
        target = "node2"
        edge = GraphEdge.from_source_and_target(source, target)
        row = edge.to_csv_row()
        assert row[0] == edge.source
        assert row[1] == edge.target
        assert row[2] == edge.throughput
        assert row[3] == edge.loss
        assert row[4] == edge.delay
        assert row[5] == edge.jitter


class TestSaveGraphToCSV:
    def test_save_graph_success(self, tmp_path, monkeypatch):
        def mock_clickable_path(path):
            return "file://mock/link"

        monkeypatch.setattr(
            "src.networks_processor.networks_common.get_clickable_path",
            mock_clickable_path,
        )

        sample_graph = [
            GraphEdge("1", "2", 10, 100, 100, 0),
            GraphEdge("0", "2", 15, 101, 101, 1),
            GraphEdge("0", "1", 20, 102, 102, 2),
        ]
        result = save_graph_to_csv(sample_graph, "sample_graph", output_dir=tmp_path)
        assert result is not None
        output_path, clickable_path = result

        assert output_path == tmp_path / "sample_graph.csv"
        assert clickable_path == "file://mock/link"
        assert output_path.exists()

        with open(output_path, "r", encoding="utf-8") as csvfile:
            reader = csv.reader(csvfile)
            rows = list(reader)

            assert len(rows) == 4
            assert rows[0] == [
                "Source",
                "Target",
                "Throughput",
                "Loss",
                "Delay",
                "Jitter",
            ]
            assert rows[1] == ["1", "2", "10", "100", "100", "0"]
            assert rows[2] == ["0", "2", "15", "101", "101", "1"]
            assert rows[3] == ["0", "1", "20", "102", "102", "2"]

    def test_return_none_on_failure(self, tmp_path, monkeypatch, capsys):
        def mock_open_fail(*args, **kwargs):
            raise OSError("open() failure")

        monkeypatch.setattr(Path, "open", mock_open_fail)

        sample_graph = [
            GraphEdge("1", "2", 10, 100, 100, 0),
        ]
        result = save_graph_to_csv(sample_graph, "sample_graph", output_dir=tmp_path)

        assert result is None
        captured = capsys.readouterr()
        assert "Error saving graph:" in captured.out
        assert "open() failure" in captured.out
