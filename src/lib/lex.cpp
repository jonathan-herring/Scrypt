
#include "lex.h"

std::vector<Token> Lexer::tokenize(std::string input) {
    std::string temp = "";
    std::vector<Token> tokens;
    Token token;
    int line = 1;
    int col = 0;
    bool partOfVar = false;

    for (size_t i = 0; (unsigned)i < input.length(); i++) {
        col++;
        temp += input[i];
        //std::cout << "temp: " << temp << " line: " << token.getLine() << " col " << token.getCol() << std::endl;

        //bools
        if (temp == "true" || temp == "false" || temp == "if" || temp == "while" || temp == "print" || temp == "else") {
            token = Token(line, col - temp.length() + 1, temp);
            tokens.push_back(token);
            temp = "";

            // Skip over the characters that were part of the "true" or "false" token
            i += temp.length();

            continue;
        }
        else if(input[i] == '{' || input[i] == '}'){
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
        else if (input[i] == '=') {
            if (i + 1 < input.length() && input[i-1] != '=' && input[i+1] != '=' && input[i-1] != '!' &&  // = operator
                input[i+1] != '!' && input[i-1] != '<' && input[i-1] != '>') {
                token = Token(line, col, temp);
                tokens.push_back(token);
                temp = "";
            }
            else if (input[i-1] == '=') {      // == operator
                if(tokens.back().getToken() != "<=" && tokens.back().getToken() != ">="  && // if previous token uses = prevent it from incorrectly printing == instead of =
                tokens.back().getToken() != "!=" && tokens.back().getToken() != "=="){
                    token = Token(line, col - 1, "==");
                    tokens.push_back(token);
                    temp = "";
                }
                else if(input[i+1] != '='){ //prevents print of = when next node would make it ==
                   token = Token(line, col, temp);
                    tokens.push_back(token);
                    temp = ""; 
                }
            }
            else if (input[i-1] == '!') {      // != operator
                token = Token(line, col - 1, "!=");
                tokens.push_back(token);
                temp = "";
            }
            else if (input[i-1] == '<') {      // <= operator
                token = Token(line, col - 1, "<=");
                tokens.pop_back();
                tokens.push_back(token);
                temp = "";
            }
            else if (input[i-1] == '>') {      // >= operator
                token = Token(line, col - 1, ">=");
                tokens.pop_back();            //THIS AND PREVIOUS POP_BACK COULD MESS WITH CODE LATER
                tokens.push_back(token);
                temp = "";
            }
        }
        else if ((input[i] == '<' || input[i] == '>') && i + 1 < input.length()) {
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '^') {            // TODO: Add token for XOR in token.cpp
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '|') {            // TODO: Add token for OR in token.cpp
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (isdigit(input[i]) || input[i] == '.') {
            // push full number
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
        else if ((input[i] == '+') || (input[i] == '-') || (input[i] == '*') || (input[i] == '/') || input[i] == '%') { // Add modulo and token for modulo
            token = Token(line, col, temp);
            tokens.push_back(token);
            temp = "";
        }
        else if (input[i] == '&') { // TODO: Add token for AND in token.cpp
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

    //check if the lex overall is valid (has float, identifier, or bool)
    bool validValue = false;
    for(size_t i = 0; i < tokens.size(); i++){
        if(tokens[i].getType() == "float" || tokens[i].getType() == "identifier"){ //ADD TRUE FALSE TO THIS
            validValue = true;
        }
    }
    if(!validValue){
        std::cout << "Syntax error on line " << token.getLine() << " column " << token.getCol() << ".\n";
        throw(1);
    }


    return tokens;
}