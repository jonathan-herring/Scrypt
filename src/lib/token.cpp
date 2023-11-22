#include "token.h"

//Creates token from a value
Token::Token(int l, int c, std::string tokenID) {
    this->line = l;
    this->col = c;
    this->tokenID = tokenID;

    if ((tokenID == "(") || (tokenID == ")")) {
        this->type = parenthesis;
    } else if (tokenID == "+" || tokenID == "-" || tokenID == "*" || tokenID == "/" || tokenID == "%") {
        this->type = op;
    } else if (tokenID == "END") {
        this->type = END;
    } else if (isFloat(tokenID)) {
        this->type = number;
    } else if (tokenID == "=") {
        this->type = op;
    } else if (isIdentifier(tokenID)) {
        this->type = identifier;
    } else if (tokenID == "<" || tokenID == "<=" || tokenID == ">" || tokenID == ">=") {
        this->type = op;
    } else if (tokenID == "==" || tokenID == "!=") {
        this->type = op;
    } else if (tokenID == "&") {
        this->type = op;
    } else if (tokenID == "^") {
        this->type = op;
    } else if (tokenID == "|") {
        this->type = op;
    } else if (tokenID == "if" || tokenID == "while" || tokenID == "print" 
    || tokenID == "else" || tokenID == "def" || tokenID == "return") {
        this->type = statement;
    } else if (tokenID == "null" || tokenID == "," || tokenID == ";") {
        this->type = other;
    } else if (tokenID == "{" || tokenID == "}") {
        this->type = other;
    } else {
        this->type = ERROR;
    }
}

void Token::print() {
    std::cout << std::right << std::setw(4) << line << std::setw(5) << col << "  " << tokenID << std::endl;
}

bool Token::isFloat(const std::string str) {
    // check for trailing or ending decimal
    if (str[str.size() - 1] == '.') {
        this->col += str.size();
        return false;
    } else if (str[0] == '.') {
        return false;
    }

    //Check for multiple decimals
    int dec_count = 0;
    for (int i = 0; (unsigned)i < str.length(); i++) {
        if (str[i] == '.') {
            dec_count++;
            if (dec_count > 1) {
                this->col += i;
                return false;
            }
        } else if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

bool Token::isIdentifier(const std::string str) {
    if (!isalpha(str[0]) && str[0] != '_') {
        return false;
    }
    for (int i = 1; (unsigned)i < str.length(); i++) {
        if (!isalpha(str[i]) && str[i] != '_' && !isdigit(str[i])) {
            return false;
        }
    }
    return true;
}