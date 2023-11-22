#include "statementParser.h"

StatementParser::~StatementParser() {
    while (this->checkForEmptiness() == false) {
        Node* treeToDelete;
        treeToDelete = this->extractStatement();
        delete treeToDelete;
    }
}

void StatementParser::StatementsParse(std::deque<Token> tokens) {
    while(tokAtFront.getType() != END) {
        if(tokAtFront.getType() == other) {
            Token tokAtFront = tokens.front();
            std::cout << "Unexpected token at line " << tokAtFront.getLine() 
            << " column " << tokAtFront.getLine() << ": " << tokAtFront.getToken() << ".";
            throw(2);
        }
        this->statements.push_back(FormStatement(tokens));
    }
}

bool StatementParser::checkForEmptiness() {
    if (this->statements.empty()) {
        return true;
    } else {
        return false;
    }
}

Node* StatementParser::extractStatement() {
    Node* statementToExtract = this->statements.front();
    this->statements.pop_front();
    return statementToExtract;
}

bool checkNext(Token& tokenToCheck) {
    return (tokenToCheck.getType() != statement && tokenToCheck.getType() != END);
}

Node* StatementParser::FormStatement(std::deque<Token>& tokens) {
    if(this->checkNext(tokens.front())){
        return ExpressionStatement(tokens);
    }
    std::string statementString = tokAtFront.getToken();
    tokens.pop_front();
    std::unique_ptr<Node> firstNode(nullptr);

    if (statementString == "def") {
        firstNode.reset(FunctionDefinition(tokens));
    }
    else if (statementString == "return") {
        firstNode.reset(ReturnStatement(tokens));
    }
    else if (statementString == "while") {
        firstNode.reset(WhileStatement(tokens));
    }
    else if (statementString == "print") {
        firstNode.reset(PrintStatement(tokens));
    }
    else if (statementString == "if") {
        firstNode.reset(IfStatement(tokens));
    }
    return firstNode.release();
}

std::vector<Node*> FormBlock(std::deque<Token>& tokens) {

}

Node* PrintStatement(std::deque<Token>& tokens) {

}

Node* ReturnStatement(std::deque<Token>& tokens) {

}

Node* ExpressionStatement(std::deque<Token>& tokens) {

}

Node* FunctionDefinition(std::deque<Token>& tokens) {

}

Node* IfStatement(std::deque<Token>& tokens) {

}

Node* WhileStatement(std::deque<Token>& tokens) {

}

