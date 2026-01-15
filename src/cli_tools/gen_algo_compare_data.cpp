#include "GraphParser.h"
#include "INSGA.h"
#include "Individual.h"
#include "Intent.h"
#include "NSGA2.h"
#include "Node.h"
#include "SPEA2.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <vector>
#include "MutationOperators.h"

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

    auto pop_generator = [&](int population_size) {
        return individual_population_generator(population_size, graph);
    };


    std::vector<std::tuple<std::string, int, Individual>> experiments_data;

    SPEA2<Individual> spea2(individual_distance_function);
    std::vector<float> spea2_params = {5.0};

    NSGA2<Individual> nsga2;
    std::vector<float> nsga2_params = {5.0, 1};

    INSGA<Individual> insga(QoSCriterion::Loss);
    float mutation_probability = 0.5;
    float crossover_probability = 0.0;
    std::vector<float> insga_params = {mutation_probability,
                                       crossover_probability};

    for (int i = 0; i < iterations; i++) {
        std::cout << "SPEA2 Run No" << i + 1 << std::endl;
        auto spea2_indivs = spea2.solve(30, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, spea2_params);
        for (const auto &indiv : spea2_indivs) {
            experiments_data.push_back({"SPEA2", i, indiv});
        }

        std::cout << "NSGA2 Run No" << i + 1 << std::endl;
        auto nsga2_indivs = nsga2.solve(30, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, nsga2_params);
        for (const auto &indiv : nsga2_indivs) {
            experiments_data.push_back({"NSGA2", i, indiv});
        }

        std::cout << "INSGA Run No" << i << std::endl;
        auto insga_indivs = insga.solve(10, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, insga_params);
        for (const auto &indiv : insga_indivs) {
            experiments_data.push_back({"INSGA", i, indiv});
        }
    }

    std::ofstream outFile(outputPath);
    if (outFile.is_open()) {
        outFile << "algo,run_id,loss,delay,jitter\n";
        for (const auto &[algo_name, run_id, indiv] : experiments_data) {
            std::vector<float> objectives =
                individual_proper_target_function(indiv);
            int delay = objectives[0];
            int loss = objectives[1];
            int jitter = objectives[2];
            outFile << algo_name << "," << run_id << "," << loss << "," << delay
                    << "," << jitter << "\n";
        }
        outFile.close();
    } else {
        std::cerr << "Error: Could not open output file " << outputPath
                  << std::endl;
    }

    return 0;
}