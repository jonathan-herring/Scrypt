#include "lib/infixParser.h"
#include "lib/lex.h"

int main() {
    std::string input1 = "1 + 2 * 3"; // Good
    std::string input2 = "1 + 2 * 3 + 4"; // Good
    std::string input3 = "(1 + 1) * 2"; // Good
    std::string input4 = "(1 + 3 * 4) + (2 + 5 * 6)"; // Bad
    std::string inputA = "1 + 3 * 4 + (2 + 5 * 6)"; // 
    std::string input5 = "a = 1 + 2 * 3";
    std::string input6 = "b = 1";
    std::string input7 = "a + b";

    std::string input8 = "("; // Fails - Seg fault
    std::string input9 = "+1"; // 
    std::string input10 = "2-"; // 
    std::string input11 = "3 +- 4"; //

    std::string input12 = "1+2)"; // 
    std::string input13 = "(3+4"; //


    Lexer lex;
    std::vector<Token> tokens = lex.tokenize(input9);

    std::cout << "Made it past lex" << std::endl;

    if (!isValidExpression(tokens)) {
        std::cout << "Error: Invalid expression." << std::endl;
        return 2;
    }

    Node* head = buildAST(tokens);

    std::cout << "Made it past build" << std::endl;

    printAST(head);

    std::cout << std::endl;

    std::map<std::string, double> variables;
    std::cout << evaluateAST(head, variables) << std::endl;

    return 0;
}