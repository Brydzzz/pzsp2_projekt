//
// Created by tomek on 11/15/25.
//
#pragma once
#include "Graph.h"
#include <stdexcept>

template<typename T>
Graph<T>::Graph(std::vector<std::vector<T>>& adjacencyMatrix) {
    for (auto vec : adjacencyMatrix) {
        if (vec.size() != adjacencyMatrix.size()) {
            throw std::invalid_argument("The matrix has to be of NxN size");
        }
    }
    this->adjacencyMatrix = adjacencyMatrix;

}


template<typename T>
Graph<T>::Graph(int nodes) {
    if (nodes < 0) {
        throw std::invalid_argument("Nodes number has to be positive or zero");
    }
    this->adjacencyMatrix = std::vector<std::vector<T>>(nodes,std::vector<T>(nodes, std::numeric_limits<T>::max()));
}

template<typename T>
Graph<T>::Graph() : Graph(0) {
}

template<typename T>
int Graph<T>::getNodes() {
    return adjacencyMatrix.size();
}

template<typename T>
void Graph<T>::addEdge(int node1, int node2, T weight) {
    adjacencyMatrix[node1][node2] = weight;
    adjacencyMatrix[node2][node1] = weight;
}


template<typename T>
T Graph<T>::getWeightBetween(int node1, int node2) {
    return adjacencyMatrix[node1][node2];
}


