#include "lib/parser.h"
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

    std::string input8 = "(* 8 9 (+ 1 2 3))";

    std::string input9 = "42";
    std::string input10 = "(= x 12)";
    
    std::string input11 = "* 1 2 3";
    std::string input12 = "(2 3 5 7)"; // Parse: Invalid Operation (0/2) - supposed to give an error but doesn't
    std::string input13 = "(=)"; // Parse: Invalid Assignment (0/4) - supposed to give an error but doesn't


    Lexer lex;
    std::vector<Token> tokens = lex.tokenize(input9);

    std::cout << "Made it past lex.tokenize" << std::endl; // DELETE LATER

    std::vector<Node*> heads = constructTrees(tokens);

    std::cout << "Made it past makeAST" << std::endl; // DELETE LATER
    std::cout << heads.size() << std::endl; // DELETE LATER

    std::map<std::string, double> variables;

    for (size_t i = 0; i < heads.size(); i++) {
        printTree(heads[i]);
        std::cout << std::endl;
        std::cout << evaluatePrefix(heads[i], variables) << std::endl;
    }

    for (size_t i = 0; i < heads.size(); i++) {
        printTree(heads[i]);
    }
    

    return 0;
}