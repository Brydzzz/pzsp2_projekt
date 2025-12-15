//
// Created by tomek on 12/9/25.
//

#pragma once

#include "Node.h"
template <typename T>
class Edge {
  public:
    Node first_node;
    Node second_node;
    T weight;
    Edge(Node &node1, Node &node2, T weight)
        : first_node(node1), second_node(node2), weight(weight) {}
};
