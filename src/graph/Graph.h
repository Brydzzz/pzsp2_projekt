//
// Created by tomek on 11/15/25.
//
#pragma once
#include <vector>
#ifndef GRAPH_H
#define GRAPH_H



template <typename T> class Graph {
    std::vector<std::vector<T>> adjacencyMatrix;
public:
    Graph();
    explicit Graph(int nodes);
    int getNodes();
};


#include <Graph.tpp>
#endif //GRAPH_H
