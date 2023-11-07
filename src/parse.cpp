#include "lib/prefixParser.h"
#include "lib/lex.h"

int main () {

    char ch;
    std::string input = "";
    Lexer lex;

    while ((std::cin.peek() != EOF)) {
        std::cin.get(ch);
        input += ch;
    }
    std::map<std::string, double> variables;
    std::vector<Token> tokens;
    std::vector<Node*> astHeads;
    Node* head = nullptr;
    try {
        tokens = lex.tokenize(input);
        astHeads = constructTrees(tokens);
    }
    catch (int exitCode) {
        // Lex error
        if (exitCode == 1) {
            delete head;
            exit(1);
        }
        // Parse error
        if (exitCode == 2) {
            if(head != nullptr){
                delete head;                
            }
            for (size_t i = 0; i < astHeads.size(); i++) {
                deleteAST(astHeads[i]);
            }
            exit(2);
        }
    }
    for (size_t i = 0; i < astHeads.size(); i++) {
        head = astHeads[i];

        try {
            std::cout << evaluatePrefix(head, variables) << std::endl;
            // printInfix(head);
            std::cout << std::endl;
            deleteAST(head);
        }
        catch (int exitCode) {
            // Runtime error
            if (exitCode == 3) {
                deleteAST(head);
                for (size_t i = 0; i < astHeads.size(); i++) {
                    deleteAST(astHeads[i]);
                }
                exit(3);
            }
        }
    }

    return 0;
}