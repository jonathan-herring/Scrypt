#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <iomanip>
#include <vector>

enum class tokenType {
    statement,
    boolean,
    binOp, // Includes +, -, *, /, =, and %
    number,
    identifier,
    END,
    ERROR
};

class Token {
    private:
        std::string tokenID;
        std::string type; // Possible types are: "parenthesis", "binOp", "float", "assignment", "identifier", "END", and "ERROR"
        int line;
        int col;
        int precedence;
        bool isFloat(const std::string str);
        bool isIdentifier(const std::string str);
    public:
        Token() { tokenID = "", type = "", line = 0, col = 0; };
        Token(int l, int c, std::string tokenID);
        Token(int l, int c, std::string tokenID, std::string type);
        std::string getType() { return type; };
        std::string getToken() { return tokenID; };
        int getLine() { return line; };
        int getCol() { return col; };
        int getPrecedence() { return precedence; };
        void print();
};

#endif