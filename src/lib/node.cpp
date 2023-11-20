#include "node.h"

std::string Node::getIDName() {
    std::cout << "Runtime error" << std::endl; // Add more detail?
}



void LeafNode::print(size_t depth) {
    for (size_t i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << this->value;
}

ReturnValue LeafNode::evaluate(std::map<std::string, ReturnValue>& variableMap) {
    return ReturnValue(this->value);
}



OpNode::OpNode(std::string op) {
    this->op = op;
    this->left = nullptr;
    this->right = nullptr;
}

void OpNode::print(size_t depth) {
    for (size_t i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << "(";
    this->left->print(0);
    if (this->right) {
        std::cout << " " << this->op << " ";
        this->right->print(0);
    }
    std::cout << ")";
}

ReturnValue OpNode::evaluate(std::map<std::string, ReturnValue> variableMap) {
    if (this->op == "*" || this->op == "%" || this->op == "/" || this->op == "-" || this->op == "+") {
        return evaluateBinOp(variableMap);
    } else {
        return evaluateComparison(variableMap);
    }
}

ReturnValue OpNode::evaluateComparison(std::map<std::string, ReturnValue> variableMap) {
    
}

ReturnValue OpNode::evaluateBinOp(std::map<std::string, ReturnValue> variableMap) {

}