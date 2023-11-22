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

ReturnValue::ReturnValue(std::shared_ptr<Function> function) {
    this->returnVal = function;
    this->returnType = FunctionType;
}



FunctionCallVal::FunctionCallVal(ReturnValue valueToReturn) : std::runtime_error("Runtime error") {
    this->storedValue = valueToReturn;
}

ReturnValue FunctionCallVal::getValue() {
    return this->storedValue;
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


StatementNode::StatementNode(std::string statement) {
    this->statementString = statement;
}

StatementNode::~StatementNode() {
    delete this->conditional;
    for (Node* subNode : this->statementsForFalse) {
        delete subNode;
    }
    for (Node* subNode : this->statementsForTrue) {
        delete subNode;
    }
}


ReturnValue StatementNode::evaluate(std::map<std::string, ReturnValue>& variableMap) {
    if (this->statementString == "return") {
        evaluateReturn(variableMap);
    }
    else if (this->statementString == "if") {
        evaluateIf(variableMap);
    }
    else if (this->statementString == "expression") {
        evaluateExpression(variableMap);
    }
    else if (this->statementString == "print") {
        evaluatePrint(variableMap);
    }
    else if (this->statementString == "while") {
        evaluateWhile(variableMap);
    }
    else {
        return ReturnValue(nullptr);
    }
    return ReturnValue();
}

void StatementNode::print(size_t depth) {
    for (int i = 0; i < depth; ++i) {
        std::cout << "    ";
    }
    if (this->statementString == "if") { 
        std::cout << "if ";
        this->conditional->print(0);
        std::cout << " {";

        for (Node* Statement : this->statementsForTrue) {
            std::cout << std::endl;
            Statement->print(depth + 1);
        } // PLAG MARKER
        std::cout << std::endl;
        for(int i = 0; i < depth; ++i) {
            std::cout << "    ";
        }
        std::cout << "}";
        
        if (this->statementsForFalse.size() > 0) {
            std::cout << std::endl;
            for (int i = 0; i < depth; ++i) {
                std::cout << "    ";
            }
            std::cout << "else {";
            for(Node* Statement : this->statementsForFalse){
                std::cout << std::endl;
                Statement->print(depth + 1);
            }
            std::cout << std::endl;
            for(int i = 0; i < depth; ++i) {
                std::cout << "    ";
            }
            std::cout << "}";
        }
    }
    else if (this->statementString == "while") {
        std::cout << "while ";
        this->conditional->print(0);
        std::cout << " {";

        for(Node* Statement : this->statementsForTrue){
            std::cout << std::endl;
            Statement->print(depth + 1);
        }
        std::cout << std::endl;
        for (int i = 0; i < depth; ++i){
            std::cout << "    ";
        }
        std::cout << "}";
    }
    else if (this->statementString == "print") {
        std::cout << "print ";
        this->conditional->print(0);
        std::cout << ";";
    }
    else if (this->statementString == "return") {
        std::cout << "return";
        if(this->conditional != nullptr){
            std::cout << " ";
            this->conditional->print(0);
        }
        std::cout << ";";
    }
    else if (this->statementString == "expression") {
        this->conditional->print(0);
        std::cout << ";";
    }
    else {
        std::cout << "null";
    }
}

void StatementNode::evaluateExpression(std::map<std::string, ReturnValue>& variableMap) {
    this->conditional->evaluate(variableMap);
    return;
}

void StatementNode::evaluateIf(std::map<std::string, ReturnValue>& variableMap) {
    ReturnValue conditionValue(this->conditional->evaluate(variableMap));

    if (conditionValue.getType() != Boolean) {
        std::cout << "Runtime error" << std::endl;
        throw(3);
    }
    if (std::get<bool>(conditionValue.getVal())){
        for (Node* statement : this->statementsForTrue) {
            statement->evaluate(variableMap);
        }
        return;
    }
    for (Node* statement : this->statementsForFalse){
        statement->evaluate(variableMap);
    }
    return;
}

void StatementNode::evaluateReturn(std::map<std::string, ReturnValue>& variableMap) {
    if (this->conditional == nullptr) {
        throw FunctionCallVal(ReturnValue(nullptr));
    }
    ReturnValue conditionalValue(this->conditional->evaluate(variableMap));
    throw FunctionCallVal(conditionalValue);
}

void StatementNode::evaluateWhile(std::map<std::string, ReturnValue>& variableMap) {
    ReturnValue conditionValue(this->conditional->evaluate(variableMap));

    if (conditionValue.getType() != Boolean) {
        std::cout << "Runtime error" << std::endl;
        throw(3);
    }
    if (std::get<bool>(conditionValue.getVal())) {
        for (Node* statement : this->statementsForTrue) {
            statement->evaluate(variableMap);
        }
        conditionValue = ReturnValue(conditional->evaluate(variableMap));

        if (conditionValue.getType() != Boolean) {
            std::cout << "Runtime error" << std::endl;
            throw(3);
        }
    }
    return;
}

void StatementNode::evaluatePrint(std::map<std::string, ReturnValue>& variableMap) {
    ReturnValue conditionalValue(this->conditional->evaluate(variableMap));

    typeReturnVal conditionalValueType = conditionalValue.getType();
    if (conditionalValueType == Number) {
        double extractedValue = std::get<double>(conditionalValue.getVal());
        std::cout << extractedValue << std::endl;
        return;
    }
    else if (conditionalValueType == Boolean) {
        if(std::get<bool>(conditionalValue.getVal())){
            std::cout << "true" << std::endl;
            return;
        } else {
            std::cout << "false" << std::endl;
            return;
        }
    }
    else if (conditionalValueType == Nothing) {
        std::cout << "null" << std::endl;
        return;
    }
    std::cout << "Can't call print on this" << std::endl;
    throw(3);
}


/*
class DefinitionNode : public Node {
    private:
        std::vector<std::string>  parameters;
        std::string nameOfFunction;
        std::shared_ptr<std::vector<Node*>> statements;
    public:
        DefinitionNode(std::string name);
        ~DefinitionNode();

        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);
};

class CallNode : public Node {
    private:
        Node* functionToCall;
        std::vector<Node*> arguments;
    public:
        CallNode(Node* functionToCall);
        ~CallNode();
        virtual void print(size_t depth);

        void addArguments(std::vector<Node*> argumentsVector);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);
};*/
