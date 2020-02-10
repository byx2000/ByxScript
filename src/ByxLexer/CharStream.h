#pragma once

#include <string>

class CharStream
{
public:
    CharStream(const std::string& input);
    char now();
    void next();
    char peek();
    bool end();
    int row();
    int col();
private:
    std::string input;
    int index;
    int curRow, curCol;
};
