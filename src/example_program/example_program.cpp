#include "GraphParser.h"
#include "Intent.h"
#include <fstream>
#include <iostream>

int main() {
    std::ifstream f("full_mesh_10.csv");
    if (!f.is_open()) {
        std::cerr << "Error: Could not open file 'full_mesh_10.csv'"
                  << std::endl;
        return 1;
    }
    auto graph = GraphParser::parseCSVToGraph(f);
    f.close();
    Intent intentGenerator;
    intentGenerator.randomizeIntent(graph);
    auto nodes = graph.getNodes();
    auto intents = intentGenerator.getIntentInNodeOrder(nodes);
    // call algorithm here
    return 0;
}
