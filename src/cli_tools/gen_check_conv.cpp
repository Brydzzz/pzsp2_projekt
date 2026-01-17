#include "GraphParser.h"
#include "INSGA.h"
#include "Individual.h"
#include "Intent.h"
#include "MutationOperators.h"
#include "NSGA2.h"
#include "Node.h"
#include "SPEA2.h"
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>

int main(int argc, char *argv[]) {
    if (argc < 7) {
        std::cerr << "Usage: " << argv[0]
                  << "<graph_csv_path> <intent_csv_path> "
                     "<output_raw_data_path> <iterations> <runs> "
                     "<mutation_probability>"
                  << std::endl;
        return 1;
    }

    std::string graphPath = argv[1];
    std::string intentsPath = argv[2];
    std::string outputPath = argv[3];
    int iterations = atoi(argv[4]);
    int runs = atoi(argv[5]);
    float mut_prob = std::stof(argv[6]);
    int max_pop = atoi(argv[7]);
    int step = atoi(argv[8]);

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

    std::vector<float> spea2_params = {mut_prob};
    std::vector<float> nsga2_params = {mut_prob};
    std::vector<float> insga_params = {mut_prob};
    std::ofstream outFile(outputPath);
    if (outFile.is_open()) {
        outFile << "algo,run_id,iteration,popsize,loss,delay,jitter\n";
        outFile.close();
    } else {
        std::cerr << "Error: Could not open output file " << outputPath
                  << std::endl;
    }
    for (int popsize = step; popsize <= max_pop; popsize += step) {
        std::vector<std::tuple<std::string, int, int, int, Individual>>
            experiments_data;

        std::cout << "Popsize: " << popsize << std::endl;
        std::vector<std::thread> threads;

        std::vector<std::vector<std::vector<Individual>>> spea2_indivs(runs),
            nsga2_indivs(runs), insga_indivs(runs);
        for (int i = 0; i < runs; i++) {
            threads.push_back(std::thread([&, i]() {
                SPEA2<Individual> spea2(&individual_distance_function, true);
                spea2.solve(popsize, iterations, individual_target_function,
                            INSGAMutationVariantAStrategy, pop_generator,
                            spea2_params);
                spea2_indivs[i] = spea2.getLogs();
            }));

            threads.push_back(std::thread([&, i]() {
                NSGA2<Individual> nsga2(true);
                nsga2.solve(popsize, iterations, individual_target_function,
                            INSGAMutationVariantAStrategy, pop_generator,
                            nsga2_params);
                nsga2_indivs[i] = nsga2.getLogs();
            }));
            threads.push_back(std::thread([&, i]() {
                INSGA<Individual> insga(QoSCriterion::Loss, true);
                insga.solve(popsize, iterations, individual_target_function,
                            INSGAMutationVariantAStrategy, pop_generator,
                            insga_params);
                insga_indivs[i] = insga.getLogs();
            }));
        }

        for (unsigned int i = 0; i < threads.size(); i++) {
            threads[i].join();
        }

        for (int i = 0; i < runs; i++) {
            for (size_t j = 0; j < insga_indivs[i].size(); j++) {
                for (const auto &indiv : insga_indivs[i][j])
                    experiments_data.push_back({"INSGA", i, j, popsize, indiv});
            }
        }

        for (int i = 0; i < runs; i++) {
            for (size_t j = 0; j < nsga2_indivs[i].size(); j++) {
                for (const auto &indiv : nsga2_indivs[i][j])
                    experiments_data.push_back({"NSGA2", i, j, popsize, indiv});
            }
        }

        for (int i = 0; i < runs; i++) {
            for (size_t j = 0; j < spea2_indivs[i].size(); j++) {
                for (const auto &indiv : spea2_indivs[i][j])
                    experiments_data.push_back({"SPEA2", i, j, popsize, indiv});
            }
        }
        std::ofstream outFile(outputPath, std::ios::app);
        if (outFile.is_open()) {

            for (const auto &[algo_name, run_id, iterations, popsize, indiv] :
                 experiments_data) {
                std::vector<float> objectives =
                    individual_proper_target_function(indiv);
                int delay = objectives[0];
                int loss = objectives[1];
                int jitter = objectives[2];
                outFile << algo_name << "," << run_id << "," << iterations
                        << "," << popsize << "," << loss << "," << delay << ","
                        << jitter << "\n";
            }
            outFile.close();
        } else {
            std::cerr << "Error: Could not open output file " << outputPath
                      << std::endl;
        }
    }
    return 0;
}
