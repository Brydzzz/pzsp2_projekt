//
// Created by tomek on 11/15/25.
//
#include "gtest/gtest.h"
#include "../../src/graph/Graph.h"
TEST(GraphTests, testGraphConstructor) {
    Graph<int> graph = Graph<int>();
    ASSERT_EQ(graph.getNodes(), 0);
}
