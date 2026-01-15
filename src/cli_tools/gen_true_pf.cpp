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

    

    auto pop_generator = [&](int population_size) {
        return individual_population_generator(population_size, graph);
    };

    // TODO change because we are getting rid of flow_left
    
    std::vector<Individual> solutions_set;

    SPEA2<Individual> spea2(individual_distance_function);
    std::vector<float> spea2_params = {5.0};

    NSGA2<Individual> nsga2;
    std::vector<float> nsga2_params = {5.0, 1};

    INSGA<Individual> insga(QoSCriterion::Loss);
    float mutation_probability = 0.5;
    float crossover_probability = 0.0;
    std::vector<float> insga_params = {mutation_probability,
                                       crossover_probability};

    for (int i = 0; i < 1; i++) {
        std::cout << "SPEA2 Run No" << i << std::endl;
        auto spea2_indivs = spea2.solve(30, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, spea2_params);
        solutions_set.insert(solutions_set.end(), spea2_indivs.begin(),
                             spea2_indivs.end());

        std::cout << "NSGA2 Run No" << i << std::endl;
        auto nsga2_indivs = nsga2.solve(30, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, nsga2_params);
        solutions_set.insert(solutions_set.end(), nsga2_indivs.begin(),
                             nsga2_indivs.end());

        std::cout << "INSGA Run No" << i << std::endl;
        auto insga_indivs = insga.solve(10, 10, individual_target_function,
                                        INSGAMutationVariantAStrategy, pop_generator, insga_params);
        solutions_set.insert(solutions_set.end(), insga_indivs.begin(),
                             insga_indivs.end());
    }

    pareto::front<float, 3, Individual> true_pf;

    std::cout << "Building Pareto Front from " << solutions_set.size()
              << " solutions..." << std::endl;

    for (const auto &indiv : solutions_set) {
        std::vector<float> objectives =
            individual_proper_target_function(indiv);
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