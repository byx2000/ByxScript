#pragma once

#include "../Common/FunctionTable.h"
#include "../Common/Value.h"

#include <stack>

class CallStack
{
public:
	// 调用栈的栈帧
	class Frame
	{
	public:
		Frame(const FunctionTable::Entry& info, int retAddr);
		Value getVar(int index) const; // 获取变量
		void setVar(int index, const Value& value); // 设置变量
		int getReturnAddr() const; // 获取返回地址
		int getVarSpace() const; // 获取变量空间

		std::string toString() const;

	private:
		const FunctionTable::Entry& info; // 函数信息
		std::vector<Value> varTable; // 变量表
		int retAddr; // 返回地址

		void checkIndex(int index) const; // 下标检查
	};

	void push(const FunctionTable::Entry& info, int retAddr); // 压入栈帧
	int pop(); // 弹出栈帧，返回返回地址
	Frame& top(); // 获取调用栈栈顶

	// 异常类
	class PopWhenStackEmpty {}; // pop时调用栈为空
	class VarIndexOutOfBoundary {}; // 变量索引越界

private:
	std::stack<Frame> calls; // 调用栈
};