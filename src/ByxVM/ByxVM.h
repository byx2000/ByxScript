#pragma once

#include "../Common/Code.h"
#include "../Common/FunctionTable.h"
#include "CallStack.h"
#include "OperandStack.h"

#include <vector>

class ByxVM
{
public:
	ByxVM(const Code& code, const FunctionTable& functionTable, int globalSpace);
	void exec(); //执行
	OperandStack getOpStack() const; // 获取操作数栈
	std::vector<Value> getGlobalVars() const; // 获取全局变量

	// 异常
	class ByxVMError
	{
	public:
		std::string msg;
		ByxVMError(const std::string& msg);
	};

private:
	Code code; // 指令
	FunctionTable functionTable; // 函数表
	CallStack callStack; // 调用栈
	OperandStack operandStack; // 操作数栈
	std::vector<Value> globalVars; // 全局变量
};