#ifndef NODE_H
#define NODE_H

#include "token.h"

#include <variant>
#include <map>
#include <vector>
#include <memory>

enum typeReturnVal {
    Number,
    Boolean,
    Nothing, // Null
    FunctionType,
    Undefined
};

class Function;
class Node;
class ReturnValue { // Different evaluates return different values, so we need this 
    private:
        typeReturnVal returnType;
        std::variant<bool, double, std::shared_ptr<Function>> returnVal;

    public:
        ReturnValue(double numVal);
        ReturnValue(bool boolVal);
        ReturnValue(std::nullptr_t);
        ReturnValue();
        ReturnValue(std::shared_ptr<Function> function);

        typeReturnVal getType() {return this->returnType;};
        std::variant<bool, double, std::shared_ptr<Function>> getVal() {return returnVal;};

        bool operator == (ReturnValue);
        bool operator != (ReturnValue);
};

class Function {
    private:
        std::shared_ptr<std::vector<Node*>> statements;
        std::vector<std::string> parameters;
        std::map<std::string, ReturnValue>* variableMap;

    public:
        Function(std::map<std::string, ReturnValue> variableMap, std::vector<std::string> parameters, 
        std::shared_ptr<std::vector<Node*>> statements);
        ~Function();
        std::vector<std::string> getParameters();
        std::shared_ptr<std::vector<Node*>> getStatements();
        std::map<std::string, ReturnValue>* getVariableMap();
};

class FunctionCallVal: public std::runtime_error {
    private:
        ReturnValue storedValue;

    public:
        FunctionCallVal(ReturnValue valueToReturn);
        ReturnValue getValue();
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
        void subNodeAdd(Node* nodeToAdd);
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
        std::string boolValue;
    public:
        BoolNode(std::string boolValue);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
};

class AssignNode : public Node {
    private:
        Node* left;
        Node* right;
    public:
        AssignNode();
        void subNodeAdd(Node* nodeToAdd);
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue> variableMap);
};

class StatementNode : public Node {
    private:
        Node* conditional;
        std::string statementString;
    public:
        StatementNode(std::string statement);
        ~StatementNode();
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);
        virtual void print(size_t depth);

        void evaluateExpression(std::map<std::string, ReturnValue>& variableMap);
        void evaluateIf(std::map<std::string, ReturnValue>& variableMap);
        void evaluatePrint(std::map<std::string, ReturnValue>& variableMap);
        void evaluateReturn(std::map<std::string, ReturnValue>& variableMap);
        void evaluateWhile(std::map<std::string, ReturnValue>& variableMap);
        void setConditional(Node* conditional);
        std::vector<Node*> statementsForTrue;
        std::vector<Node*>  statementsForFalse;
};

class DefinitionNode : public Node {
    private:
        std::string nameOfFunction;
    public:
        DefinitionNode(std::string name);
        ~DefinitionNode();

        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);
        std::vector<std::string>  parameters;
        std::shared_ptr<std::vector<Node*>> statements;
};

class CallNode : public Node {
    private:
        Node* functionToCall;
        std::vector<Node*> arguments;
    public:
        CallNode(Node* functionToCall);
        ~CallNode();
        virtual void print(size_t depth);
        virtual ReturnValue evaluate(std::map<std::string, ReturnValue>& variableMap);

        void addArguments(std::vector<Node*> argumentsVector);
};

#endif