//
// Created by tomek on 12/9/25.
//

#ifndef EDGE_H
#define EDGE_H
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

#endif // EDGE_H
