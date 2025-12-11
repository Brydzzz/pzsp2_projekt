#pragma once
#include "Edge.h"
#include "Graph.h"
#include "NetStat.h"
#include <istream>
#include <stdexcept>

class GraphParser {
  public:
    static Graph<NetStat> parseCSVToGraph(std::istream &source_stream);
};

class CSVParserException : public std::runtime_error {
  public:
    explicit CSVParserException(const std::string &message)
        : std::runtime_error(message) {}
};
