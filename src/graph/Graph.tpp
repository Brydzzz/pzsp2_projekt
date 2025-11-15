//
// Created by tomek on 11/15/25.
//

template<typename T>
Graph<T>::Graph(int nodes) {
    adjacencyMatrix = std::vector<std::vector<T>>(nodes,std::vector<T>(nodes,0));
}

template<typename T>
Graph<T>::Graph() {
    Graph(0);
}


template<typename T>
int Graph<T>::getNodes() {
    return adjacencyMatrix.size();
}

