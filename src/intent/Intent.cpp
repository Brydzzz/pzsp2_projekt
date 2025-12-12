#include "Intent.h"
#include "../utils/utils.hpp"

int Intent::getDemand(Node &from, Node &to) { return intents[{from, to}]; }

void Intent::setDemand(Node &from, Node &to, int demand) {
    intents[{from, to}] = demand;
}

void Intent::randomizeIntent(std::vector<Node> &nodes) {
    srand(time(NULL));
    for (Node &fromNode : nodes) {
        for (Node &toNode : nodes) {
            if (fromNode == toNode) {
                continue;
            }
            setDemand(fromNode, toNode, getRandomFromRange(0, 10));
        }
    }
}

void Intent::createIntentForPath(Graph<NetStat>& graph, std::vector<Node> &path) {
    unsigned int maxThroughput = 0;
    for (int i = 1; i<path.size(); i++) {
        auto edge =graph.getEdgeBetween(path[0], path[1]);
        if (!edge) {
            intents[std::make_pair(path[0], path.back())] = 0;
            return;
        }
        if (i == 1 or maxThroughput > edge.value().weight.throughput) {
            maxThroughput = edge.value().weight.throughput;
        }
    }
    intents[std::make_pair(path[0], path.back())] = maxThroughput;
}

void Intent::randomazeIntent(Graph<NetStat>& graph) {
    srand(time(NULL));
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

std::vector<std::vector<unsigned int>> Intent::getIntentInNodeOrder(std::vector<Node> &nodes) {
    std::vector<std::vector<unsigned int>> result(nodes.size(), std::vector<unsigned int>(nodes.size(), 0));

    for (int i = 0; i<nodes.size(); i++) {
        for (int j = 0; j<nodes.size(); j++) {
            if (i == j) {
                continue;
            }
            result[i][j] = getDemand(nodes[i], nodes[j]);
        }
    }
    return result;
}
