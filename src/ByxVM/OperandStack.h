#pragma once

#include "../Common/Value.h"

#include <stack>

class OperandStack
{
public:
	void push(const Value& val); // 压入数值
	Value pop(); // 弹出数值

	std::string toString() const;

	// 异常
	class PopWhenStackEmpty {}; // pop时栈为空

private:
	std::stack<Value> operands;
	void checkStack() const;
};