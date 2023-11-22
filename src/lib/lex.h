#ifndef LEX_H
#define LEX_H

#include "token.h"

#include <deque>


class Lexer
{
    public:
        void tokenize(std::string input);
        std::deque<Token> getDeque();
        void print();

    private:
        std::deque<Token> tokens;
};

#endif