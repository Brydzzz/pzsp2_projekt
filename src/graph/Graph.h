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

    void check_node_existance(Node &node) const;
    std::vector<Node> resolvePath(Node &startNode, Node &endNode,
                                  std::map<Node, Node *> comingFrom);
    int getNextPathChoice(std::vector<int> &pathChoices) const;
    void updateEdgeFlows(std::vector<Node> path, unsigned int intent);
    void updateEdgeFlows(std::vector<Node> path, unsigned int intent,
                          std::map<Edge<T>, int>& external_flow_left) const;

  public:
    Graph();
    std::map<Edge<T>, int> flow_left;
    Graph(std::vector<Node> &nodes);
    Graph(std::vector<Node> &nodes,
          std::map<Node, std::vector<Edge<T>>> adjacencyList);
    Graph(std::vector<Node> &nodes, std::vector<Edge<T>> edges);
    int countNodes();
    std::vector<Node> getNodes() const;
    void addEdge(Node &node1, Node &node2, T weight);
    T getWeightBetween(Node &node1, Node &node2);
    bool hasEdge(Node &from, Node &to) const;
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode,
                                         unsigned int intent);
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode,
                                         unsigned int intent, 
                                         std::map<Edge<T>, int>& external_flow_left) const;
    std::vector<Node> generateRandomPath(Node &startNode, Node &endNode) const;

    std::vector<Node> generateWidestPath(Node &startNode, Node &endNode);
    std::optional<Edge<T>> getEdgeBetween(Node &fromNode, Node &toNode) const;

    std::vector<Edge<T>> getEdgesOf(const Node &node) const;
    void addNode(Node &node) { nodes.push_back(node); }
    bool hasNode(Node &node) const {
        // This will now work because <algorithm> is included
        if (std::find(nodes.begin(), nodes.end(), node) == nodes.end()) {
            return false;
        }
        return true;
    }
};

#include "Graph.tpp"
