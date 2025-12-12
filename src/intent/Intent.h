#ifndef INTENT_H
#define INTENT_H

#include <vector>

#include "../graph/Graph.h"

class Intent {
    std::map<std::pair<Node, Node>, int> intents;
    void createIntentForPath(std::vector<Node>& path);
  public:
    Intent() {}
    int getDemand(Node &from, Node &to);
    void setDemand(Node &from, Node &to, int demand);
    void randomizeIntent(std::vector<Node> &nodes);
    void randomazeIntent(Graph<NetStat> &graph);
    std::vector<std::vector<unsigned int>> getIntentInNodeOrder(std::vector<Node> &nodes);
};

#include "Intent.cpp"

#endif