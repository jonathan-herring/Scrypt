#include "lib/parser.h"
#include "lib/lex.h"
#include <iostream>

int main() {
    std::map<std::string, ReturnValue> variableMap;
    std::string input = "";
    std::deque<Token> tokens;
    Lexer lex;
    Parser parse;

    while (getline(std::cin, input)) {

        // Check for Lex errors
        try {
            tokens = lex.tokenize(input);
        }
        catch (int exitCode) {
            if (exitCode == 1) {
                continue;
            }
        }
        // Check for Parse errors

        std::unique_ptr<Node> head;
        
        try {
            std::map<std::string, ReturnValue> volatileVars = variableMap;

            head.reset(parse.parse(tokens));

            // Print
            head->print(1);
            std::cout << std::endl;

            // Evaluation
            ReturnValue result(head->evaluate(volatileVars));

            if (result.getType() == Number) {
                std::cout << std::get<double>(result.getVal()) << std::endl;
            } 
            else if (result.getType() == Boolean) {
                if (std::get<bool>(result.getVal())) {
                    std::cout << "true" << std::endl;
                }
                else if (!(std::get<bool>(result.getVal()))) {
                    std::cout << "false" << std::endl;
                }
            }
            variableMap = volatileVars;
        }
        catch (int exitCode) {
            if (exitCode == 2) {
                continue;
            }
            if (exitCode == 3) {
                continue;
            }
        }
    }

    return 0;
}