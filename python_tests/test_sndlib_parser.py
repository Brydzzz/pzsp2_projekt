from pathlib import Path

import pytest
import requests

from src.networks_processor.networks_common import GRAPH_FOLDER, GraphEdge
from src.networks_processor.sndlib_parser import (
    parse_and_save_sndlib,
    parse_sndlib_xml,
)

TEST_DATA_DIR = Path(__file__).parent / "test_data"


class TestParseSndlibXml:
    @pytest.fixture
    def correct_network_name(self) -> str:
        return "polska"

    @pytest.fixture
    def incorrect_network_name(self) -> str:
        return "janos_us"

    def test_parse_success(self, monkeypatch, correct_network_name, capsys):
        xml_path = TEST_DATA_DIR / "test_network.xml"
        with open(xml_path, "rb") as f:
            binary_xml_content = f.read()

        class MockResponse:
            def __init__(self):
                self.status_code = 200
                self.content = binary_xml_content

        def mock_get(*args, **kwargs):
            return MockResponse()

        monkeypatch.setattr(requests, "get", mock_get)

        graph = parse_sndlib_xml(correct_network_name)

        captured = capsys.readouterr()
        assert "Downloading network from SNDlib..." in captured.out
        assert "Parsing network..." in captured.out

        expected_connections = {
            ("Gdansk", "Warsaw"),
            ("Gdansk", "Kolobrzeg"),
            ("Bydgoszcz", "Kolobrzeg"),
            ("Bydgoszcz", "Poznan"),
            ("Bydgoszcz", "Warsaw"),
            ("Kolobrzeg", "Szczecin"),
            ("Katowice", "Krakow"),
            ("Katowice", "Lodz"),
            ("Katowice", "Wroclaw"),
            ("Krakow", "Rzeszow"),
            ("Krakow", "Warsaw"),
            ("Bialystok", "Rzeszow"),
            ("Bialystok", "Warsaw"),
            ("Lodz", "Warsaw"),
            ("Lodz", "Wroclaw"),
            ("Poznan", "Szczecin"),
            ("Poznan", "Wroclaw"),
            ("Gdansk", "Bialystok"),
        }
        actual_connections = {(edge.source, edge.target) for edge in graph}
        assert expected_connections == actual_connections

    def test_network_name_doesnt_exist(
        self, monkeypatch, incorrect_network_name, capsys
    ):
        class MockResponse:
            def __init__(self):
                self.status_code = 404
                self.content = ""

        def mock_get(*args, **kwargs):
            return MockResponse()

        monkeypatch.setattr(requests, "get", mock_get)

        graph = parse_sndlib_xml(incorrect_network_name)

        assert graph is None

        captured = capsys.readouterr()
        assert "Downloading network from SNDlib..." in captured.out
        assert (
            "Couldn't find the network, check if you spelled network name correctly."
            in captured.out.replace("\n", "")
        )

    def test_network_download_failed(
        self, monkeypatch, capsys, incorrect_network_name
    ):
        class MockResponse:
            def __init__(self):
                self.status_code = 500
                self.content = ""

        def mock_get(*args, **kwargs):
            return MockResponse()

        monkeypatch.setattr(requests, "get", mock_get)

        graph = parse_sndlib_xml(incorrect_network_name)

        assert graph is None

        captured = capsys.readouterr()
        assert "Downloading network from SNDlib..." in captured.out
        assert "(Status: " in captured.out


class TestParseAndSaveSndlib:
    @pytest.fixture
    def network_name(self) -> str:
        return "janos-us-ca"

    def test_prints_success_message_when_save_succeeds(
        self, monkeypatch, capsys, network_name
    ):
        def mock_save(graph, net_name):
            return (
                Path(f"{GRAPH_FOLDER}/{network_name}.csv"),
                f"file:///home/spongebob/pzsp2-projekt/src/graph_csv_files/{network_name}.csv",
            )

        def mock_parse_sndlib(net_name):
            return [GraphEdge.from_source_and_target("0", "1")]

        monkeypatch.setattr(
            "src.networks_processor.sndlib_parser.save_graph_to_csv",
            mock_save,
        )

        monkeypatch.setattr(
            "src.networks_processor.sndlib_parser.parse_sndlib_xml",
            mock_parse_sndlib,
        )

        parse_and_save_sndlib(network_name)
        captured = capsys.readouterr()
        output = captured.out

        assert "Success" in output
        assert "Sndlib graph saved in graphs folder as" in output
        assert f"{network_name}.csv" in output

    def test_handles_save_failure(
        self, monkeypatch, capsys, network_name
    ):
        def mock_save(graph, net_name):
            return None

        def mock_parse_sndlib(net_name):
            return [GraphEdge.from_source_and_target("0", "1")]

        monkeypatch.setattr(
            "src.networks_processor.sndlib_parser.save_graph_to_csv",
            mock_save,
        )

        monkeypatch.setattr(
            "src.networks_processor.sndlib_parser.parse_sndlib_xml",
            mock_parse_sndlib,
        )

        parse_and_save_sndlib(network_name)

        captured = capsys.readouterr()
        output = captured.out

        assert "Failed to saved parsed network." in output

    def test_handles_parse_failure(
        self, monkeypatch, capsys, network_name
    ):

        def mock_parse_sndlib(net_name):
            return None

        monkeypatch.setattr(
            "src.networks_processor.sndlib_parser.parse_sndlib_xml",
            mock_parse_sndlib,
        )

        parse_and_save_sndlib(network_name)

        captured = capsys.readouterr()
        output = captured.out

        assert len(output) == 0
