#include "GraphParser.h"
#include "Intent.h"
#include "INSGA.h"
#include "Individual.h"
#include <fstream>
#include <iostream>

#DEFINE GRAPH_FILE "full_mesh_10.csv"

int main()
{
    std;:ifstream f(GRAPH_FILE);
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
    }

    INSGA<Individual> insga(QoSCriterion::Loss);
    double mutation_probability = 0.5;
    double crossover_probability = 0.0;
    std::vector<float> params = {mutation_probability, crossover_probability};
    auto solution = insga.solve(10, 10, individual_target, crossing, 
                                individual_population_generator,
                                params)
}   