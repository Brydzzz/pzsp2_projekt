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

    // auto cross_two_paths = [&](Individual first_paths,
    //                            Individual second_paths) -> Individual {
    //     Individual crossed_paths = first_paths;
    //     for (unsigned int i = 0; i < first_paths.paths.size(); i++) {
    //         if (rand() % 10 < 5) {
    //             auto try_paths = crossed_paths;
    //             try_paths.paths[i] = second_paths.paths[i];
    //             auto try_paths_result =
    //                 are_paths_correct<NetStat>(try_paths, intentGenerator);
    //             if (try_paths_result.first) {
    //                 crossed_paths = try_paths;
    //             }
    //         }
    //     }
    //     return crossed_paths;
    // };

    // auto crossing = [&](std::vector<Individual> paths,
    //                     std::vector<float> params) {
    //     std::vector<Individual> new_paths;
    //     new_paths.reserve(paths.size());

    //     if (paths.size() < 2)
    //         return paths;

    //     for (unsigned int i = 0; i < paths.size(); i++) {
    //         auto first_paths = paths[i];
    //         auto second_paths = paths[(i + 1) % paths.size()];

    //         if (rand() % 10 < params[0]) {
    //             new_paths.push_back(first_paths);
    //         } else {
    //             auto crossed_path = cross_two_paths(first_paths,
    //             second_paths); new_paths.push_back(crossed_path);
    //         }
    //     }
    //     return new_paths;
    // };

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

    std::vector<Individual> solutions_set;

    SPEA2<Individual> spea2(distance_function);
    std::vector<float> spea2_params = {5.0};

    NSGA2<Individual> nsga2;
    std::vector<float> nsga2_params = {5.0, 1};

    // INSGA<Individual> insga(QoSCriterion::Loss);
    // double mutation_probability = 0.5;
    // double crossover_probability = 0.0;
    // std::vector<float> insga_params = {mutation_probability,
    //                                    crossover_probability};

    // Run each algo 10 times
    for (int i = 0; i < 2; i++) {
        std::cout << "SPEA2 Run No" << i << std::endl;
        auto spea2_indivs = spea2.solve(30, 10, target_function, crossing,
                                        generate_random_paths, spea2_params);
        for (const auto &indiv : spea2_indivs) {
            if (are_paths_correct<NetStat>(indiv, intentGenerator).first) {
                solutions_set.push_back(indiv);
            }
        }

        std::cout << "NSGA2 Run No" << i << std::endl;
        auto nsga2_indivs = nsga2.solve(30, 10, target_function, crossing,
                                        generate_random_paths, nsga2_params);
        for (const auto &indiv : nsga2_indivs) {
            if (are_paths_correct<NetStat>(indiv, intentGenerator).first) {
                solutions_set.push_back(indiv);
            }
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

    // Build true pareto front approximation
    pareto::front<float, 3, Individual> true_pf;

    std::cout << "Building Pareto Front from " << solutions_set.size()
              << " solutions..." << std::endl;

    for (const auto &indiv : solutions_set) {
        std::vector<float> objectives = proper_target(indiv);
        auto indiv_point =
            pareto::point<float, 3>(objectives.begin(), objectives.end());
        true_pf.insert({indiv_point, indiv});
    }

    std::cout << "Final Pareto Front size: " << true_pf.size() << std::endl;

    std::ofstream outFile(outputPath);
    if (outFile.is_open()) {
        outFile << "delay,loss,jitter\n";
        for (const auto &[obj, indiv] : true_pf) {
            outFile << obj[0] << "," << obj[1] << "," << obj[2] << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Error: Could not open output file " << outputPath
                  << std::endl;
    }

    return 0;
}