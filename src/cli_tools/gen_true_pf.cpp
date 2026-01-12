#include "GraphParser.h"
#include "Intent.h"
#include "Node.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc < 4) {
        std::cerr << "Usage: " << argv[0]
                  << " <graph_csv_path> <intent_csv_path> <output_pf_path"
                  << std::endl;
        return 1;
    }

    std::string graphPath = argv[1];
    std::string intentsPath = argv[2];
    std::string outputPath = argv[3];

    std::ifstream graphFile(graphPath);
    if (!graphFile.is_open()) {
        std::cerr << "Error: Could not open graph file at " << graphPath
                  << std::endl;
        return 1;
    }
    Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphFile);
    graphFile.close();

    Intent intent;
    std::ifstream intentsFile(intentsPath);
    if (!intentsFile.is_open()) {
        std::cerr << "Error: Could not open intent file at " << outputPath
                  << std::endl;
        return 1;
    }
    intentsFile >> intent;
    intentsFile.close();

    std::cout << "Pareto generating logic goes brrrrrrrrrr" << std::endl;

    return 0;
}