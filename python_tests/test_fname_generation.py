from datetime import datetime

from src.cli.folder_and_fnames import (
    generate_alg_comp_plots_fname,
    generate_alg_comp_results_fname,
    generate_true_pareto_fname,
)


class TestFilenamesGeneration:
    class MockDatetime:
        @classmethod
        def now(cls):
            return datetime(2026, 1, 8, 20, 45, 31)

    def test_generate_true_pareto_front_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        result = generate_true_pareto_fname(g_fname, i_fname)
        expected = "poland_big__poland_big_intent_10__pf__20260108_204531.csv"
        assert result == expected

    def test_generate_alg_comp_results_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        iters = 25
        results = generate_alg_comp_results_fname(g_fname, i_fname, iters)
        expected = (
            "poland_big__poland_big_intent_10__25it__alg_compare__20260108_204531.csv"
        )
        assert results == expected

    def test_generate_alg_comp_plots_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland"
        i_fname = "poland_intent_10"
        iters = 25
        results = generate_alg_comp_plots_fname(g_fname, i_fname, iters)
        expected = [
            "poland__poland_intent_10__25it__alg_compare_plot_delay__20260108_204531.csv",
            "poland__poland_intent_10__25it__alg_compare_plot_loss__20260108_204531.csv",
            "poland__poland_intent_10__25it__alg_compare_plot_jitter__20260108_204531.csv",
            "poland__poland_intent_10__25it__alg_compare_plot_gdmetrics__20260108_204531.csv",
        ]
        assert results == expected
