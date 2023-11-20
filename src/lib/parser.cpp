#include "parser.h"



void Parser::eatToken(std::deque<Token>& tokens) {
    if (tokens.empty()) { // Underflow
        std::cout << "Unexpected token at line " << nextToken.getLine() << " column " 
        << nextToken.getCol() << ": " << nextToken.getToken() << std::endl;
        throw(2);
    } else {
        this->nextToken = tokens.front();
        
        if (this->nextToken.getType() == END) {
            this->nextNextToken.setType(END);
        } else {
            this->nextNextToken = tokens[1]; // The token after next token
        }
        tokens.pop_front();
    }
}

Node* Parser::parse(std::deque<Token>& tokens) {

}

Node* Parser::parseOperand(std::deque<Token>& tokens) {

}

Node* Parser::parseAssign(std::deque<Token>& tokens) {

}

Node* Parser::parseEquals(std::deque<Token>& tokens) {

}

Node* Parser::parseCompare(std::deque<Token>& tokens) {

}

Node* Parser::parsePlusMinus(std::deque<Token>& tokens) {

}

Node* Parser::parseDivMult(std::deque<Token>& tokens) {

}

Node* Parser::parseOr(std::deque<Token>& tokens) {

}

Node* Parser::parseXor(std::deque<Token>& tokens) {

}

Node* Parser::parseAnd(std::deque<Token>& tokens) {

}