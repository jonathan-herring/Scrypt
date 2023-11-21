#ifndef NODE_H
#define NODE_H

#include "token.h"

#include <variant>
#include <map>

enum typeReturnVal {
    Number,
    Boolean,
    Nothing, // Null
    Function,
    Undefined
};

class ReturnValue { // Different evaluates return different values, so we need this 
    private:
        typeReturnVal returnType;
        std::variant<bool, double> returnVal;

    public:
        ReturnValue(double numVal);
        ReturnValue(bool boolVal);
        ReturnValue(std::nullptr_t);
        ReturnValue() {};
        // Possible ReturnValue for functions

        typeReturnVal getType() {return this->returnType;};
        std::variant<bool, double> getVal() {return returnVal;};

        bool operator == (ReturnValue);
        bool operator != (ReturnValue);
};

class Node {
    public:
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
        virtual ~Node() {};
        virtual std::string getIDName();
};

class LeafNode : public Node {
    private:
        double value;
    public:
        LeafNode(double value);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);
};

class OpNode : public Node {
    private:
        Node* left;
        Node* right;
        std::string op;
    public:
        OpNode(std::string op);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
        ReturnValue evaluateComparison(std::map<std::string, ReturnValue> variableMap);
        ReturnValue evaluateBinOp(std::map<std::string, ReturnValue> variableMap);
};

class IdentifierNode : public Node {
    private:
        std::string ID;
    public:
        IdentifierNode(std::string ID);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
        virtual std::string getIDName(); // Only class for which getIDName is specially defined for
};

class BoolNode : public Node {
    private:
        bool boolValue;
    public:
        BoolNode(bool boolValue);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
};

class AssignNode : public Node {
    private:
        Node* left;
        Node* right;
    public:
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
};

#endif