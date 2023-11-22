#ifndef PARSER_H
#define PARSER_H

#include "token.h"
#include "node.h"

#include <iostream>
#include <stack>
#include <deque>
#include <map>

class Parser {
    private:
        Token nextToken;
        Token nextNextToken;
        void eatToken(std::deque<Token>& tokens);

    public:
        Parser();
        ~Parser();

        Node* parse(std::deque<Token>& tokens);
        Node* parseBigWrapper(std::deque<Token>& tokens);
        Node* parseSmallWrapper(std::deque<Token>& tokens);
        Node* parseOperand(std::deque<Token>& tokens); // This includes numbers, parentheses, and identifiers
        Node* parseAssign(std::deque<Token>& tokens); //
        Node* parseEquals(std::deque<Token>& tokens);
        Node* parseCompare(std::deque<Token>& tokens); //
        Node* parsePlusMinus(std::deque<Token>& tokens);
        Node* parseDivMult(std::deque<Token>& tokens);
        Node* parseIor(std::deque<Token>& tokens); // Inclusive or - |
        Node* parseXor(std::deque<Token>& tokens); // Exclusive or - ^
        Node* parseAnd(std::deque<Token>& tokens); //
        Node* parseFunction(std::deque<Token>& tokens);

        // Node* parseFunction(std::deque<Token>& tokens);
        
};

#endif PARSER_H