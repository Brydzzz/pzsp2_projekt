//
// Created by tomek on 11/15/25.
//
#include "gtest/gtest.h"
#include "Graph.h"
TEST(GraphTests, testGraphConstructor) {
    auto graph = Graph<int>();
    ASSERT_EQ(graph.getNodes(), 0);
}
