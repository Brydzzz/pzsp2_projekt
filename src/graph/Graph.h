//
// Created by tomek on 11/15/25.
//
#pragma once
#include <vector>
#include <limits>

#ifndef GRAPH_H
#define GRAPH_H
#include "Edge.h"
template <typename T> class Graph {
    std::map<Node, std::vector<Edge<T>>> adjacencyList;
    std::vector<Node> nodes;
    void check_node_existance(Node& node);
public:
    Graph();
    Graph(std::vector<Node>& nodes);
    Graph(std::vector<Node>& nodes, std::map<Node, std::vector<Edge<T>>>);
    Graph(std::vector<Node>& nodes, std::vector<Edge<T>>);
    int getNodes();
    void addEdge(Node& node1, Node& node2, T weight);
    T getWeightBetween(Node& node1, Node& node2);

};


#include "Graph.tpp"
#endif //GRAPH_H
