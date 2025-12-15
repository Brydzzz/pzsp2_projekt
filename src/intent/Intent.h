#pragma once

#include <vector>

#include "../graph/Graph.h"

class Intent {
    std::map<std::pair<Node, Node>, unsigned int> intents;
    void createIntentForPath(Graph<NetStat> &graph, std::vector<Node> &path);

  public:
    Intent() = default;
    unsigned int getDemand(Node &from, Node &to);
    void setDemand(Node &from, Node &to, unsigned int demand);
    void randomizeIntent(std::vector<Node> &nodes, int minValue = 0,
                         int maxValue = 10, int seed = 0);
    void randomizeIntent(Graph<NetStat> &graph, int seed = 0);
    std::vector<std::vector<unsigned int>>
    getIntentInNodeOrder(std::vector<Node> &nodes);
};
