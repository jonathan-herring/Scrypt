#ifndef PREFIX_NODE_H
#define PREFIX_NODE_H

#include "token.h"
#include <vector>

struct Node {
    Token token;
    std::vector<Node*> children;
    Node(Token inToken) : token(inToken) {}
    ~Node() {
        for (Node* child : children) {
            delete child;
        }
    }
};

#endif