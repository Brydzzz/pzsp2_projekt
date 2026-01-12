#include "Individual.h"

Individual::Individual(std::vector<std::vector<Node>> &paths, const Graph<NetStat>& graph) 
                        : paths(paths), graph(graph) 
{
    for (const Node& node : graph.getNodes())
    {
        for (const Edge<NetStat> edge : graph.getEdgesOf(node))
        {
            flow_left[edge] = edge.toInt();
        }
    }
}