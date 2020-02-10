#pragma once

#include "CharStream.h"
#include "Token.h"
#include "LexerState.h"

#include <string>
#include <vector>

class ByxLexer
{
public:

    // “Ï≥£¿‡
    struct LexError
    {
        std::string msg;
        int row;
        int col;
        LexError(const std::string& msg, int row, int col);
    };

    ByxLexer(const std::string& input);
    void setInput(const std::string& input);
    Token& next();
    Token& peek();
    Token& read(TokenType type);
    TokenType nextType();
    std::string nextVal();
    void back();
    bool end();
    void lex();
    void addToken(const Token& token);
    void addToVal(char c);
    std::string getVal();
    void setCurrentState(StateProc proc);
    int row();
    int col();

private:
    std::string input;
    std::vector<Token> tokens;
    int curIndex;
    std::string val;
    StateProc stateProc;

    void init();
};

