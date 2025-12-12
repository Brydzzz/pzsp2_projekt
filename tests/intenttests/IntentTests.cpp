//
// Created by tomek on 11/15/25.
//
#include <stdlib.h>

#include "Graph.h"
#include "gtest/gtest.h"

bool isPathGood(Graph<int> &graph, std::vector<Node> &path) {
    for (int i = 1; i < path.size(); i++) {
        if (!graph.haveEdge(path[i - 1], path[i]))
            return false;
    }
    return true;
}

Node node1 = Node("Warszawa");
Node node2 = Node("Kraków");
Node node3 = Node("Gdańsk");
Node node4 = Node("Poznań");
Node node5 = Node("Kraków2");
Node node6 = Node("Gdańsk2");
Node node7 = Node("Poznań2");
Node node8 = Node("Kraków3");
Node node9 = Node("Gdańsk3");
Node node10 = Node("Poznań3");

TEST(GraphTests, aaa) {
    auto graph = Graph<int>();
    ASSERT_EQ(1, 0);
}

TEST(GraphTests, testGraphConstructorManyNodes) {
    std::vector nodes = {node1, node2, node3};
    auto graph = Graph<int>(nodes);
    ASSERT_EQ(graph.getNodes(), 3);
}

TEST(GraphTests, testGraphConstructorList) {
    std::vector nodes = {node1, node2, node3};
    std::vector<Edge<int>> adjacencyList = {
        Edge<int>(node1, node2, 1),
        Edge<int>(node2, node3, 2),
        Edge<int>(node3, node1, 3),
    };
    auto graph = Graph(nodes, adjacencyList);
    ASSERT_EQ(graph.getNodes(), 3);
    ASSERT_EQ(graph.getWeightBetween(node1, node2), 1);
    ASSERT_EQ(graph.getWeightBetween(node2, node3), 2);
    ASSERT_EQ(graph.getWeightBetween(node3, node1), 3);
}

TEST(GraphTests, testGraphConstructorInvalidMatrix) {
    std::vector nodes = {node1, node2, node3};
    std::vector<Edge<int>> adjacencyList = {
        Edge<int>(node4, node2, 1),
        Edge<int>(node2, node3, 2),
        Edge<int>(node3, node1, 3),
    };
    ASSERT_ANY_THROW(auto graph = Graph(nodes, adjacencyList));
}

TEST(GraphTests, addEdgeTest) {
    std::vector nodes = {node1, node2, node3};
    auto graph = Graph<int>(nodes);
    graph.addEdge(node1, node2, 2);
    ASSERT_EQ(graph.getWeightBetween(node1, node2), 2);
    ASSERT_EQ(graph.getWeightBetween(node2, node1), 2);
}

TEST(GraphTests, addEdgeTest2) {
    std::vector nodes = {node1, node2, node3};
    auto graph = Graph<int>(nodes);
    graph.addEdge(node1, node2, 2);
    ASSERT_EQ(graph.getWeightBetween(node1, node2), 2);
    ASSERT_EQ(graph.getWeightBetween(node2, node1), 2);
}

TEST(GraphTests, randomPathGeneratorTest) {
    std::vector nodes = {node1, node2, node3, node4, node5,
                         node6, node7, node8, node9, node10};
    std::vector<Edge<int>> adjacencyList = {
        Edge<int>(node1, node2, 1),  Edge<int>(node2, node3, 2),
        Edge<int>(node3, node4, 3),  Edge<int>(node4, node5, 1),
        Edge<int>(node5, node6, 2),  Edge<int>(node6, node7, 3),
        Edge<int>(node7, node8, 1),  Edge<int>(node8, node9, 2),
        Edge<int>(node9, node10, 3),
    };
    auto graph = Graph(nodes, adjacencyList);
    std::vector<Node> path = graph.generateRandomPath(nodes[2], nodes.back());
    ASSERT_GT(path.size(), 0);
    ASSERT_EQ(path[0], nodes[2]);
    ASSERT_EQ(path.back(), nodes.back());
    ASSERT_EQ(isPathGood(graph, path), true);
}

TEST(GraphTests, randomPathGeneratorTest2) {
    std::vector nodes = {node1, node2, node4, node3};
    std::vector<Edge<int>> adjacencyList = {
        Edge<int>(node1, node2, 1),
        Edge<int>(node2, node3, 2),
        Edge<int>(node1, node3, 3),
        Edge<int>(node3, node1, 1),
    };
    auto graph = Graph(nodes, adjacencyList);
    std::vector<Node> path =
        graph.generateRandomPath(nodes.front(), nodes.back());
    ASSERT_GT(path.size(), 0);
    ASSERT_EQ(path[0], nodes[0]);
    ASSERT_EQ(path.back(), nodes.back());
    ASSERT_EQ(isPathGood(graph, path), true);

    ASSERT_ANY_THROW(graph.generateRandomPath(nodes.front(), node5));
    path = graph.generateRandomPath(nodes.front(), node4);
    ASSERT_EQ(path.size(), 0);
}
