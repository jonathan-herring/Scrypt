#ifndef NODE_H
#define NODE_H

#include "token.h"

class Node {
public:
    Node* left;
    Node* right;
    Token token;

    Node(Token token) {
        this->token = token;
        this->left = nullptr;
        this->right = nullptr;
    }
};

#endif