import os
from datetime import datetime


"""
change NODE_COUNT to change the size of full mesh graph
"""
NODE_COUNT = 50
MIN_CORE = 0
MAX_CORE = 4


os.system(f"python -m src.run gen-full-mesh {NODE_COUNT}")
os.system(f"mv graph_csv_files/full_mesh_{NODE_COUNT}.csv graph_csv_files/fm{NODE_COUNT}.csv")
os.system(f"python -m src.run gen-intents fm{NODE_COUNT}.csv")
timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
os.system(f"mv ./intents_csv_files/fm{NODE_COUNT}__intents__{timestamp}.csv ./intents_csv_files/fm{NODE_COUNT}_intents.csv")
os.system(f"taskset -c {MIN_CORE}-{MAX_CORE} python -m src.run gen-true-pareto fm{NODE_COUNT}.csv fm{NODE_COUNT}_intents.csv 400 1 0.1")
os.system(f"mv ./pareto_fronts/fm{NODE_COUNT}__fm{NODE_COUNT}_intents__pf.csv ./pareto_fronts/fm{NODE_COUNT}_pf.csv")