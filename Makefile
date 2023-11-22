CXXFLAGS = -std=c++17 -Wall -Wextra -Werror

.PHONY: clean

calc: src/calc.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o calc src/calc.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp

format: src/format.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o format src/format.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp

scrypt: src/scrypt.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp
	g++ $(CXXFLAGS) -o calc src/calc.cpp src/lib/parser.cpp src/lib/statementParser.cpp src/lib/node.cpp src/lib/token.cpp src/lib/lex.cpp

lex: src/lex.cpp src/lib/lex.cpp src/lib/token.cpp
	g++ $(CXXFLAGS) -o lex src/lex.cpp src/lib/lex.cpp src/lib/token.cpp

parse: src/parse.cpp src/lib/parser.cpp src/lib/token.cpp src/lib/lex.cpp src/lib/node.cpp
	g++ $(CXXFLAGS) -o parse src/parse.cpp src/lib/parser.cpp src/lib/token.cpp src/lib/lex.cpp src/lib/node.cpp

clean:
	rm -rf *.exe