#include "GraphParser.h"
#include "csv.h"
#include <set>

Graph<NetStat> GraphParser::parseCSVToGraph(std::istream &source_stream) {
    io::CSVReader<6> in("csv_source_stream", source_stream);
    in.read_header(io::ignore_no_column, "Source", "Target", "Throughput",
                   "Loss", "Delay", "Jitter");
    std::string source;
    std::string target;
    unsigned int throughput;
    unsigned int loss;
    unsigned int delay;
    int jitter;

    std::set<Node> nodes{};
    std::vector<Edge<NetStat>> edges{};
    while (in.read_row(source, target, throughput, loss, delay, jitter)) {
        Node source_node(source);
        Node target_node(target);
        NetStat stat(throughput, loss, delay, jitter);
        Edge edge(source_node, target_node, stat);
        nodes.insert(source_node);
        nodes.insert(target_node);
        edges.push_back(edge);
    };
    if (edges.empty()) {
        throw CSVParserException("CSV source contains header but no data rows.");
    }

    std::vector<Node> nodes_vec(nodes.begin(), nodes.end());
    return Graph<NetStat>(nodes_vec, edges);
}
