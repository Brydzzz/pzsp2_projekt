//
// Created by tomek on 12/9/25.
//

#pragma once

#include "Node.h"
template <typename T>
class Edge {
  public:
    int toInt() const { return weight + 0; };
    Node first_node;
    Node second_node;
    T weight;
    Edge(Node &node1, Node &node2, T weight)
        : first_node(node1), second_node(node2), weight(weight) {}
    bool operator<(const Edge &other) const {
        const std::string min = std::min(first_node.name, second_node.name);
        const std::string max = std::max(first_node.name, second_node.name);
        const std::string other_min =
            std::min(other.first_node.name, other.second_node.name);
        const std::string other_max =
            std::max(other.first_node.name, other.second_node.name);
        if (min != other_min) {
            return min < other_min;
        }
        return max < other_max;
    }
};
