#include "parser.h"

#include <set>

void Parser::eatToken(std::deque<Token>& tokens) {
    if (tokens.empty()) { // Underflow
        std::cout << "A:Unexpected token at line " << nextToken.getLine() << " column " 
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

Node* Parser::parseBigWrapper(std::deque<Token>& tokens) {
    std::unique_ptr<Node> tree;
    tree.reset(parse(tokens));
    tokens.push_front(nextToken);
    return tree.release();
}

Node* Parser::parseSmallWrapper(std::deque<Token>& tokens) {
    std::unique_ptr<Node> tree;
    tree.reset(parse(tokens));
    if (nextToken.getToken() != "END") {
        std::cout << "B:Unexpected token at line " << nextToken.getLine() << " column " 
        << nextToken.getCol() << ": " << nextToken.getToken() << std::endl;
        throw(2);
    }
    return tree.release();
}

Node* Parser::parseOperand(std::deque<Token>& tokens) {
    if (this->nextToken.getToken() == "") {
        std::cout << "Parse error a" << std::endl; // Add more details later
        throw(2);
    }
    if (this->nextToken.getType() == booleanVal) {
        std::unique_ptr<BoolNode> boolean(new BoolNode(this->nextToken.getToken()));
        eatToken(tokens);
        return boolean.release();
    }
    else if (this->nextToken.getType() == identifier) {
        std::unique_ptr<IdentifierNode> nodeLeaf(new IdentifierNode(nextToken.getToken()));
        eatToken(tokens);
        return nodeLeaf.release();
    }
    else if (this->nextToken.getType() == number) {
        std::unique_ptr<LeafNode> leafNode(new LeafNode(std::stold(nextToken.getToken())));
        eatToken(tokens);
        return leafNode.release();
    }
    else if(tokens[0].getType() == number && tokens.size() == 2){
        std::unique_ptr<LeafNode> leafNode(new LeafNode(std::stold(nextToken.getToken())));
        eatToken(tokens);
        return leafNode.release();
    }
    else if (this->nextToken.getToken() == "(") {
        eatToken(tokens);
        std::unique_ptr<Node> variableNode(parseAssign(tokens));

        if (nextToken.getToken() == ")") {
            eatToken(tokens);
            return variableNode.release();
        } else {
            std::cout << "Parse Error: Missing closing parenthesis" << std::endl;
            throw(2);
        }
    }
    else{
        std::cout << "C:Unexpected token at line " << nextToken.getLine() << " column " << nextToken.getCol() << ": " << nextToken.getToken() << std::endl;
        throw(2);
    }

    return nullptr;
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

        std::unique_ptr<OpNode> operatorNode(new OpNode(operation));
        operatorNode->subNodeAdd(leftSide.release());
        operatorNode->subNodeAdd(rightSide.release());
    }
    return leftSide.release();
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
    std::unique_ptr<Node> leftSide(parseFunction(tokens));

    std::string nextTokenValue = nextToken.getToken();
    while (nextTokenValue == "%" || nextTokenValue == "*" || nextTokenValue == "/") {
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseFunction(tokens));
        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Parse error c" << std::endl;
            throw(2);
        }
        std::unique_ptr<OpNode> operationNode(new OpNode(nextTokenValue));
        operationNode->subNodeAdd(leftSide.release());
        operationNode->subNodeAdd(rightSide.release());
    }
    return leftSide.release();
}

Node* Parser::parseFunction(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftValue(parseOperand(tokens));
    while (this->nextToken.getToken() == "("){
        std::unique_ptr<CallNode> functionNode(new CallNode(leftValue.release()));
        functionNode->addArguments(parseFunctionArguments(tokens));
        leftValue.reset(functionNode.release());
    }
    return leftValue.release();
}

Node* Parser::parseIor(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseXor(tokens));

    while (nextToken.getToken() == "|") {
        std::string operation = this->nextToken.getToken();
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseXor(tokens));
        if (leftSide == nullptr || rightSide == nullptr) {
            std::cout << "Parse error d" << std::endl;
            throw(2);
        }
        std::unique_ptr<OpNode> operationNode(new OpNode(operation));
        operationNode->subNodeAdd(leftSide.release());
        operationNode->subNodeAdd(rightSide.release());
        leftSide.reset(operationNode.release());
    }
    return leftSide.release();
}

Node* Parser::parseXor(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseAnd(tokens));

    while (nextToken.getToken() == "^") {
        std::string operation = this->nextToken.getToken();
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseAnd(tokens));
        if (leftSide == nullptr || rightSide == nullptr) {
            throw(2);
        }
        std::unique_ptr<OpNode> operationNode(new OpNode(operation));
        operationNode->subNodeAdd(leftSide.release());
        operationNode->subNodeAdd(rightSide.release());
        leftSide.reset(operationNode.release());
    }
    return leftSide.release();
}

Node* Parser::parseAnd(std::deque<Token>& tokens) {
    std::unique_ptr<Node> leftSide(parseEquals(tokens));

    while (nextToken.getToken() == "&") {
        std::string operation = nextToken.getToken();
        eatToken(tokens);
        std::unique_ptr<Node> rightSide(parseEquals(tokens));
        if (rightSide == nullptr || leftSide == nullptr) {
            std::cout << "Parse error e" << std::endl;
            throw(2);
        }
        std::unique_ptr<OpNode> operationNode(new OpNode(operation));
        operationNode->subNodeAdd(leftSide.release());
        operationNode->subNodeAdd(rightSide.release());
        leftSide.reset(operationNode.release());
    }
    return leftSide.release();
}

std::vector<Node*> Parser::parseFunctionArguments(std::deque<Token>& tokens) {
    std::string openingBound = nextToken.getToken();
    eatToken(tokens);
    std::string closingBound;
    if (openingBound == "(") {
        closingBound = ")";
    } else {
        closingBound = "]";
    }
    std::vector<Node*> arguments;
    if (nextToken.getToken() == closingBound) {
        eatToken(tokens);
        return arguments;
    }
    arguments.push_back(parseAssign(tokens));
    if (nextToken.getToken() != closingBound && nextToken.getToken() != ",") {
        std::cout << "D:Unexpected token at line " << nextToken.getLine() << " column " 
        << nextToken.getCol() << ": " << nextToken.getToken() << std::endl;
        throw(3);
    }
    while (nextToken.getToken() != closingBound) {
        eatToken(tokens);
        arguments.push_back(parseAssign(tokens));
        if (nextToken.getToken() == closingBound) {
            break;
        }
        if (nextToken.getToken() != ",") {
            std::cout << "E:Unexpected token at line " << nextToken.getLine() << " column " 
            << nextToken.getCol() << ": " << nextToken.getToken() << std::endl;
            throw(3);
        }
    }
    eatToken(tokens);
    return arguments;
}