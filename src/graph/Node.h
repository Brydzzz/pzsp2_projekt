//
// Created by tomek on 12/9/25.
//

#ifndef NODE_H
#define NODE_H
#include <string>


class Node {
public:
    std::string name;
    bool operator==(const Node &other) const {
        return this->name == other.name;
    }
    bool operator<(const Node &other) const {
        return this->name < other.name;
    }
};



#endif //NODE_H
