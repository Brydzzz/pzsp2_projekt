#include "Graph.h"
#include "GraphParser.h"
#include "gtest/gtest.h"
#include "csv.h"
#include <sstream>

TEST(GraphParserTests, ParseValidSimpleCSV) {
    std::string csv_data =
        "Source,Target,Throughput,Loss,Delay,Jitter\n0,1,32,274,10,52\n0,2,30,"
        "271,15,72\n1,2,38,270,11,42\n";
    std::istringstream graphStream(csv_data);

    Node node0 = Node("0");
    Node node1 = Node("1");
    Node node2 = Node("2");
    NetStat true_stat01 = NetStat(32, 274, 10, 52);
    NetStat true_stat02 = NetStat(30, 271, 15, 72);
    NetStat true_stat12 = NetStat(38, 270, 11, 42);

    Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphStream);
    ASSERT_EQ(graph.getNodes(), 3);

    NetStat stat_edge01 = graph.getWeightBetween(node0, node1);
    NetStat stat_edge02 = graph.getWeightBetween(node0, node2);
    NetStat stat_edge12 = graph.getWeightBetween(node1, node2);
    ASSERT_EQ(stat_edge01, true_stat01);
    ASSERT_EQ(stat_edge02, true_stat02);
    ASSERT_EQ(stat_edge12, true_stat12);
}

TEST(GraphParserTest, ParseReorderedColumns) {
    std::string csv_data =
        "Delay,Jitter,Loss,Throughput,Target,Source\n"
        "99,10,5,500,NodeY,NodeX\n";

    std::istringstream graphStream(csv_data);
    Graph<NetStat> graph = GraphParser::parseCSVToGraph(graphStream);

    ASSERT_EQ(graph.getNodes(), 2);

    Node nodeX("NodeX");
    Node nodeY("NodeY");
    NetStat true_stat(500,5,99,10);
    NetStat stat_edge = graph.getWeightBetween(nodeX, nodeY);

    ASSERT_EQ(stat_edge, true_stat);
}

TEST(GraphParserTests, ThrowsOnEmptyBody){
    std::string csv_data = "Source,Target,Throughput,Loss,Delay,Jitter\n";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), CSVParserException);
}

TEST(GraphParserTests, ThrowsOnMissingHeaderColumns){
    std::string csv_data = "Source,Target,Throughput,Jitter\n0,1,32,274,10,52\n";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), io::error::missing_column_in_header);
}

TEST(GraphParserTests, ThrowsOnNoHeader){
    std::string csv_data = "0,1,32,274,10,52\n";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), io::error::extra_column_in_header);
}

TEST(GraphParserTests, ThrowsOnEmpyStream){
    std::string csv_data = "";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), io::error::header_missing);
}

TEST(GraphParserTests, ThrowsOnIllegalString) {
    std::string csv_data =
        "Source,Target,Throughput,Loss,Delay,Jitter\n"
        "0,1,Two,274,10,52\n"
        "0,2,30,271,15,72\n"
        "1,2,38,270,11,42";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), io::error::base);
}

TEST(GraphParserTests, ThrowsOnIllegalNegativeInt) {
    std::string csv_data =
        "Source,Target,Throughput,Loss,Delay,Jitter\n"
        "0,1,12,274,-10,52\n"
        "0,2,30,271,15,72\n"
        "1,2,38,270,11,42";
    std::istringstream graphStream(csv_data);

    EXPECT_THROW(GraphParser::parseCSVToGraph(graphStream), io::error::base);
}