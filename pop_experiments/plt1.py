import os
import pandas as pd
import matplotlib.pyplot as plt

x = [round(x/100, 2) for x in range(0, 101, 5)]
y = []

GRAPH_FILE = "fm10"
INTENT_FILE = "fm10_intents"
ITERATIONS = 1000
POPULATION = 10

for mutp in x:
    fn = f"alg_compare_results__{GRAPH_FILE}__{INTENT_FILE}__{ITERATIONS}__{POPULATION}__{mutp}it_"
    os.system(f"cp ./algo_compare/{fn}_* ./algo_compare/{fn}.csv")

    v = pd.read_csv(f"./algo_compare/{fn}.csv")
    y.append(v["igd_plus_mean"])

plt.plot(x[1:], y[1:])
plt.xlabel("Mutation probability")
plt.ylabel("IGD+")
plt.title("Value of IGD+ based on mutation probability for full mesh 10")
plt.savefig(f"./plots/{GRAPH_FILE}_plot_1.png")