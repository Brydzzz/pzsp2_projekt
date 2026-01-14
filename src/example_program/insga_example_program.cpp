#include "GraphParser.h"
#include "Intent.h"
#include "INSGA.h"
#include "Individual.h"
#include <fstream>
#include <iostream>

#define GRAPH_FILE "full_mesh_10.csv"

int main()
{
    std::ifstream f(GRAPH_FILE);
    if (!f.is_open())
    {
        std::cerr << "Error: Could not open graph file '" << GRAPH_FILE << "'" << std::endl;
        return 1; 
    }
    auto graph = GraphParser::parseCSVToGraph(f);
    f.close();

    auto crossing = [&](std::vector<Individual> paths,
                        std::vector<float> params)
    {
        // we assume no crossover operator
        return paths;
    };

    auto pop_generator = [&](int population_size) {
        return individual_population_generator(population_size, graph);
    };

    INSGA<Individual> insga(QoSCriterion::Loss);
    float mutation_probability = 0.5;
    float crossover_probability = 0.0;
    std::vector<float> params = {mutation_probability, crossover_probability};
    auto solution = insga.solve(10, 10, individual_target_function, crossing, 
                                pop_generator,
                                params);
    for (auto indiv : solution)
    {
        auto vals = individual_proper_target_function(indiv);
        for (auto v : vals)
        {
            std::cout << v << " ";
        }
        std::cout << std::endl;
    }
    return 0;
}   