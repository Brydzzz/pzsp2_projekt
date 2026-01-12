#include "GraphParser.h"
#include "Intent.h"
#include "SPEA2.h"
#include <fstream>
#include <iostream>
class Individual {
  public:
    std::vector<std::vector<Node>> paths;
    Graph<NetStat> graph;
    Individual(std::vector<std::vector<Node>> &paths, Graph<NetStat> graph)
        : paths(paths), graph(graph) {}
};

template <typename T>
std::pair<bool, Graph<T>> paths_correct(Individual paths,
                                        Intent intentGenerator) {
    Graph<NetStat> checkGraph = paths.graph;
    for (auto path : paths.paths) {
        std::vector<Edge<T>> edges;
        for (unsigned int i = 0; i < path.size() - 1; i++) {
            auto edge = checkGraph.getEdgeBetween(path[i], path[i + 1]);

            if (!edge.has_value()) {
                return {false, paths.graph};
            }

            if (checkGraph.flow_left[*edge] <
                static_cast<int>(intentGenerator.getDemand(
                    path[0], path[path.size() - 1]))) {
                return {false, paths.graph};
            }
            checkGraph.flow_left[*edge] =
                (*edge).toInt() -
                intentGenerator.getDemand(path[0], path[path.size() - 1]);
        }
    }
    return {true, checkGraph};
}

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
    auto generate_random_path = [&]() -> Individual {
        std::vector<std::vector<Node>> paths;
        Individual indiv(paths, graph);
        for (auto intent : intentGenerator.intents) {
            Node startNode = intent.first.first;
            Node endNode = intent.first.second;
            indiv.paths.push_back(indiv.graph.generateRandomPath(
                startNode, endNode, intent.second));
        }
        return indiv;
    };
    auto generate_random_paths = [&](int pop_num) -> std::vector<Individual> {
        std::vector<Individual> pops;
        for (int i = 0; i < pop_num; i++) {
            pops.push_back(generate_random_path());
        }
        return pops;
    };

    auto cross_two_paths = [&](Individual first_paths,
                               Individual second_paths) -> Individual {
        Individual crossed_paths = first_paths;
        for (unsigned int i = 0; i < first_paths.paths.size(); i++) {
            if (rand() % 10 < 5) {
                auto try_paths = crossed_paths;
                try_paths.paths[i] = second_paths.paths[i];
                auto try_paths_result =
                    paths_correct<NetStat>(try_paths, intentGenerator);
                if (try_paths_result.first) {
                    crossed_paths = try_paths;
                }
            }
        }
        return crossed_paths;
    };

    auto crossing = [&](std::vector<Individual> paths,
                        std::vector<float> params) {
        std::vector<Individual> new_paths;
        new_paths.reserve(paths.size());

        if (paths.size() < 2)
            return paths;

        for (unsigned int i = 0; i < paths.size(); i++) {
            auto first_paths = paths[i];
            auto second_paths = paths[(i + 1) % paths.size()];

            if (rand() % 10 < params[0]) {
                new_paths.push_back(first_paths);
            } else {
                auto crossed_path = cross_two_paths(first_paths, second_paths);
                new_paths.push_back(crossed_path);
            }
        }
        return new_paths;
    };

    auto target_function = [&](Individual indiv) {
        int delay = 0;
        int jitter = 0;
        int loss = 0;
        for (auto path : indiv.paths) {
            for (unsigned int i = 0; i < path.size() - 1; i++) {
                auto edge = indiv.graph.getEdgeBetween(path[i], path[i + 1]);
                if (edge.has_value()) {
                    delay += (*edge).weight.delay;
                    jitter += (*edge).weight.jitter;
                    loss += (*edge).weight.loss;
                }
            }
        }
        for (auto edge = indiv.graph.flow_left.begin();
             edge != indiv.graph.flow_left.end(); edge++) {
            if (edge->second < 0) {
                delay += edge->second * 10;
                loss += edge->second * 10;
                jitter += edge->second * 10;
            }
        }
        return std::vector<float>{static_cast<float>(delay),
                                  static_cast<float>(loss),
                                  static_cast<float>(jitter)};
    };

    auto proper_target = [&](Individual indiv) {
        int delay = 0;
        int jitter = 0;
        int loss = 0;
        for (auto path : indiv.paths) {
            for (unsigned int i = 0; i < path.size() - 1; i++) {
                auto edge = indiv.graph.getEdgeBetween(path[i], path[i + 1]);
                if (edge.has_value()) {
                    delay += (*edge).weight.delay;
                    jitter += (*edge).weight.jitter;
                    loss += (*edge).weight.loss;
                }
            }
        }
        return std::vector<float>{static_cast<float>(delay),
                                  static_cast<float>(loss),
                                  static_cast<float>(jitter)};
    };
    auto distance_function = [&](const Individual &first,
                                 const Individual &second) {
        double distance = 0.0;
        for (const auto &[key, val1] : first.graph.flow_left) {
            double val2 = second.graph.flow_left.at(key);
            distance += std::abs(val1 - val2);
        }
        return distance;
    };

    SPEA2<Individual> spea2(distance_function);
    // spea2.distance_function =distance_function
    std::vector<float> params = {5.0};
    auto indivs = spea2.solve(30, 40, target_function, crossing,
                              generate_random_paths, params);
    for (auto indiv : indivs) {
        auto vals = proper_target(indiv);
        for (auto v : vals)
            std::cout << v << " ";
        std::cout << std::endl;
    }
    return 0;
}