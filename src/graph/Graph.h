//
// Created by tomek on 11/15/25.
//
#pragma once
#include <limits>
#include <map>
#include <set>
#include <vector>
#include <optional>

#include "Edge.h"
#include "NetStat.h"
template <typename T>
class Graph {
  protected:
    std::map<Node, std::vector<Edge<T>>> adjacencyList;
    std::vector<Node> nodes;

    void check_node_existance(Node &node);

  public:
    Graph();
    Graph(std::vector<Node> &nodes);
    Graph(std::vector<Node> &nodes,
          std::map<Node, std::vector<Edge<T>>> adjacencyList);
    Graph(std::vector<Node> &nodes, std::vector<Edge<T>> edges);
    int getNodes();
    void addEdge(Node &node1, Node &node2, T weight);
    T getWeightBetween(Node &node1, Node &node2);
    bool haveEdge(Node &from, Node &to);
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode);
    std::optional<Edge<T>> getEdgeBetween(Node &fromNode, Node &toNode);

};

#include "Graph.tpp"
