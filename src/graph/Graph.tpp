//
// Created by tomek on 11/15/25.
//

#include "../utils/utils.hpp"
#include "Edge.h"
#include "Graph.h"
#include "defines.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <optional>
#include <queue>
#include <random>
#include <stdexcept>

template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes,
                std::map<Node, std::vector<Edge<T>>>) {
    this->nodes = nodes;
    for (auto edge : adjacencyList) {
        if (std::ranges::find(this->nodes.begin(), this->nodes.end(),
                              edge.first_node) == this->nodes.end()) {
            throw std::invalid_argument("Node " + edge.first_node.name +
                                        " doesn't exist in this graph");
        }
        if (std::ranges::find(this->nodes.begin(), this->nodes.end(),
                              edge.second_node) == this->nodes.end()) {
            throw std::invalid_argument("Node " + edge.second_node.name +
                                        " doesn't exist in this graph");
        }
    }
    this->adjacencyList = adjacencyList;
}

template <typename T>
void Graph<T>::check_node_existance(Node &node) const {
    if (std::ranges::find(this->nodes.begin(), this->nodes.end(), node) ==
        this->nodes.end()) {
        throw std::invalid_argument("Node " + node.name +
                                    " doesn't exist in this graph");
    }
}
template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes, std::vector<Edge<T>> adjacencyList)
    : nodes(nodes) {
    for (auto node : this->nodes) {
        this->adjacencyList[node] = std::vector<Edge<T>>();
    }
    for (auto edge : adjacencyList) {
        check_node_existance(edge.first_node);
        check_node_existance(edge.second_node);
        addEdge(edge.first_node, edge.second_node, edge.weight);
    }
}

template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes) {
    this->nodes = nodes;
    for (auto &node : this->nodes) {
        this->adjacencyList[node] = std::vector<Edge<T>>();
    }
}

template <typename T>
Graph<T>::Graph() {
    this->nodes = std::vector<Node>();
    this->adjacencyList = std::map<Node, std::vector<Edge<T>>>();
}

template <typename T>
int Graph<T>::countNodes() {
    return nodes.size();
}

template <typename T>
std::vector<Node> Graph<T>::getNodes() const {
    return nodes;
}

template <typename T>
void Graph<T>::addEdge(Node &node1, Node &node2, T weight) {
    check_node_existance(node1);
    check_node_existance(node2);
    auto first_edge = Edge(node1, node2, weight);
    auto second_edge = Edge(node2, node1, weight);
    adjacencyList[node1].push_back(first_edge);
    adjacencyList[node2].push_back(second_edge);
    flow_left[first_edge] = first_edge.toInt();
}

template <typename T>
T Graph<T>::getWeightBetween(Node &node1, Node &node2) {
    for (unsigned i = 0; i < adjacencyList[node1].size(); i++) {
        if ((adjacencyList[node1][i].first_node == node1 &&
             adjacencyList[node1][i].second_node == node2) ||
            (adjacencyList[node1][i].first_node == node2 &&
             adjacencyList[node1][i].second_node == node1)) {
            return adjacencyList[node1][i].weight;
        }
    }
    throw std::invalid_argument("There isn't an edge between those nodes");
}

template <typename T>
bool Graph<T>::hasEdge(Node &from, Node &to) const {
    for (auto &edge : adjacencyList.at(from)) {
        if (edge.second_node == to)
            return true;
    }
    return false;
}

template <typename T>
int Graph<T>::getNextPathChoice(std::vector<int> &pathChoices) const {
    int pathChoice = pathChoices.back();
    pathChoices.pop_back();
    return pathChoice;
}
template <typename T>
void Graph<T>::updateEdgeFlows(std::vector<Node> path, unsigned int intent) {
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        flow_left[*getEdgeBetween(path[i], path[i + 1])] -= intent;
    }
}

template <typename T>
void Graph<T>::updateEdgeFlows(std::vector<Node> path, unsigned int intent,
                        std::map<Edge<T>, int>& external_flow_left) const
{
    for (unsigned int i = 0; i < path.size() - 1; i++) {
        external_flow_left[*getEdgeBetween(path[i], path[i + 1])] -= intent;
    }
}

template <typename T>
std::vector<Node> Graph<T>::generateRandomPath(Node &startNode, Node &endNode,
                                               unsigned int intent) {
    check_node_existance(startNode);
    check_node_existance(endNode);

    std::vector<Node> path;
    // to keep the adjacencyList intact we create a permutations for each node,
    // so that the order in which we take edges is randomized resulting in
    // random paths
    std::map<Node, std::vector<int>> pathChoices;

    std::mt19937 randomDevice = createRandomDevice();

    std::set<Node> usedNodes;
    usedNodes.insert(startNode);
    path.push_back(startNode);
    while (path.size() and path.back() != endNode) {
        auto lastNode = path.back();
        if (pathChoices.find(lastNode) == pathChoices.end()) {
            pathChoices[lastNode] = createRandomizedPermutation(
                adjacencyList.at(lastNode).size(), randomDevice);
        }
        std::optional<Node> nextNode = std::nullopt;
        while (pathChoices[lastNode].size()) {
            int pathChoice = getNextPathChoice(pathChoices[lastNode]);
            nextNode = adjacencyList.at(lastNode)[pathChoice].second_node;
            if (usedNodes.find(nextNode.value()) != usedNodes.end()) {
                nextNode = std::nullopt;
                continue;
            }
            break;
        }
        // int flow = flow_left[*getEdgeBetween(
        //     path.back(), *(nextNode))];
        if (nextNode) {
            path.push_back(nextNode.value());
            usedNodes.insert(path.back());
            continue;
        }
        usedNodes.erase(path.back());
        path.pop_back();
    }
    updateEdgeFlows(path, intent);
    return path;
}

template <typename T>
std::vector<Node> Graph<T>::generateRandomPath(Node &startNode, Node &endNode) const {
    check_node_existance(startNode);
    check_node_existance(endNode);

    std::vector<Node> path;
    // to keep the adjacencyList intact we create a permutations for each node,
    // so that the order in which we take edges is randomized resulting in
    // random paths
    std::map<Node, std::vector<int>> pathChoices;

    std::mt19937 randomDevice = createRandomDevice();

    std::set<Node> usedNodes;
    usedNodes.insert(startNode);
    path.push_back(startNode);
    while (path.size() and path.back() != endNode) {
        auto lastNode = path.back();
        if (pathChoices.find(lastNode) == pathChoices.end()) {
            pathChoices[lastNode] = createRandomizedPermutation(
                adjacencyList.at(lastNode).size(), randomDevice);
        }
        std::optional<Node> nextNode = std::nullopt;
        while (pathChoices[lastNode].size()) {
            int pathChoice = getNextPathChoice(pathChoices[lastNode]);
            nextNode = adjacencyList.at(lastNode)[pathChoice].second_node;
            if (usedNodes.find(nextNode.value()) != usedNodes.end()) {
                nextNode = std::nullopt;
                continue;
            }
            break;
        }
        if (nextNode) {
            path.push_back(nextNode.value());
            usedNodes.insert(path.back());
            continue;
        }
        usedNodes.erase(path.back());
        path.pop_back();
    }
    return path;
}

template <typename T>
std::vector<Node> Graph<T>::generateRandomPath(Node &startNode, Node &endNode,
                                         unsigned int intent, 
                                         std::map<Edge<T>, int>& external_flow_left) const
{
    std::vector<Node> generated_path = generateRandomPath(startNode, endNode);
    updateEdgeFlows(generated_path, intent, external_flow_left);
    return generated_path;
}

// TODO: implement this function properly
template <typename T>
std::vector<Node> Graph<T>::generateWidestPath(Node &startNode, Node &endNode) {
    check_node_existance(startNode);
    check_node_existance(endNode);

    return {};
}

template <typename T>
std::optional<Edge<T>> Graph<T>::getEdgeBetween(Node &fromNode, Node &toNode) const {
    check_node_existance(fromNode);
    check_node_existance(toNode);

    auto adjacencyListIterator = adjacencyList.find(fromNode);
    if (adjacencyListIterator == adjacencyList.end()) {
        return std::nullopt;
    }
    for (Edge<T> edge : adjacencyListIterator->second) {
        if (edge.second_node == toNode) {
            return edge;
        }
    }
    return std::nullopt;
}

template <typename T>
std::vector<Edge<T>> Graph<T>::getEdgesOf(const Node& node) const
{
    return adjacencyList.at(node);
}