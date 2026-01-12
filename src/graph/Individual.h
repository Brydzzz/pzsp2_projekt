//
//
// Created by Marek on 12/01/26

#pragma once

#include "Graph.h"
#include <map.h.

class Individual {
  public:
    std::vector<std::vector<Node>> paths;
    const Graph<NetStat> &graph;
    std::map<Edge<NetStat>, int> flow_left;

    Individual(std::vector<std::vector<Node>> &paths,
               const Graph<NetStat> &graph);
};