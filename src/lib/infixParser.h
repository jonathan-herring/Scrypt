#ifndef INFIXPARSER_H
#define INFIXPARSER_H

#include "token.h"
#include "node.h"

#include <iostream>
#include <stack>
#include <vector>
#include <algorithm>
#include <map>

//Builds an AST given a vector of tokens in infix notation
Node* buildAST(std::vector<Token>& tokens) {
    std::stack<Node*> nodeStack;
    std::stack<Token> opStack;

    for (Token token : tokens) {
        if (token.getType() == "ERROR") {
            return nullptr;
        } else if (token.getType() == "float" || token.getType() == "identifier") {
            nodeStack.push(new Node(token));
        } else if (token.getType() == "binOp" || token.getType() == "assignment") {
            while (!opStack.empty() && opStack.top().getType() != "parenthesis" && (opStack.top().getPrecedence() > token.getPrecedence() || (opStack.top().getPrecedence() == token.getPrecedence() && token.getType() != "assignment"))) {
                Token op = opStack.top();
                opStack.pop();

                Node* right = nodeStack.top();
                nodeStack.pop();
                Node* left = nodeStack.top();
                nodeStack.pop();

                Node* opNode = new Node(op);
                opNode->left = left;
                opNode->right = right;

                nodeStack.push(opNode);
            }

            opStack.push(token);
        } else if (token.getType() == "parenthesis" && token.getToken() == "(") {
            opStack.push(token);
        } else if (token.getType() == "parenthesis" && token.getToken() == ")") {
            while (opStack.top().getType() != "parenthesis" || opStack.top().getToken() != "(") {
                Token op = opStack.top();
                opStack.pop();

                Node* right = nodeStack.top();
                nodeStack.pop();
                Node* left = nodeStack.top();
                nodeStack.pop();

                Node* opNode = new Node(op);
                opNode->left = left;
                opNode->right = right;

                nodeStack.push(opNode);
            }

            opStack.pop();
        }
    }

    while (!opStack.empty()) {
        Token op = opStack.top();
        opStack.pop();

        Node* right = nodeStack.top();
        nodeStack.pop();
        Node* left = nodeStack.top();
        nodeStack.pop();

        Node* opNode = new Node(op);
        opNode->left = left;
        opNode->right = right;

        nodeStack.push(opNode);
    }

    return nodeStack.top();
}

void printAST(Node* node) {
    if (node == nullptr) {
        return;
    }
    if (node->left == nullptr && node->right == nullptr) {
        std::cout << node->token.getToken();
        return;
    }
    std::cout << "(";
    printAST(node->left);
    std::cout << " " << node->token.getToken() << " ";
    printAST(node->right);
    std::cout << ")";
}

double evaluateASTHelper(Node* root, std::map<std::string, double>& tempVariables) {
    if (!root) {
        std::cerr << "Error: Null node encountered." << std::endl;
        throw 3;
    }

    //If node is float
    if (root->token.getType() == "float") {
        return std::stod(root->token.getToken());
    }
    
    //If node is variable
    else if (root->token.getType() == "identifier") {
        if (tempVariables.find(root->token.getToken()) == tempVariables.end()) {
            std::cout << "Runtime error: unknown identifier " << root->token.getToken() << std::endl;
            throw 3;
        }
        return tempVariables[root->token.getToken()];
    }

    //If node is operator or =
    else if (root->token.getType() == "binOp" || root->token.getType() == "assignment") {
        double leftValue;
        double rightValue = evaluateASTHelper(root->right, tempVariables);  // Evaluate the right child first

        if (root->token.getToken() == "=") {
            if (root->left->token.getType() != "identifier") {
                std::cout << "Error: Invalid assignment target." << std::endl;
                throw 2;
            }
            std::string variableName = root->left->token.getToken();
            tempVariables[variableName] = rightValue;  // Assign the value to the identifier in the tempVariables map
            return rightValue;
        }

        leftValue = evaluateASTHelper(root->left, tempVariables);  // Then evaluate the left child

        std::string op = root->token.getToken();
        if (op == "+") {
            return leftValue + rightValue;
        } else if (op == "-") {
            return leftValue - rightValue;
        } else if (op == "*") {
            return leftValue * rightValue;
        } else if (op == "/") {
            if (rightValue == 0.0) {
                std::cout << "Runtime error: division by zero." << std::endl;
                throw 3;
            }
            return leftValue / rightValue;
        } else {
            std::cout << "Error: Unknown binary operation " << op << std::endl;
            throw 3;
        }
    }

    else {
        std::cout << "Error: Unknown node type." << std::endl;
        throw 3;
    }
}

void deleteAST(Node* node) {
    if (!node) return;

    deleteAST(node->left);
    deleteAST(node->right);

    delete node;
}

double evaluateAST(Node* root, std::map<std::string, double>& variables) {
    std::map<std::string, double> tempVariables(variables); // Create a copy of variables

    double result = evaluateASTHelper(root, tempVariables);

    // If no errors, merge the temporary variables with the main variable map
    for (const auto& pair : tempVariables) {
        variables[pair.first] = pair.second;
    }

    return result;
}

bool isValidExpression(std::vector<Token>& tokens) {
    // Check if there are enough tokens to form a valid expression
    if (tokens.empty()) return false;

    // Count open and close parentheses
    int openParens = 0;

    for (size_t i = 0; i < tokens.size(); i++) {
        Token& token = tokens[i];

        // Check for mismatched or unbalanced parentheses
        if (token.getType() == "parenthesis") {
            if (token.getToken() == "(") {
                openParens++;
            } else if (token.getToken() == ")") {
                openParens--;
                if (openParens < 0) return false; // Unmatched right parenthesis
            }
        }

        // Check for leading or trailing operators
        if (token.getType() == "binOp" || token.getType() == "assignment") {
            // Leading operator
            if (i == 0) return false;

            // Trailing operator
            if (i == tokens.size() - 1) return false;

            // Two consecutive operators (excluding unary operators like `-1`)
            if (tokens[i + 1].getType() == "binOp" || tokens[i + 1].getType() == "assignment") {
                return false;
            }
        }
    }

    if (openParens != 0) return false; // Unmatched left parenthesis

    return true;
}

#endif