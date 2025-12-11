//
// Created by tomek on 11/15/25.
//
#include "gtest/gtest.h"
#include "Graph.h"

Node node1 = Node("Warszawa");
Node node2 = Node("Kraków");
Node node3 = Node("Gdańsk");
Node node4 = Node("Poznań");


TEST(GraphTests, testGraphConstructor) {
    auto graph = Graph<int>();
    ASSERT_EQ(graph.getNodes(), 0);
}

TEST(GraphTests, testGraphConstructorManyNodes) {
    std::vector nodes = {node1, node2, node3};
    auto graph = Graph<int>(nodes);
    ASSERT_EQ(graph.getNodes(), 3);
}


TEST(GraphTests, testGraphConstructorList) {
    std::vector nodes = {node1, node2, node3};
    std::vector<Edge<int>> adjacencyList = {Edge<int>(node1, node2, 1), Edge<int>(node2, node3, 2),Edge<int>(node3, node1, 3),};
    auto graph = Graph(nodes, adjacencyList);
    ASSERT_EQ(graph.getNodes(), 3);
    ASSERT_EQ(graph.getWeightBetween(node1,node2), 1);
    ASSERT_EQ(graph.getWeightBetween(node2,node3), 2);
    ASSERT_EQ(graph.getWeightBetween(node3,node1), 3);
}


TEST(GraphTests, testGraphConstructorInvalidAdjacencyList) {
    std::vector nodes = {node1, node2, node3};
    std::vector<Edge<int>> adjacencyList = {Edge<int>(node4, node2, 1), Edge<int>(node2, node3, 2),Edge<int>(node3, node1, 3),};
    ASSERT_ANY_THROW(auto graph = Graph(nodes, adjacencyList));
}


TEST(GraphTests, addEdgeTest) {
    std::vector nodes = {node1, node2, node3};
    auto graph = Graph<int>(nodes);
    graph.addEdge(node1, node2, 2);
    ASSERT_EQ(graph.getWeightBetween(node1,node2), 2);
    ASSERT_EQ(graph.getWeightBetween(node2,node1), 2);
}




