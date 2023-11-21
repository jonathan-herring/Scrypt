#include "lib/lex.h"

#include <iostream>

int main () {
    char ch;
    std::string input;
    Lexer lex;
    std::deque<Token> tokens;

    while ((std::cin.peek() != EOF)) {
        std::cin.get(ch);
        input += ch;
    }
    try {
        tokens = lex.tokenize(input);
    }
    catch (int x) {
        if (x == 1) {
            exit(1);
        }
    }
    for (size_t i = 0; i < tokens.size(); i++) {
        tokens[i].print();
    }
    
    return 0;
}