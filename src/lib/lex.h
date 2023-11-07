#ifndef LEX_H
#define LEX_H

#include "token.h"
#include <vector>

class Lexer
{
    public:
        Lexer() {};
        std::vector<Token> tokenize(std::string input);
};

#endif