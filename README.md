# Calculator
CS32 Checkpoint 1

All files are located in the src folder. Inside the src folder, you can find the cpp files used to run the lexer, parser, and calc. All take input from the terminal. Also inside the src folder is the lib folder which contains all of the header files which contains the code that defines the lexter and parser

To use the lexer, pass a string into the lexer() function and it will convert the string into a series of tokens. To avoiding causing an error only include parenthses, basic binary operators (+, -, *, /), and digits within the string.

To use the parser, pass a series of tokens into the constructTree() function in prefix notation and it will create an Abstract Syntax tree (AST). Within the Makefile a string was turned into tokens by using the lexer, then those tokens were turned into an AST using the parser. To evaluate the expression within the AST pass the root of the AST into the evaluateAST() function and it will return the value as a double variable.

To use the calc, pass a series of tokens into the BuildAST() function in infix notation and it will create an AST similar to the one from the parser. Also like the parser, the calc uses string that has been converted into a vector of tokens using the lexer. It can also be evaluated by passing in the root of the AST into the evaluatePrefix() function which will return a double.

The programs can also be ran using Makefile. To compile the lexer type "make lex", to compile the parser type "make parse", to compile the calc type "make calc", and to compile test type "make test". Then to delete all executable run clean by typing "make clean"