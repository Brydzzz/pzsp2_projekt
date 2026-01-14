#include "GraphParser.h"
// #include "INSGA.h"
#include "Intent.h"
#include "NSGA2.h"
#include "Node.h"
#include "SPEA2.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// TODO: change to Individual from graph/ when it will be ready
class Individual {
  public:
    std::vector<std::vector<Node>> paths;
    Graph<NetStat> graph;
    Individual() = default;
    Individual(std::vector<std::vector<Node>> &paths, Graph<NetStat> graph)
        : paths(paths), graph(graph) {}
};

template <typename T>
std::pair<bool, Graph<T>> are_paths_correct(Individual paths,
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

int main(int argc, char *argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0]
                  << "<graph_csv_path> <intent_csv_path> "
                     "<output_raw_data_path> <iterations>"
                  << std::endl;
        return 1;
    }

    std::string graphPath = argv[1];
    std::string intentsPath = argv[2];
    std::string outputPath = argv[3];
    int iterations = atoi(argv[4]);

    std::ifstream graphFile(graphPath);
    if (!graphFile.is_open()) {
        std::cerr << "Error: Could not open graph file at " << graphPath
                  << std::endl;
        return 1;
    }
    Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphFile);
    graphFile.close();

    Intent intentGenerator;
    std::ifstream intentsFile(intentsPath);
    if (!intentsFile.is_open()) {
        std::cerr << "Error: Could not open intent file at " << outputPath
                  << std::endl;
        return 1;
    }
    intentsFile >> intentGenerator;
    intentsFile.close();

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

    auto crossing = [&](std::vector<Individual> paths,
                        std::vector<float> params) {
        for (auto indiv : paths) {
            for (auto &path : indiv.paths) {
                float random = rand();
                float prec = random / (float)RAND_MAX;
                if (prec > params[1])
                    continue;
                auto start = path.front();
                auto end = path.back();
                auto newPath = graph.generateRandomPath(start, end);
                path = newPath;
            }
        }
        return paths;
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
                delay += edge->second * 1000;
                loss += edge->second * 1000;
                jitter += edge->second * 1000;
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

    std::vector<std::tuple<std::string, int, Individual, bool>>
        experiments_data;

    SPEA2<Individual> spea2(distance_function);
    std::vector<float> spea2_params = {5.0};

    NSGA2<Individual> nsga2;
    std::vector<float> nsga2_params = {5.0, 1};

    // INSGA<Individual> insga(QoSCriterion::Loss);
    // double mutation_probability = 0.5;
    // double crossover_probability = 0.0;
    // std::vector<float> insga_params = {mutation_probability,
    //                                    crossover_probability};

    for (int i = 0; i < iterations; i++) {
        std::cout << "SPEA2 Run No" << i + 1 << std::endl;
        auto spea2_indivs = spea2.solve(30, 3, target_function, crossing,
                                        generate_random_paths, spea2_params);
        for (const auto &indiv : spea2_indivs) {
            experiments_data.push_back(
                {"SPEA2", i, indiv,
                 are_paths_correct<NetStat>(indiv, intentGenerator).first});
        }

        std::cout << "NSGA2 Run No" << i + 1 << std::endl;
        auto nsga2_indivs = nsga2.solve(30, 3, target_function, crossing,
                                        generate_random_paths, nsga2_params);
        for (const auto &indiv : nsga2_indivs) {
            experiments_data.push_back(
                {"NSGA2", i, indiv,
                 are_paths_correct<NetStat>(indiv, intentGenerator).first});
        }

        // std::cout << "INSGA Run No" << i << std::endl;
        // auto insga_indivs = insga.solve(10, 5, target_function, crossing,
        //                                 generate_random_path, insga_params);
        // solutions_set.insert(solutions_set.end(), insga_indivs.begin(),
        //                      insga_indivs.end());
        // for (const auto& indiv : insga_indivs) {
        //     if (are_paths_correct<NetStat>(indiv, intentGenerator).first) {
        //         solutions_set.push_back(indiv);
        //     }
        // }
    }

    std::ofstream outFile(outputPath);
    if (outFile.is_open()) {
        outFile << "algo,run_id,loss,delay,jitter,valid\n";
        for (const auto &[algo_name, run_id, indiv, isvalid] :
             experiments_data) {
            std::vector<float> objectives = proper_target(indiv);
            int delay = objectives[0];
            int loss = objectives[1];
            int jitter = objectives[2];
            std::string valid = (isvalid) ? "True" : "False";
            outFile << algo_name << "," << run_id << "," << loss << "," << delay
                    << "," << jitter << "," << valid << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Error: Could not open output file " << outputPath
                  << std::endl;
    }

    return 0;
}