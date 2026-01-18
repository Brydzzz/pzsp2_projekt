from datetime import datetime

from src.cli.folder_and_fnames import (
    generate_alg_comp_plots_fname,
    generate_alg_comp_raw_data_fname,
    generate_alg_comp_results_fname,
    generate_c_comp_fname,
    generate_c_comp_plot_fname,
    generate_true_pareto_fname,
)


class TestFilenamesGeneration:
    class MockDatetime:
        @classmethod
        def now(cls):
            return datetime(2026, 1, 8, 20, 45, 31)

    def test_generate_true_pareto_front_fname(self, monkeypatch):
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        result = generate_true_pareto_fname(g_fname, i_fname)
        expected = "poland_big__poland_big_intent_10__pf.csv"
        assert result == expected

    def test_generate_alg_comp_raw_data_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        iters = 25
        runs = 100
        mut_prob = 0.5
        results = generate_alg_comp_raw_data_fname(
            g_fname, i_fname, iters, runs, mut_prob
        )
        expected = "poland_big__poland_big_intent_10__25__100__0.5_it__alg_compare_raw__20260108_204531.csv"
        assert results == expected

    def test_generate_alg_comp_results_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        g_fname = "poland_big"
        i_fname = "poland_big_intent_10"
        iters = 25
        runs = 100
        mut_prob = 0.5
        results = generate_alg_comp_results_fname(
            g_fname, i_fname, iters, runs, mut_prob
        )
        expected = "alg_compare_results__poland_big__poland_big_intent_10__25__100__0.5it__20260108_204531.csv"
        assert results == expected

    def test_generate_alg_comp_plots_fname(self):
        results_fname = (
            "alg_compare_results__poland__poland_intent_10__25it__20260108_204531.csv"
        )
        results = generate_alg_comp_plots_fname(results_fname)
        expected = [
            "alg_compare_plot_objectives__poland__poland_intent_10__25it__20260108_204531.png",
            "alg_compare_plot_gdmetrics__poland__poland_intent_10__25it__20260108_204531.png",
        ]
        assert results == expected

    def test_generate_c_comp_fname(self, monkeypatch):
        monkeypatch.setattr("src.cli.folder_and_fnames.datetime", self.MockDatetime)
        results = generate_c_comp_fname(10, 2, 0.5)
        expected = "c_comp_results__10it__2runs__0.5mut__20260108_204531.csv"
        assert results == expected

    def test_generate_c_comp_plot_fname(results_fname: str) -> str:
        results_fname = "c_comp_results__10it__2runs__0.5mut__20260108_204531.csv"
        results = generate_c_comp_plot_fname(results_fname)
        excepted = "c_comp_plot__10it__2runs__0.5mut__20260108_204531.png"
        assert results == excepted
