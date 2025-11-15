//
// Created by tomek on 11/15/25.
//
#pragma once
#include <vector>
#include <limits>

#ifndef GRAPH_H
#define GRAPH_H

template <typename T> class Graph {
    std::vector<std::vector<T>> adjacencyMatrix;
public:
    Graph();

    explicit Graph(std::vector<std::vector<T>>& adjacencyMatrix);

    explicit Graph(int nodes);
    int getNodes();
    void addEdge(int node1, int node2, T weight);
    T getWeightBetween(int node1, int node2);

};


#include "Graph.tpp"
#endif //GRAPH_H
