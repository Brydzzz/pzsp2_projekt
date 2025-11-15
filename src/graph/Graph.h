//
// Created by tomek on 11/15/25.
//
#include <vector>
#ifndef GRAPH_H
#define GRAPH_H



template <typename T> class Graph {
    std::vector<std::vector<T>> adjacencyMatrix;
public:
    Graph();
    Graph(int nodes);
    getNodes();
};



#endif //GRAPH_H
