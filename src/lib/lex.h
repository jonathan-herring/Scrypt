#ifndef LEX_H
#define LEX_H

#include "token.h"

#include <deque>
class Lexer
{
    public:
        Lexer() {};
        std::deque<Token> tokenize(std::string input);
};

#endif