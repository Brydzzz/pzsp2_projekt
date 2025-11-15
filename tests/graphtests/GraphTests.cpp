//
// Created by tomek on 11/15/25.
//
#include "gtest/gtest.h"
#include "Graph.h"
TEST(GraphTests, testGraphConstructor) {
    auto graph = Graph<int>();
    ASSERT_EQ(graph.getNodes(), 0);
}

TEST(GraphTests, testGraphConstructorManyNodes) {
    auto graph = Graph<int>(10);
    ASSERT_EQ(graph.getNodes(), 10);
}

TEST(GraphTests, testGraphConstructorNegativeNodes) {
    ASSERT_ANY_THROW(Graph<int>(-10));
}


TEST(GraphTests, addEdgeTest) {
    auto graph = Graph<int>(10);
    graph.addEdge(0, 1, 2);
    ASSERT_EQ(graph.getWeightBetween(0,1), 2);
}



