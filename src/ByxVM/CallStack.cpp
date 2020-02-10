#include "CallStack.h"

using namespace std;

CallStack::Frame::Frame(const FunctionTable::Entry& info, int retAddr)
	: info(info), retAddr(retAddr)
{
	// 设置变量表的大小
	varTable.resize(info.space);
}

Value CallStack::Frame::getVar(int index) const
{
	checkIndex(index);
	return varTable[index];
}

void CallStack::Frame::setVar(int index, const Value& value)
{
	checkIndex(index);
	varTable[index] = value;
}

int CallStack::Frame::getReturnAddr() const
{
	return retAddr;
}

int CallStack::Frame::getVarSpace() const
{
	return varTable.size();
}

std::string CallStack::Frame::toString() const
{
	string s = "stack frame:\n\n";
	s += "function info:\nName\tSpace\tAddress\n";
	s += info.toString();
	s += "\n\nvar table:\nIndex\tType\tValue\n";
	for (int i = 0; i < (int)varTable.size(); ++i)
	{
		s += to_string(i);
		s += "\t";
		s += varTable[i].toString();
		s += "\n";
	}
	s += "\nreturn address: ";
	s += to_string(retAddr);
	return s;
}

void CallStack::Frame::checkIndex(int index) const
{
	if (index < 0 || index >= (int)varTable.size())
	{
		throw CallStack::VarIndexOutOfBoundary();
	}
}

void CallStack::push(const FunctionTable::Entry& info, int retAddr)
{
	calls.push(Frame(info, retAddr));
}

int CallStack::pop()
{
	if (calls.empty())
	{
		throw PopWhenStackEmpty();
	}

	int retAddr = calls.top().getReturnAddr();
	calls.pop();
	return retAddr;
}

CallStack::Frame& CallStack::top()
{
	return calls.top();
}
