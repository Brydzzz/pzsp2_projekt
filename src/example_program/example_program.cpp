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
    auto generate_random_paths =
        [&](int pop_num) -> std::vector<std::vector<Node>> {
        std::vector<std::vector<Node>> paths;
        for (auto intent : intentGenerator.intents) {
            paths.push_back(graph.generateRandomPath(intent.first.first, intent.first.second, intent.second));
        }
        return paths;
    };
    auto cross_over_ =
    [&](std::vector<std::vector<Node>> , std::vector<float> params){
        
    }

    // call algorithm here
    return 0;
}
