#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <iomanip>

enum tokenType {
    none,
    parenthesis,
    op,
    number,
    identifier,
    statement,
    booleanVal,
    other,
    bracketArray,
    ERROR,
    END,
};

class Token {
    private:
        std::string tokenID;
        tokenType type;
        int line;
        int col;
    public:
        Token() { tokenID = "", type = none, line = 0, col = 0; };
        Token(int l, int c, std::string tokenID);
        tokenType getType() { return type; };
        void setType(tokenType type) {this->type = type;};
        std::string getToken() { return tokenID; };
        int getLine() { return line; };
        int getCol() { return col; };
        bool isFloat(const std::string str);
        bool isIdentifier(const std::string str);
        void print();
};

#endif