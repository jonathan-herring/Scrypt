// #include "lib/parser.h"
// #include "lib/infixParser.h"

// #include <iostream>

// int main () {
//     char ch;
//     std::string input = "";
//     Lexer lex;

//     while ((std::cin.peek() != EOF)) {
//         while (std::cin.peek() != '\n') {
//             std::cin.get(ch);
//             input += ch;
//         }
//         std::vector<Token> lexedVector = lex.tokenize(input);
//         Node* head = infixParse(lexedVector);
//         printAST(head);
//         std::cout << evalAST(head) << std::endl;
//     }

//     return 0;
// }