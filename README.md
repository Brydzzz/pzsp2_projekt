# PZSP2 project
This is a repo for PZSP2 project.

## Table of contents
1. [Requirements](./documentation/requirements.md)
2. [Branch Structure](./documentation/branch-structure.md)

## How to run CLI client?

Create python environment with required libraries:
```bash
python -m venv .venv/
source .venv/bin/activate
pip install -r requirements.txt
```

Open project folder and run `run.py` as module:
```bash
cd <project folder>
python -m src.run [subcommand] [subcommand options]
```