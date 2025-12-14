//
// Created by tomek on 11/15/25.
//

#include "Graph.h"
#include <stdexcept>

template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes,
                std::map<Node, std::vector<Edge<T>>>) {
    this->nodes = nodes;
    for (auto edge : adjacencyList) {
        if (std::find(this->nodes.begin(), this->nodes.end(), edge.first_node)
            == this->nodes.end()) {
            throw std::invalid_argument(
                "Node " + edge.first_node.name +
                " doesn't exist in this graph");
        }
        if (std::find(this->nodes.begin(), this->nodes.end(), edge.second_node)
            == this->nodes.end()) {
            throw std::invalid_argument(
                "Node " + edge.second_node.name +
                " doesn't exist in this graph");
        }
    }
    this->adjacencyList = adjacencyList;
}

template <typename T>
void Graph<T>::check_node_existance(Node &node) {
    if (std::find(this->nodes.begin(), this->nodes.end(), node) == this->nodes.
        end()) {
        throw std::invalid_argument(
            "Node " + node.name + " doesn't exist in this graph");
    }
}

template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes, std::vector<Edge<T>> adjacencyList) {
    this->nodes = nodes;
    for (auto node : this->nodes) {
        this->adjacencyList[node] = std::vector<Edge<T>>();
    }
    for (auto edge : adjacencyList) {
        check_node_existance(edge.first_node);
        check_node_existance(edge.second_node);
        this->adjacencyList[edge.first_node].push_back(edge);
        this->adjacencyList[edge.second_node].push_back(edge);
    }
}

template <typename T>
Graph<T>::Graph(std::vector<Node> &nodes) {
    this->nodes = nodes;
}


template <typename T>
Graph<T>::Graph() {
    this->nodes = std::vector<Node>();
    this->adjacencyList = std::map<Node, std::vector<Edge<T>>>();
}

template <typename T>
int Graph<T>::getNodes() {
    return nodes.size();
}

template <typename T>
void Graph<T>::addEdge(Node &node1, Node &node2, T weight) {
    check_node_existance(node1);
    check_node_existance(node2);
    adjacencyList[node1].push_back(Edge(node1, node2, weight));
    adjacencyList[node2].push_back(Edge(node1, node2, weight));
}


template <typename T>
T Graph<T>::getWeightBetween(Node &node1, Node &node2) {
    for (unsigned int i = 0; i < adjacencyList[node1].size(); i++) {
        if ((adjacencyList[node1][i].first_node == node1 && adjacencyList[node1]
             [i].second_node == node2) || (
                adjacencyList[node1][i].first_node == node2 && adjacencyList[
                    node1][i].second_node == node1)) {
            return adjacencyList[node1][i].weight;
        }
    }
    throw std::invalid_argument("There isn't an edge between those nodes");
}


