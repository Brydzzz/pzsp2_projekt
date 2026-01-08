from datetime import datetime
from src.cli.folder_and_fnames import generate_true_pareto_fname

class TestFilenamesGeneration:
    class MockDatetime:
        @classmethod
        def now(cls):
            return datetime(2026, 1, 8, 20, 45)

    def test_generate_true_pareto_front_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        result = generate_true_pareto_fname(g_fname, i_fname)
        expected = "poland_big__poland_big_intent_10__pf__20260108_2045.csv"
        assert result == expected