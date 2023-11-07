
#ifndef PARSER_H
#define PARSER_H

#include "prefixNode.h"
#include <stack>
#include <map>

//Builds an AST given a vector of tokens in prefix notation
Node* constructTree(std::vector<Token> tokens, int& index) {
    if (tokens.size() == 0) {
        std::cout << "Unexpected token at line 1 column 1: END" << std::endl;
        exit(2);
    }
    if (tokens.size() == 1) {
        std::cout << "Unexpected token at line " << tokens[0].getLine() << " column " << tokens[0].getCol() << ": " << tokens[0].getToken() << std::endl;
        exit(2);
    }
    if ((unsigned)index >= tokens.size()) {
        return nullptr;
    }
    if (tokens[index].getType() == "float" || tokens[index].getType() == "identifier") {
        Node* root = new Node(tokens[index]);
        index++;
        return root;
    }

    if (tokens[index].getToken() != "(") {
        std::cout << "Unexpected token at line " << tokens[index].getLine() << " column " << tokens[index].getCol() << ": " << tokens[index].getToken() << std::endl;
        exit(2);
    }

    index++;
    Node* root = new Node(tokens[index]);
    index++;

    // Begin with 1 because of the opening parenthesis which led to this call
    size_t parenthesisCount = 1;

    while (parenthesisCount > 0 && (unsigned)index < tokens.size()) {
        if (tokens[index].getToken() == "(") {
            parenthesisCount++;
            root->children.push_back(constructTree(tokens, index));
        } else if (tokens[index].getToken() == ")") {
            parenthesisCount--;
        } else {
            root->children.push_back(new Node(tokens[index]));
        }
        index++;
    }

    // After processing all tokens:
    if (parenthesisCount != 0) {
        std::cout << "Unexpected token at line 1 column 20: END" << std::endl; // Might not work
        exit(2);
    }

    return root;
}

//Used to create multiple AST given multiple expressions of input
std::vector<Node*> constructTrees(std::vector<Token> tokens) {
    std::vector<Node*> trees;
    int index = 0;

    while ((unsigned)index < tokens.size() && tokens[index].getType() != "END") {
        trees.push_back(constructTree(tokens, index));
    }

    return trees;
}

void printTree(Node* root) {
    if (!root) return;

    bool isOperator = root->token.getType() == "binOp" || root->token.getToken() == "=";

    if (isOperator) {
        for (size_t i = 0; i < root->children.size(); ++i) {
            if (i != 0) {
                std::cout << " " << root->token.getToken() << " ";
            }
            
            if (root->children[i]->token.getType() == "binOp") {
                std::cout << "(";
                printTree(root->children[i]);
                std::cout << ")";
            } else {
                printTree(root->children[i]);
            }
        }
    } else {
        std::cout << root->token.getToken();
    }
}

//Evaluates AST created using infix notation
double evaluatePrefix(Node* root, std::map<std::string, double>& variables) {
    // Base case: if the node is a float
    if (root->token.getType() == "float") {
        return std::stod(root->token.getToken());
    }

    // Base case: if the node is an identifier
    if (root->token.getType() == "identifier") {
        if (variables.find(root->token.getToken()) == variables.end()) {
            throw 3;
        }
        return variables[root->token.getToken()];
    }

    // Recursively evaluate for binary operators and assignment
    if (root->children.size() == 2) {
        double leftValue = evaluatePrefix(root->children[0], variables);
        double rightValue = evaluatePrefix(root->children[1], variables);

        if (root->token.getType() == "binOp") {
            if (root->token.getToken() == "+") return leftValue + rightValue;
            if (root->token.getToken() == "-") return leftValue - rightValue;
            if (root->token.getToken() == "*") return leftValue * rightValue;
            if (root->token.getToken() == "/") {
                if (rightValue == 0.0) throw std::runtime_error("Division by zero.");
                return leftValue / rightValue;
            }
            // ... add more binary operators as needed
        } else if (root->token.getType() == "assignment") {
            // Assuming left child is always an identifier for assignments
            std::string variableName = root->children[0]->token.getToken();
            variables[variableName] = rightValue;
            return rightValue;
        }
    }

    throw std::runtime_error("Invalid AST node.");
}

void deleteAST(Node* head) {
    for (size_t i = 0; i < head->children.size(); i++) {
        deleteAST(head->children[i]);
    }
    delete head;
}

#endif