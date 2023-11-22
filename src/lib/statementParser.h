#ifndef STATEMENTPARSER_H
#define STATEMENTPARSER_H

#include "parser.h"

#include <vector>

class StatementParser {
    private:
        std::deque<Node*> statements;
        bool checkNext(Token& token);
        Node* FormStatement(std::deque<Token>& tokens);
        std::vector<Node*> FormBlock(std::deque<Token>& tokens);
        Node* PrintStatement(std::deque<Token>& tokens);
        Node* ReturnStatement(std::deque<Token>& tokens);
        Node* ExpressionStatement(std::deque<Token>& tokens);
        Node* FunctionDefinition(std::deque<Token>& tokens);
        Node* IfStatement(std::deque<Token>& tokens);
        Node* WhileStatement(std::deque<Token>& tokens);
    
    public:
        StatementParser() {};
        ~StatementParser();
        void StatementsParse(std::deque<Token> tokens);
        bool checkForEmptiness();
        Node* extractStatement();

};


#endif