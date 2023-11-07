#include "lex.h"

std::vector<Token> Lexer::tokenize(std::string input) {
    std::string temp = "";
    std::vector<Token> tokens;
    Token token;
    int line = 1;
    int col = 0;
    bool partOfVar = false;

    for (int i = 0; (unsigned)i < input.length(); i++) {
        col++;
        temp += input[i];
        //Number tokens
        if (isalpha(input[i]) || (input[i] == '_') || (partOfVar && isdigit(input[i]))) {
            partOfVar = true;
            if (!isalpha(input[i+1]) && !isdigit(input[i+1]) && input[i+1] != '_') {
                token = Token(line, col - temp.length() + 1, temp);
                tokens.push_back(token);
                temp = "";
                partOfVar = false;
            }
        }
        else if (input[i] == '=') {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (isdigit(input[i]) || input[i] == '.') {
            // push full number
            if (!isdigit(input[i+1]) && input[i+1] != '.') {
                token = Token(line, col - temp.length() + 1, temp);
                tokens.push_back(token);
                temp = "";
            }
        }
        else if ((input[i] == '(') || (input[i] == ')')) {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/')) {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else {
            if (temp == "\n") {
                line++;
                col = 0;
            }
            else if (temp != " " && temp != "\t") {
                token = Token(line, col, temp);
                tokens.push_back(token);
            }
            temp = "";
        }
        //Error handling
        if (token.getType() == "ERROR") {
            std::cout << "Syntax error on line " << token.getLine() << " column " << token.getCol() << ".\n";
            throw(1);
        }
        //End token
        else if ((unsigned)i == input.length() - 1) {
            if (input[i] == '\n') {
                token = Token(line, 1, "END");
            }
            else {
                token = Token(line, col+1 , "END");
            }
            tokens.push_back(token);
        }
    }
    return tokens;
}