#pragma once

#include "../Common/Value.h"

#include <stack>

class OperandStack
{
public:
	void push(const Value& val); // ѹ����ֵ
	Value pop(); // ������ֵ

	std::string toString() const;

	// �쳣
	class PopWhenStackEmpty {}; // popʱջΪ��

private:
	std::stack<Value> operands;
	void checkStack() const;
};