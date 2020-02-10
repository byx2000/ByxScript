#include "OperandStack.h"

#include <vector>

using namespace std;

void OperandStack::push(const Value& val)
{
	operands.push(val);
}

Value OperandStack::pop()
{
	Value val = operands.top();
	operands.pop();
	return val;
}

std::string OperandStack::toString() const
{
	string s = "operand stack:\n";
	s += "Type\tValue\n";
	
	stack<Value> t = operands;
	while (!t.empty())
	{
		s += t.top().toString();
		s += "\n";
		t.pop();
	}

	return s;
}

void OperandStack::checkStack() const
{
	if (operands.empty())
	{
		throw PopWhenStackEmpty();
	}
}
