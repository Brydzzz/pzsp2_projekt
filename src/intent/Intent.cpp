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
