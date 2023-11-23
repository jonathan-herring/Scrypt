#include "statementParser.h"

StatementParser::~StatementParser() {
    while (this->checkForEmptiness() == false) {
        Node* treeToDelete;
        treeToDelete = this->extractStatement();
        delete treeToDelete;
    }
}

void StatementParser::StatementsParse(std::deque<Token> tokens) {
    Token tokAtFront = tokens.front();

    while (tokAtFront.getType() != END) {
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
    }
    else {
        return false;
    }
}

Node* StatementParser::extractStatement() {
    Node* statementToExtract = this->statements.front();
    this->statements.pop_front();
    return statementToExtract;
}

bool StatementParser::checkNext(Token& tokenToCheck) {
    return (tokenToCheck.getType() != statement && tokenToCheck.getType() != END);
}

Node* StatementParser::FormStatement(std::deque<Token>& tokens) {
    Token tokAtFront = tokens.front();

    if (this->checkNext(tokAtFront)) {
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

std::vector<Node*> StatementParser::FormBlock(std::deque<Token>& tokens) {
    std::vector<Node*> block;
    tokens.pop_front();

    Token tokAtFront = tokens.front();
    while (tokAtFront.getToken() != "}") {
        if (tokAtFront.getToken() == "{" || tokAtFront.getToken() == "END") {
            std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
            << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
            throw(2);
        }
        block.push_back(FormStatement(tokens));
    }
    tokens.pop_front();
    return block;
}

Node* StatementParser::PrintStatement(std::deque<Token>& tokens) {
    std::unique_ptr<StatementNode> rootOfStatement(new StatementNode("print"));
    Parser p;
    rootOfStatement->setConditional(p.parseBigWrapper(tokens)); // FIXXXX

    Token tokAtFront = tokens.front();
    if (tokAtFront.getToken() != ";") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(2);
    }
    tokens.pop_front();
    return rootOfStatement.release();
}

Node* StatementParser::ReturnStatement(std::deque<Token>& tokens) {
    std::unique_ptr<StatementNode> rootOfStatement(new StatementNode("return"));
 
    Token tokAtFront = tokens.front();
    if (tokAtFront.getToken() == ";") {
        tokens.pop_front();
        rootOfStatement->setConditional(nullptr);
        return rootOfStatement.release();
    }
    else if (tokAtFront.getToken() == "null") {
        tokens.pop_front();
        if (tokAtFront.getToken() == ";") {
            tokens.pop_front();
            std::unique_ptr<StatementNode> nullPointer(new StatementNode("null"));
            rootOfStatement->setConditional(nullPointer.release());
            return rootOfStatement.release();
        }
        else {
            std::cout << "Unexpected token at line " << tokAtFront.getLine() 
            << " column " << tokAtFront.getLine() << ": " << tokAtFront.getToken() << "." << std::endl;
            throw(2);
        }
    }
    Parser p;
    rootOfStatement->setConditional(p.parseBigWrapper(tokens));
    if (tokAtFront.getToken() != ";") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() 
        << " column " << tokAtFront.getLine() << ": " << tokAtFront.getToken() << "." << std::endl;
    }
    tokens.pop_front();
    return rootOfStatement.release();
}

Node* StatementParser::ExpressionStatement(std::deque<Token>& tokens) {
    std::unique_ptr<StatementNode> rootOfStatement(new StatementNode("expression"));
    Parser p;
    rootOfStatement->setConditional(p.parseBigWrapper(tokens));

    Token tokAtFront = tokens.front();
    if (tokAtFront.getToken() != ";") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(2);
    }
    tokens.pop_front();
    return rootOfStatement.release();
}
Node* StatementParser::FunctionDefinition(std::deque<Token>& tokens) {
    Token tokAtFront = tokens.front();
    if (tokAtFront.getType() != identifier) {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(3);
    }
    std::unique_ptr<DefinitionNode> rootOfDefinition(new DefinitionNode(tokAtFront.getToken()));
    tokens.pop_front();

    if (tokAtFront.getToken() != "(") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(3);
    }
    tokens.pop_front();

    while(tokAtFront.getToken() != ")"){ 
        if (tokAtFront.getType() != identifier) {
            std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
            << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
            throw(3);
        }
        rootOfDefinition->parameters.push_back(tokAtFront.getToken());
        tokens.pop_front();
        if (tokAtFront.getToken() == ")") {
            break;
        }
        if (tokAtFront.getToken() != ",") {
            std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
            << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
            throw(3);
        }
        tokens.pop_front();
        if (tokAtFront.getType() != identifier) {
            std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
            << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
            throw(3);
        }
    }
    tokens.pop_front();
    if(tokAtFront.getToken() != "{") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(3);
    }

    std::shared_ptr<std::vector<Node*>> statementsTemporary = std::make_shared<std::vector<Node*>>(FormBlock(tokens));
    rootOfDefinition->statements = statementsTemporary; 
    return rootOfDefinition.release();
}

Node* StatementParser::IfStatement(std::deque<Token>& tokens) {
    std::unique_ptr<StatementNode> rootOfStatement(new StatementNode("if"));
    Parser p;
    rootOfStatement->setConditional(p.parseBigWrapper(tokens));
    
    Token tokAtFront = tokens.front();
    if(tokAtFront.getToken() != "{") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(2);
        
    }
    rootOfStatement->statementsForTrue = FormBlock(tokens);

    if(tokAtFront.getToken() != "else") {
        return rootOfStatement.release();
    }
    tokens.pop_front();
    if (tokAtFront.getToken() == "if") {
        rootOfStatement->statementsForFalse.push_back(FormStatement(tokens));
    }
    else if (tokAtFront.getToken() == "{") {
        rootOfStatement->statementsForFalse = FormBlock(tokens);
    }
    else {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(2);
    }
    return rootOfStatement.release();
}

Node* StatementParser::WhileStatement(std::deque<Token>& tokens) {
    std::unique_ptr<StatementNode> rootOfStatement(new StatementNode("while"));
    Parser p;
    rootOfStatement->setConditional(p.parseBigWrapper(tokens));
    Token tokAtFront = tokens.front();
    if (tokAtFront.getToken() != "{") {
        std::cout << "Unexpected token at line " << tokAtFront.getLine() << " column " 
        << tokAtFront.getCol() << ": " << tokAtFront.getToken() << std::endl;
        throw(2);
    }
    rootOfStatement->statementsForTrue = FormBlock(tokens);
    return rootOfStatement.release();
}

