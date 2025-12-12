#include "Intent.h"
#include "../utils/utils.hpp"

unsigned int Intent::getDemand(Node &from, Node &to) {
    return intents[{from, to}];
}

void Intent::setDemand(Node &from, Node &to, unsigned int demand) {
    intents[{from, to}] = demand;
}

void Intent::randomizeIntent(std::vector<Node> &nodes, int minValue,
                             int maxValue, int seed) {
    std::srand(seed);
    intents.clear();
    for (Node &fromNode : nodes) {
        for (Node &toNode : nodes) {
            if (fromNode == toNode) {
                continue;
            }
            setDemand(fromNode, toNode, getRandomFromRange(minValue, maxValue));
        }
    }
}

void Intent::createIntentForPath(Graph<NetStat> &graph,
                                 std::vector<Node> &path) {
    if (path.size() < 2) {
        return;
    }
    unsigned int maxThroughput = 0;
    for (unsigned int i = 1; i < path.size(); i++) {
        auto edge = graph.getEdgeBetween(path[i - 1], path[i]);
        if (!edge) {
            setDemand(path[0], path.back(), 0);
            return;
        }
        unsigned int edgeThroughput =
            edge.value().weight.throughput - getDemand(path[i - 1], path[i]);
        if (i == 1 or maxThroughput > edgeThroughput) {
            maxThroughput = edgeThroughput;
        }
    }
    setDemand(path[0], path.back(), getRandomFromRange(0, maxThroughput));
}

void Intent::randomizeIntent(Graph<NetStat> &graph, int seed) {
    srand(seed);
    intents.clear();
    for (Node &fromNode : graph.getNodes()) {
        for (Node &toNode : graph.getNodes()) {
            if (fromNode == toNode) {
                continue;
            }
            auto randomPath = graph.generateRandomPath(fromNode, toNode);
            createIntentForPath(graph, randomPath);
        }
    }
}

std::vector<std::vector<unsigned int>>
Intent::getIntentInNodeOrder(std::vector<Node> &nodes) {
    std::vector<std::vector<unsigned int>> result(
        nodes.size(), std::vector<unsigned int>(nodes.size(), 0));

    for (unsigned int i = 0; i < nodes.size(); i++) {
        for (unsigned int j = 0; j < nodes.size(); j++) {
            if (i == j) {
                continue;
            }
            result[i][j] = getDemand(nodes[i], nodes[j]);
        }
    }
    return result;
}
