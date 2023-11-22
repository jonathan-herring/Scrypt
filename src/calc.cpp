#include "lib/parser.h"
#include "lib/lex.h"
#include <iostream>

int main() {

    std::map<std::string, double> variables;
    std::string input = "";
    std::deque<Token> tokens;
    Lexer lex;

    while (getline(std::cin, input)) {

        // Check for Lex errors
        try {
            tokens = lex.tokenize(input);
        } catch (int exitCode) {
            if (exitCode == 1) {
                continue;
            }
        }
        // Check for Parse errors

        Node* head;
        try {
            if (!isValidExpression(tokens))   // New addition - not fullproof
                throw 2;
            head = buildAST(tokens);
        } catch (int exitCode) {
            if (exitCode == 2) {
                deleteAST(head);
                continue;
            }
        }

        printAST(head);
        std::cout << std::endl;
        // Check for Runtime errors
        try {
            std::cout << evaluateAST(head, variables) << std::endl;
        } catch (int exitCode) {
            if (exitCode == 3) {
                deleteAST(head);
                continue;
            }
        }

        deleteAST(head);
    }

    return 0;
}