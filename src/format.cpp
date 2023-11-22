#include "lib/lex.h"
#include "lib/statementParser.h"
#include "lib/parser.h"

int main() {
    char ch;
    std::string userInput = "";

    while (std::cin.get(ch)) {
        userInput += ch;
    }

    Lexer lexer;
    std::deque<Token> tokens;
    try {
        tokens = lexer.tokenize(userInput);
    }
    catch (int errorCode) {
        exit(1);
    }

    StatementParser statementParser;
    try { // Attempts to parse queue
        statementParser.StatementsParse(tokens);
    }
    catch (int errorCode){
        exit(2);
    }

    while (statementParser.checkForEmptiness() == false) {
        Node* tree = statementParser.extractStatement();
        tree->print(0);
        std::cout << std::endl;
        delete tree;
    }

    return 0;
};