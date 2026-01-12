//
// Created by tomek on 12/9/25.
//

#pragma once
#include <string>

class Node {
  public:
    explicit Node(std::string _name) : name(_name) {}
    std::string name;
    bool operator==(const Node &other) const {
        return this->name == other.name;
    }
    bool operator!=(const Node &other) const { return !(*this == other); }
    bool operator<(const Node &other) const { return this->name < other.name; }

    friend std::ostream &operator<<(std::ostream &os, const Node &node) {
        os << node.name;
        return os;
    }

    friend std::istream &operator>>(std::istream &is, Node &node) {
        is >> node.name;
        return is;
    }
};
