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
    ERROR,
    END,
};

class Token {
    private:
        std::string tokenID;
        tokenType type; // Possible types are: "parenthesis", "binOp", "float", "assignment", "identifier", "END", and "ERROR"
        int line;
        int col;
        // int precedence; // Not necessary anymore?
        // bool isFloat(const std::string str); // We should know from type checking
        // bool isIdentifier(const std::string str); // Should know from type checking
    public:
        Token() { tokenID = "", type = none, line = 0, col = 0; };
        Token(int l, int c, std::string tokenID);
        // Token(int l, int c, std::string tokenID, tokenType type);
        tokenType getType() { return type; };
        void setType(tokenType type) {this->type = type;};
        std::string getToken() { return tokenID; };
        int getLine() { return line; };
        int getCol() { return col; };
        // int getPrecedence() { return precedence; };
        bool isFloat(const std::string str);
        bool isIdentifier(const std::string str);
        void print();
};



// // Much simpler token -- Lexer will be in charge of handling more

// class Token {
//     std::string tokenID;
//     tokenType tokenType;
//     size_t line;
//     size_t col;
// };

#endif