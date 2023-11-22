CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

.PHONY: clean

test: src/test.cpp src/lib/infixParser.h src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o test src/test.cpp src/lib/infixParser.h src/lib/token.cpp src/lib/lex.cpp

testPrefix: src/testPrefix.cpp src/lib/prefixParser.h src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o testPrefix src/testPrefix.cpp src/lib/infixParser.h src/lib/token.cpp src/lib/lex.cpp

calc: src/calc.cpp src/lib/infixParser.h src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o calc src/calc.cpp src/lib/infixParser.h src/lib/token.cpp src/lib/lex.cpp

parse: src/parse.cpp src/lib/parser.cpp src/lib/token.cpp src/lib/lex.cpp src/lib/node.cpp
	g++ $(CXXFLAGS) -o parse src/parse.cpp src/lib/parser.cpp src/lib/token.cpp src/lib/lex.cpp src/lib/node.cpp

lex: src/lex.cpp src/lib/lex.cpp src/lib/token.cpp
	g++ $(CXXFLAGS) -o lex src/lex.cpp src/lib/lex.cpp src/lib/token.cpp

clean:
	rm -rf *.exe parse lex test infix infixParser