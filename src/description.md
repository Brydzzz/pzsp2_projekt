# This is a file containing the description of how to move through the project

- `documentation/`
  - `Architecture` - diagrams for architecture expert
  - `Software` - notes for software expert
  - `Notes` - notes from meetings with project owner
  - `branch-structure.md` - description of branch structure used in project
  - `requirements.md` - requirements for running the project
- `src/`
  - `algorithms` - implementation of INSGA, NSGA2 and SPEA2 algorithms
  - `cli` - cli program files
  - `cli_tools` - c++ programs that generate data for experiments
  - `experiment_processing` - processing data saved by cli_tools
  - `graph` - Graph related classes
  - `intent` - Intent class
  - `networks_processor` - parsing sndlib networks, full-mesh generation, graph parameters generation
  - `utils` - c++ utils for randomization
  - `run.py` - main CLI client script
- `python_tests/` - pytest tests for code written in Python
- `tests/` - tests for C++ code, can be build with `CMakeLists.txt` in this folder