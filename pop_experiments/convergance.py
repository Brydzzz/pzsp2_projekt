import os

"""
change NODE_COUNT to change the size of full mesh graph
"""
NODE_COUNT = 50
MIN_CORE = 0
MAX_CORE = 4

"""
change network names to test different networks
"""
NETWORK_NAME = f"fm{NODE_COUNT}.csv"
INTENT_NAME = f"fm{NODE_COUNT}_intents.csv"
TRUE_PF = f"fm{NODE_COUNT}_pf.csv"

for x in range(0, 101, 5):
    xp = x / 100
    os.system(
        f"taskset -c {MIN_CORE}-{MAX_CORE} python -m src.run algo-compare {NETWORK_NAME} {INTENT_NAME} {TRUE_PF} 400 10 {xp} --plot-data"
    )
