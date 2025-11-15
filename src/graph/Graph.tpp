//
// Created by tomek on 11/15/25.
//

template<typename T>
Graph<T>::Graph(int nodes) {
    if (nodes < 0) {
        throw std::invalid_argument("Nodes number has to be positive or zero");
    }
    adjacencyMatrix = std::vector<std::vector<T>>(nodes,std::vector<T>(nodes, T()));
}

template<typename T>
Graph<T>::Graph() {
    Graph(0);
}


template<typename T>
int Graph<T>::getNodes() {
    return adjacencyMatrix.size();
}

template<typename T>
void Graph<T>::addEdge(int node1, int node2, T weight) {
    adjacencyMatrix[node1][node2] = weight;
}


template<typename T>
T Graph<T>::getWeightBetween(int node1, int node2) {
    return adjacencyMatrix[node1][node2];
}


