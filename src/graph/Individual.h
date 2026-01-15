//
//
// Created by Marek on 12/01/26

#pragma once

#include "Graph.h"
#include <functional>
#include <map>
template <typename T>
using target_function = std::function<std::vector<float>(T)>;
class Individual {
  public:
    std::vector<std::vector<Node>> paths;
    const Graph<NetStat> *graph;

    Individual() = default;

    Individual(std::vector<std::vector<Node>> &paths,
               const Graph<NetStat> *graph);
};

std::vector<Individual>
individual_population_generator(int population_size,
                                const Graph<NetStat> &graph);
std::vector<float> individual_target_function(Individual indiv);
std::vector<float> individual_proper_target_function(Individual indiv);
float individual_distance_function(const std::vector<float> &target_first,
                                   const std::vector<float> &target_second);