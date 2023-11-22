#include "lib/parser.h"
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
    std::deque<Token> tokens;

    try {
        tokens = lex.tokenize(input);
    }
    catch (int exitCode) {
        // Lex error
        if (exitCode == 1) {
            exit(1);
        }
    }

    Parser parse;
    try {
        parse.
    }

    return 0;
}