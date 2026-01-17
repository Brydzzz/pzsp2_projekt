#include "Individual.h"
#include "Intent.h"

Individual::Individual(std::vector<std::vector<Node>> &paths,
                       const Graph<NetStat> *graph)
    : paths(paths), graph(graph) {}

Individual generate_random_individual(const Graph<NetStat> &graph,
                                      const Intent &intentGenerator) {
    std::vector<std::vector<Node>> paths;
    Individual indiv(paths, &graph);
    for (auto intent : intentGenerator.intents) {
        Node startNode = intent.first.first;
        Node endNode = intent.first.second;
        indiv.paths.push_back(
            indiv.graph->generateRandomPath(startNode, endNode));
    }
    return indiv;
};

std::vector<Individual>
individual_population_generator(int population_size,
                                const Graph<NetStat> &graph) {
    Intent intentGenerator;
    intentGenerator.randomizeIntent(graph);
    std::vector<Individual> population;
    for (int i = 0; i < population_size; i++) {
        population.push_back(
            generate_random_individual(graph, intentGenerator));
    }
    return population;
}

float individual_distance_function(const std::vector<float> &target_first,
                                   const std::vector<float> &target_second) {
    float distance = 0.0;
    for (size_t i = 0; i < target_first.size(); i++) {
        distance += std::abs(target_first[i] - target_second[i]);
    }
    return distance;
};

std::vector<float> individual_target_function(Individual indiv) {
    float delay = 0;
    float jitter = 0;
    float loss = 0.0;
    for (auto &path : indiv.paths) {
        for (size_t i = 0; i < path.size() - 1; i++) {
            auto edge = indiv.graph->getEdgeBetween(path[i], path[i + 1]);
            if (edge.has_value()) {
                delay += edge->weight.delay;
                jitter += edge->weight.jitter;
                loss += edge->weight.loss;
            }
        }
    }
    return std::vector<float>{delay, loss, jitter};
}

std::vector<float> individual_proper_target_function(Individual indiv) {
    float delay = 0;
    float jitter = 0;
    float loss = 0.0;
    for (auto &path : indiv.paths) {
        for (size_t i = 0; i < path.size() - 1; i++) {
            auto edge = indiv.graph->getEdgeBetween(path[i], path[i + 1]);
            if (edge.has_value()) {
                delay += edge->weight.delay;
                jitter += edge->weight.jitter;
                loss += edge->weight.loss;
            }
        }
    }
    return std::vector<float>{delay, loss, jitter};
}