#include "parser.h"

#include <set>

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
    eatToken(tokens);
    std::unique_ptr<Node> base(parseAssign(tokens));
    return base.release();
}

Node* Parser::parseOperand(std::deque<Token>& tokens) {

}

Node* Parser::parseAssign(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseIor(tokens));

    while (this->nextToken.getToken() == "=") {
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseAssign(tokens));
        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Error: Invalid assignment target." << std::endl;
            throw(2);
        }
        std::unique_ptr<AssignNode> assignmentTree(new AssignNode());
        assignmentTree->subNodeAdd(leftSide.release());
        assignmentTree->subNodeAdd(rightSide.release());
        leftSide.reset(assignmentTree.release());
    }
    return leftSide.release();
}

Node* Parser::parseEquals(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseCompare(tokens));

    while (this->nextToken.getToken() == "==" || this->nextToken.getToken() == "!=") {
        std::string operation = nextToken.getToken();
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseCompare(tokens));

        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Invalid comparison" << std::endl; // Possibly change error message later
            throw(2);
        }
    }
}

Node* Parser::parseCompare(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parsePlusMinus(tokens));

    std::set<std::string> validComparisonOps = {"<", "<=", ">", ">="};
    std::string comparisonOp = nextToken.getToken();
    while (validComparisonOps.find(comparisonOp) != validComparisonOps.end()) {
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parsePlusMinus(tokens));

        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Parse erorr" << std::endl; // Possibly add more details later
            throw(2);
        }

        std::unique_ptr<OpNode> operatorNode(new OpNode(comparisonOp));
        operatorNode->subNodeAdd(leftSide.release());
        operatorNode->subNodeAdd(rightSide.release());
    }
    return leftSide.release();
}

Node* Parser::parsePlusMinus(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseDivMult(tokens));

    std::string operation = "";
    if (this->nextToken.getToken() == "-" || this->nextToken.getToken() == "+") {
        operation = this->nextToken.getToken();
    }
    while (operation == "-" || operation == "+") {
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseDivMult(tokens));

        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Error" << std::endl; // possibly add more details later
            throw(2);
        }
        std::unique_ptr<OpNode> operatorNode(new OpNode(operation));
        operatorNode->subNodeAdd(leftSide.release());
        operatorNode->subNodeAdd(rightSide.release());
        leftSide.reset(operatorNode.release());
    }
    return leftSide.release();
}

Node* Parser::parseDivMult(std::deque<Token>& tokens) {

}

Node* Parser::parseIor(std::deque<Token>& tokens) {

}

Node* Parser::parseXor(std::deque<Token>& tokens) {

}

Node* Parser::parseAnd(std::deque<Token>& tokens) {

}