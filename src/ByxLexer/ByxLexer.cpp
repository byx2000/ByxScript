#include "ByxLexer.h"

#include <iostream>

using namespace std;

ByxLexer::ByxLexer(const string& input)
    : input(input)
{
    stateProc = BeginState;
    curIndex = 0;
}

void ByxLexer::lex()
{
    if (input == "")
    {
        throw LexError("Empty input.", 0, 0);
    }

    init();

    CharStream cs(input);
    char c;
    while ((c = cs.now()))
    {
        if (stateProc(*this, c, cs.row(), cs.col()))
        {
            cs.next();
        }
    }
    stateProc(*this, c, cs.row(), cs.col());

    if (stateProc != BeginState)
    {
        throw LexError("Unexpeted end of file.", cs.row(), cs.col());
    }

    tokens.push_back(Token(TokenType::End, "", 0, 0));
}

void ByxLexer::addToken(const Token& token)
{
    tokens.push_back(token);
    val.clear();
}

void ByxLexer::addToVal(char c)
{
    val.push_back(c);
}

string ByxLexer::getVal()
{
    return val;
}

void ByxLexer::setCurrentState(StateProc proc)
{
    stateProc = proc;
}

int ByxLexer::row()
{
	return tokens[curIndex].row;
}

int ByxLexer::col()
{
    return tokens[curIndex].col;
}

void ByxLexer::init()
{
    tokens.clear();
    stateProc = BeginState;
    curIndex = 0;
}

void ByxLexer::setInput(const std::string& input)
{
    this->input = input;
}

Token& ByxLexer::next()
{
    static Token end = Token(TokenType::End, "", 0, 0);

    if (curIndex == (int)tokens.size())
    {
        return end;
    }
    return tokens[curIndex++];
}

Token& ByxLexer::peek()
{
    static Token end = Token(TokenType::End, "", 0, 0);

    if (curIndex == (int)tokens.size())
    {
        return end;
    }
    return tokens[curIndex];
}

Token& ByxLexer::read(TokenType type)
{
    Token& token = next();
    if (token.type != type)
    {
        throw LexError(string("Token '") + Token::GetTypeSymbol(type) + "' expected.", token.row, token.col);
    }
    return token;
}

TokenType ByxLexer::nextType()
{
    if (curIndex == (int)tokens.size())
    {
        return TokenType::End;
    }
    return tokens[curIndex].type;
}

std::string ByxLexer::nextVal()
{
    if (curIndex == (int)tokens.size())
    {
        return "";
    }
    return tokens[curIndex].val;
}

void ByxLexer::back()
{
    curIndex--;
    if (curIndex < 0)
    {
        curIndex = 0;
    }
}

bool ByxLexer::end()
{
    return curIndex == tokens.size() - 1;
}

ByxLexer::LexError::LexError(const std::string& msg, int row, int col)
    : msg(msg), row(row), col(col)
{

}
