# Calculator
CS32 Checkpoint 1

All files are located in the src folder. Inside the src folder, you can find the cpp files used to run the lexer and parser. All take input from the terminal. Also inside the src folder is the lib folder which contains all of the header files which contains the code that defines the lexter and parser

To use the lexer, pass a string into the lexer() function and it will convert the string into a series of tokens. To avoiding causing an error only include parenthses, square brackets, operators (+, -, *, /), comparisons (<, <=, >, >=, ==, !=, &, ^, |), equal signs, and digits within the string.

To use the parser, pass a series of tokens into the parse functions in infix notation and it will create an Abstract Syntax tree (AST). Within the Makefile a string was turned into tokens by using the lexer, then those tokens were turned into an AST using the parser. To evaluate the expression within the AST pass the root of the AST into the evaluateAST() function and it will return different types of variables depending on the content of the AST.

To use the test files within the src, pass in string that you wish to be converted into an AST. The lexer will take that string and tokenize it, then the parser functions will build an AST using those tokens. The AST that was build will then be evaluated and a value will be returned. If there is any invalid string values or invalid operations being done within the AST then an error message will be printed and the functions will stop.

The programs can also be ran using Makefile. To compile the lexer type "make lex", to compile the parser type "make parse", to compile the calc type "make calc", and to compile test type "make test". Then to delete all executable run clean by typing "make clean"

To compile the files type "make *filename*", for example to compile the lex file you would type "make lex". Once you are done, to delete all executable run clean by typing "make clean".