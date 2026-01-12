//
// Created by tomek on 11/15/25.
//
#pragma once
#include "Edge.h"
#include "NetStat.h"
#include <algorithm>
#include <limits>
#include <map>
#include <optional>
#include <set>
#include <vector>
template <typename T>
class Graph {
  protected:
    std::map<Node, std::vector<Edge<T>>> adjacencyList;
    std::vector<Node> nodes;

    void check_node_existance(Node &node);
    std::vector<Node> resolvePath(Node &startNode, Node &endNode,
                                  std::map<Node, Node *> comingFrom);
    int getNextPathChoice(std::vector<int> &pathChoices);
    void updateEdgeFlows(std::vector<Node> path, unsigned int intent);

  public:
    Graph();
    std::map<Edge<T>, int> flow_left;
    Graph(std::vector<Node> &nodes);
    Graph(std::vector<Node> &nodes,
          std::map<Node, std::vector<Edge<T>>> adjacencyList);
    Graph(std::vector<Node> &nodes, std::vector<Edge<T>> edges);
    int countNodes();
    std::vector<Node> getNodes();
    void addEdge(Node &node1, Node &node2, T weight);
    T getWeightBetween(Node &node1, Node &node2);
    bool hasEdge(Node &from, Node &to);
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode,
                                         unsigned int intent);
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode);

    std::vector<Node> generateWidestPath(Node &startNode, Node &endNode);
    std::optional<Edge<T>> getEdgeBetween(Node &fromNode, Node &toNode);

    std::vector<Edge<T>> getEdgesOf(const Node &node);
    void addNode(Node &node) { nodes.push_back(node); }
    bool hasNode(Node &node) {
        // This will now work because <algorithm> is included
        if (std::find(nodes.begin(), nodes.end(), node) == nodes.end()) {
            return false;
        }
        return true;
    }
};

#include "Graph.tpp"
