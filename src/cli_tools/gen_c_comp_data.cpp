#include "GraphParser.h"
#include "INSGA.h"
#include "Individual.h"
#include "Intent.h"
#include "MutationOperators.h"
#include "NSGA2.h"
#include "Node.h"
#include "SPEA2.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>


int main(int argc, char *argv[]) {
    if (argc < 7 || (argc - 5) % 2 != 0) {
        std::cerr << "Usage: " << argv[0]
                  << " <output_path> <iterations> <runs> <mut_prob> "
                  << "<graph1> <intent1> [<graph2> <intent2> ...]" << std::endl;
        return 1;
    }

    std::string outputPath = argv[1];
    int iterations = std::atoi(argv[2]);
    int runs = std::atoi(argv[3]);
    float mut_prob = std::stof(argv[4]);

    const int population_size = 30;

    SPEA2<Individual> spea2(individual_distance_function);
    std::vector<float> spea2_params = {mut_prob};
    NSGA2<Individual> nsga2;
    std::vector<float> nsga2_params = {mut_prob};
    INSGA<Individual> insga(QoSCriterion::Loss);
    std::vector<float> insga_params = {mut_prob};

    std::ofstream outFile(outputPath);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open output file " << outputPath
                  << std::endl;
        return 1;
    }
    outFile << "node_count,algorithm,run_id,execution_time\n";

    for (int k = 5; k < argc; k += 2) {
        std::string graphPath = argv[k];
        std::string intentsPath = argv[k + 1];

        std::cout << "Processing: " << graphPath << "..." << std::endl;

        std::ifstream graphFile(graphPath);
        if (!graphFile.is_open()) {
            std::cerr << "Error: Could not open graph file at " << graphPath
                      << std::endl;
            continue;
        }
        Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphFile);
        graphFile.close();

        int node_count = graph.getNodes().size();

        Intent intentGenerator;
        std::ifstream intentsFile(intentsPath);
        if (!intentsFile.is_open()) {
            std::cerr << "Error: Could not open intent file at " << intentsPath
                      << std::endl;
            continue;
        }
        intentsFile >> intentGenerator;
        intentsFile.close();

        auto pop_generator = [&](int size) {
            return individual_population_generator(size, graph);
        };

        std::vector<std::thread> threads;
        std::vector<double> spea2_times(runs), nsga2_times(runs),
            insga_times(runs);

        for (int i = 0; i < runs; i++) {
            threads.push_back(std::thread([&, i]() {
                auto start = std::chrono::high_resolution_clock::now();
                spea2.solve(
                    population_size, iterations, individual_target_function,
                    INSGAMutationVariantAStrategy, pop_generator, spea2_params);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> diff = end - start;
                spea2_times[i] = diff.count();
            }));

            threads.push_back(std::thread([&, i]() {
                auto start = std::chrono::high_resolution_clock::now();
                nsga2.solve(
                    population_size, iterations, individual_target_function,
                    INSGAMutationVariantAStrategy, pop_generator, nsga2_params);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> diff = end - start;
                nsga2_times[i] = diff.count();
            }));

            threads.push_back(std::thread([&, i]() {
                auto start = std::chrono::high_resolution_clock::now();
                insga.solve(
                    population_size, iterations, individual_target_function,
                    INSGAMutationVariantAStrategy, pop_generator, insga_params);
                auto end = std::chrono::high_resolution_clock::now();

                std::chrono::duration<double> diff = end - start;
                insga_times[i] = diff.count();
            }));
        }

        for (unsigned int i = 0; i < threads.size(); i++) {
            threads[i].join();
        }

        for (int i = 0; i < runs; i++) {
            outFile << node_count << ",SPEA2," << i << "," << spea2_times[i]
                    << "\n";
        }
        for (int i = 0; i < runs; i++) {
            outFile << node_count << ",NSGA2," << i << "," << nsga2_times[i]
                    << "\n";
        }
        for (int i = 0; i < runs; i++) {
            outFile << node_count << ",INSGA," << i << "," << insga_times[i]
                    << "\n";
        }
    }

    outFile.close();
    std::cout << "All complexity tests completed." << std::endl;
    return 0;
}