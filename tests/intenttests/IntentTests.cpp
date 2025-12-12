//
// Created by cacteyy
//
#include <stdlib.h>
#include <vector>

#include "../graph/Graph.h"
#include "../graph/NetStat.h"
#include "../graph/Node.h"
#include "Intent.h"
#include "gtest/gtest.h"

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

std::vector<Node> allNodes = {node1, node2, node3, node4, node5,
                              node6, node7, node8, node9, node10};

TEST(IntentTests, demandNotExisting) {
    Intent testing_intent;
    ASSERT_EQ(testing_intent.getDemand(node1, node2), 0);
}

TEST(IntentTests, settingDemand) {
    Intent testing_intent;
    testing_intent.setDemand(node1, node2, 10);
    ASSERT_EQ(testing_intent.getDemand(node1, node2), 10);
    ASSERT_EQ(testing_intent.getDemand(node2, node3), 0);
}

TEST(IntentTests, randomizingIntent) {
    Intent testing_intent;
    testing_intent.setDemand(node1, node2, 10);
    ASSERT_EQ(testing_intent.getDemand(node1, node2), 10);
    ASSERT_EQ(testing_intent.getDemand(node2, node3), 0);
}

TEST(IntentTests, randomizingIntentByPath) {
    Intent testing_intent;
    Graph<NetStat> graph(allNodes);
    graph.addEdge(node1, node2, NetStat{10, 10, 10, 10});
    graph.addEdge(node2, node1, NetStat{10, 10, 10, 10});
    testing_intent.randomizeIntent(graph);
    int demand12 = testing_intent.getDemand(node1, node2);
    int demand21 = testing_intent.getDemand(node2, node1);
    ASSERT_GE(demand12, 0);
    ASSERT_LE(demand12, 10);
    ASSERT_GE(demand21, 0);
    ASSERT_LE(demand21, 10);
    ASSERT_LE(demand12 + demand21, 10);
}

TEST(IntentTests, randomizingIntentByPath2) {
    Intent testing_intent;
    Graph<NetStat> graph(allNodes);
    graph.addEdge(node1, node2, NetStat{10, 10, 10, 10});
    graph.addEdge(node2, node1, NetStat{10, 10, 10, 10});
    graph.addEdge(node1, node3, NetStat{10, 10, 10, 10});
    graph.addEdge(node3, node1, NetStat{10, 10, 10, 10});
    graph.addEdge(node3, node2, NetStat{10, 10, 10, 10});
    graph.addEdge(node2, node3, NetStat{10, 10, 10, 10});
    testing_intent.randomizeIntent(graph);
    int demand12 = testing_intent.getDemand(node1, node2);
    int demand21 = testing_intent.getDemand(node2, node1);
    int demand13 = testing_intent.getDemand(node1, node3);
    int demand31 = testing_intent.getDemand(node3, node1);
    int demand23 = testing_intent.getDemand(node2, node3);
    int demand32 = testing_intent.getDemand(node3, node2);
    ASSERT_GE(demand12, 0);
    ASSERT_LE(demand12, 10);
    ASSERT_GE(demand21, 0);
    ASSERT_LE(demand21, 10);
    ASSERT_GE(demand13, 0);
    ASSERT_LE(demand13, 10);
    ASSERT_GE(demand31, 0);
    ASSERT_LE(demand31, 10);
    ASSERT_GE(demand23, 0);
    ASSERT_LE(demand23, 10);
    ASSERT_GE(demand32, 0);
    ASSERT_LE(demand32, 10);
    ASSERT_LE(demand12 + demand21, 10);
    ASSERT_LE(demand13 + demand31, 10);
    ASSERT_LE(demand23 + demand32, 10);
}

TEST(IntentTests, returningIntentList) {
    using namespace std;
    Intent testing_intent;
    testing_intent.setDemand(node1, node2, 10);
    testing_intent.setDemand(node2, node1, 9);
    testing_intent.setDemand(node3, node1, 6);
    testing_intent.setDemand(node2, node3, 3);
    vector<Node> nodeOrder = {node1, node2, node3};
    vector<vector<unsigned int>> demandList =
        testing_intent.getIntentInNodeOrder(nodeOrder);
    ASSERT_EQ(demandList[0][0], 0);
    ASSERT_EQ(demandList[0][1], 10);
    ASSERT_EQ(demandList[0][2], 0);
    ASSERT_EQ(demandList[1][0], 9);
    ASSERT_EQ(demandList[1][1], 0);
    ASSERT_EQ(demandList[1][2], 3);
    ASSERT_EQ(demandList[2][0], 6);
    ASSERT_EQ(demandList[2][1], 0);
    ASSERT_EQ(demandList[2][2], 0);
}
