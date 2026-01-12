#include "GraphParser.h"
#include "Intent.h"
#include "Node.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0]
                  << " <graph_csv_path> <output_intent_path>" << std::endl;
        return 1;
    }

    std::string graphPath = argv[1];
    std::string outputPath = argv[2];

    std::ifstream graphFile(graphPath);
    if (!graphFile.is_open()) {
        std::cerr << "Error: Could not open graph file at " << graphPath
                  << std::endl;
        return 1;
    }

    Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphFile);
    Intent intent;
    intent.randomizeIntent(graph);


    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open intent output file at " << outputPath
                  << std::endl;
        return 1;
    }

    outFile << intent;

    return 0;
}