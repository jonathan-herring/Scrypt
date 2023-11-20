#ifndef NODE_H
#define NODE_H

#include "token.h"

#include <map>

class ReturnValue { // Different evaluates return different values, so we need this 
    private:
        double numVal;
        bool boolVal;

    public:
        ReturnValue(double numVal) {this->numVal = numVal;};
        ReturnValue(bool boolVal) {this->boolVal = boolVal;};
        ReturnValue() {};
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