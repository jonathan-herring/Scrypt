#include "node.h"

#include <cmath>

std::string Node::getIDName() {
    std::cout << "Runtime error" << std::endl; // Add more detail?
}

bool ReturnValue::operator == (ReturnValue rightValue) {
    if (rightValue.getType() != this->getType()) {
        return false;
    }

    // Possible add a case for Function type
    if (this->getType() == Boolean) {
        return (std::get<bool>(rightValue.getVal()) == std::get<bool>(this->getVal()));
    }
    else if (this->getType() == Number) {
        return (std::get<double>(rightValue.getVal()) == std::get<double>(this->getVal()));
    }
    else if (this->getType() == Nothing) {
        return true;
    }
    else if (this->getType() == Undefined) {
        throw(3); // Possibly add a detailed cout statement later on
    }
}

bool ReturnValue::operator != (ReturnValue rightValue) {
    if (rightValue == *this) {
        return false;
    } else {
        return true;
    }
}

ReturnValue::ReturnValue() {
    this->returnType = Undefined;
}

ReturnValue::ReturnValue(double numVal) {
    this->returnVal = numVal;
    this->returnType = Number;
}

ReturnValue::ReturnValue(bool boolVal) {
    this->returnVal = boolVal;
    this->returnType = Boolean;
}

ReturnValue::ReturnValue(std::nullptr_t) {
    this->returnType = Nothing;
}

// Possibly implement ReturnValue for functions



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

void OpNode::subNodeAdd(Node* nodeToAdd) {
    if (this->left == nullptr) {
        this->left = nodeToAdd;
    }
    else if (this->right == nullptr) {
        this->right = nodeToAdd;
    }
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
    ReturnValue leftValue(this->left->evaluate(variableMap));
    ReturnValue rightValue(this->right->evaluate(variableMap));

    if (leftValue.getType() == Number) {

        double left = std::get<double>(leftValue.getVal());
        double right = std::get<double>(rightValue.getVal());

        if (this->op == ">=") {
            return ReturnValue(left >= right);
        }
        else if (this->op == "<=") {
            return ReturnValue(left <= right);
        }
        else if (this->op == ">") {
            return ReturnValue(left > right);
        }
        else if (this->op == "<") {
            return ReturnValue(left < right);
        }
    }
    else if (leftValue.getType() == Boolean) {

        double left = std::get<bool>(leftValue.getVal());
        double right = std::get<bool>(rightValue.getVal());

        if (this->op == "^") {
            return ReturnValue((std::get<bool>(leftValue.getVal()) && !std::get<bool>(rightValue.getVal())) 
            || (!std::get<bool>(leftValue.getVal()) && std::get<bool>(rightValue.getVal())));
        }
        else if (this->op == "|") {
            return ReturnValue(std::get<bool>(leftValue.getVal()) || std::get<bool>(rightValue.getVal()));
        }
        else if (this->op == "&") {
            return ReturnValue(std::get<bool>(leftValue.getVal()) && std::get<bool>(rightValue.getVal()));
        }
    }
    else if (this->op == "==") {
        return (this->left == this->right);
    }
    else if (this->op == "!=") {
        return (this->left != this->right);
    }
    throw(3); // Should not be reached
}

ReturnValue OpNode::evaluateBinOp(std::map<std::string, ReturnValue> variableMap) {
    ReturnValue leftValue(this->left->evaluate(variableMap));

    if (leftValue.getType() != Number) {
        throw(3);
    }

    double calculation = std::get<double>(leftValue.getVal());

    if (op == "+") {
        ReturnValue rightValue(this->right->evaluate(variableMap));
        if(rightValue.getType() != Number) {
            throw(3);
            // Needs only numbers
        }
        calculation += std::get<double>(rightValue.getVal());
    }
    else if (op == "-") {
        ReturnValue rightValue(this->right->evaluate(variableMap));
        if (rightValue.getType() != Number) {
            throw(3);
        }
        calculation -= std::get<double>(rightValue.getVal());
    }
    else if (op == "/") {
        ReturnValue rightValue(this->right->evaluate(variableMap));
        if(rightValue.getType() != Number) {
            throw(3);
        }

        if (std::get<double>(rightValue.getVal()) == 0) {
            std::cout << "Runtime error: division by zero" << std::endl;
            throw(3);
        }

        calculation /= std::get<double>(rightValue.getVal());
    }
    else if (op == "%") {
        ReturnValue rightValue(this->right->evaluate(variableMap));
        if(rightValue.getType() != Number) {
            throw(3);
        }
        if (std::get<double>(rightValue.getVal()) == 0){
            std::cout << "Runtime error: division by zero" << std::endl;
            throw(3);
        }
        calculation = fmod(calculation, std::get<double>(rightValue.getVal()));
    }
    else if (op == "*") { 
        ReturnValue rightValue(this->right->evaluate(variableMap));
        if(rightValue.getType() != Number){
            throw(3);
        }
        calculation *= std::get<double>(rightValue.getVal());
    }

    return ReturnValue(calculation);
}



IdentifierNode::IdentifierNode(std::string ID) {
    this->ID = ID;
}

std::string IdentifierNode::getIDName() {
    this->getIDName();
}

void IdentifierNode::print(size_t depth) {
    for (size_t i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << this->getIDName();
}

ReturnValue IdentifierNode::evaluate(std::map<std::string, ReturnValue> variableMap) {
    if (variableMap.find(this->getIDName()) == variableMap.end()) {
        throw(3); // Could not find identifier value in map
    } else {
        return variableMap[this->getIDName()];
    }
}



BoolNode::BoolNode(std::string boolValue) {
    this->boolValue = boolValue;
}

void BoolNode::print(size_t depth) {
    for (size_t i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    std::cout << this->boolValue;
}

ReturnValue BoolNode::evaluate(std::map<std::string, ReturnValue> variableMap) {
    if (this->boolValue == "true") {
        return ReturnValue(true);
    } else{
        return ReturnValue(false);
    }
}



AssignNode::AssignNode() {
    this->left = nullptr;
    this->right = nullptr;
}

void AssignNode::subNodeAdd(Node* nodeToAdd) {
    if (this->left == nullptr) {
        this->left = nodeToAdd;
    }
    else if (this->right == nullptr) {
        this->right = nodeToAdd;
    }
}

void AssignNode::print(size_t depth) {
    for (size_t i = 0; i < depth; i++) {
        std::cout << "    ";
    }
    std::cout << "(";
    this->left->print(0);
    if (this->right) {
        std::cout << " = ";
        this->right->print(0);
    }
    std::cout << ")";
}

ReturnValue AssignNode::evaluate(std::map<std::string, ReturnValue> variableMap) {
    ReturnValue rightValue(this->left->evaluate(variableMap));

    if (this->left) {
        variableMap[this->right->getIDName()] = rightValue;
    }
    return rightValue;
}

Function::Function(std::map<std::string, ReturnValue> variableMap, std::vector<std::string> parameters, 
    std::shared_ptr<std::vector<Node*>> statements) {

    this->variableMap   = new std::map<std::string, ReturnValue>(variableMap);
    this->parameters = parameters;
    this->statements = statements;
}

Function::~Function() {
    delete this->variableMap;
    if (this->statements.use_count() == 1) {
        for (Node* subNode : *this->statements) {
            delete subNode;
        }
    }
    this->statements.reset();
}


