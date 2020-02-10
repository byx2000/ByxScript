#include "CharStream.h"

using namespace std;

CharStream::CharStream(const string& input)
{
    this->input = input;
    index = 0;
    curRow = 1;
    curCol = 1;
}

char CharStream::now()
{
    return input[index];
}

void CharStream::next()
{
    if (index == (int)input.size())
    {
        return;
    }

    if (input[index] == '\n')
    {
        curRow++;
        curCol = 1;
    }
    else
    {
        curCol++;
    }

    index++;
}

char CharStream::peek()
{
    if (index == (int)input.size())
    {
        return '\0';
    }
    return input[index];
}

bool CharStream::end()
{
    return index == (int)input.size();
}

int CharStream::row()
{
    return curRow;
}

int CharStream::col()
{
    return curCol;
}
