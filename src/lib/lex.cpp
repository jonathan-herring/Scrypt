#include "lex.h"

std::deque<Token> Lexer::tokenize(std::string input) {
    std::string temp = "";
    std::deque<Token> tokens;
    Token token;
    int line = 1;
    int col = 0;
    bool partOfVar = false;

    for (size_t i = 0; (unsigned)i < input.length(); i++) {
        col++;
        temp += input[i];

        //bools
        if (temp == "true" || temp == "false" || temp == "if" || temp == "while" 
        || temp == "print" || temp == "else" || temp == "def" || temp == "null") {
            token = Token(line, col - temp.length() + 1, temp);
            tokens.push_back(token);
            temp = "";

            i += temp.length();

            continue;
        }
        else if (input[i] == '{' || input[i] == '}' || input[i] == ',' || input[i] == ';'){
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (isalpha(input[i]) || (input[i] == '_') || (partOfVar && isdigit(input[i]))) {
            partOfVar = true;
            if (i + 1 < input.length() && !isalpha(input[i+1]) && !isdigit(input[i+1]) && input[i+1] != '_') {
                token = Token(line, col - temp.length() + 1, temp);
                tokens.push_back(token);
                temp = "";
                partOfVar = false;
            }
        }
        //= operator
        else if (input[i] == '=') {
            if (i + 1 < input.length() && input[i-1] != '=' && input[i+1] != '=' && input[i-1] != '!' &&
                input[i+1] != '!' && input[i-1] != '<' && input[i-1] != '>') {
                token = Token(line, col, temp);
                tokens.push_back(token);
                temp = "";
            }
            //== operator
            else if (input[i-1] == '=') {
                if (tokens.back().getToken() != "<=" && tokens.back().getToken() != ">="  &&
                tokens.back().getToken() != "!=" && tokens.back().getToken() != "=="){
                    token = Token(line, col - 1, "==");
                    tokens.push_back(token);
                    temp = "";
                }
                else if (input[i+1] != '='){
                   token = Token(line, col, temp);
                    tokens.push_back(token);
                    temp = ""; 
                }
            }
            //!= operator
            else if (input[i-1] == '!') {
                token = Token(line, col - 1, "!=");
                tokens.push_back(token);
                temp = "";
            }
            //<= operator
            else if (input[i-1] == '<') {
                token = Token(line, col - 1, "<=");
                tokens.pop_back();
                tokens.push_back(token);
                temp = "";
            }
            //>= operator
            else if (input[i-1] == '>') {
                token = Token(line, col - 1, ">=");
                tokens.pop_back();
                tokens.push_back(token);
                temp = "";
            }
        }
        else if ((input[i] == '<' || input[i] == '>') && i + 1 < input.length()) {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '^') {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '|') {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (isdigit(input[i]) || input[i] == '.') {
            if (i + 1 < input.length() && !isdigit(input[i+1]) && input[i+1] != '.') {
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
        else if ((input[i] == '[') || (input[i] == ']')) {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/') || input[i] == '%') {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '&') {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else {
            if (temp == "\n") {
                line++;
                col = 0;
            }
            else if (temp != " " && temp != "\t" && temp != "!") {
                token = Token(line, col, temp);
                tokens.push_back(token);
            }
            temp = "";
        }
        //Error handling
        if (token.getType() == ERROR) {
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