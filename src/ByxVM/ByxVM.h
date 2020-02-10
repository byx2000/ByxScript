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
	void exec(); //ִ��
	OperandStack getOpStack() const; // ��ȡ������ջ
	std::vector<Value> getGlobalVars() const; // ��ȡȫ�ֱ���

	// �쳣
	class ByxVMError
	{
	public:
		std::string msg;
		ByxVMError(const std::string& msg);
	};

private:
	Code code; // ָ��
	FunctionTable functionTable; // ������
	CallStack callStack; // ����ջ
	OperandStack operandStack; // ������ջ
	std::vector<Value> globalVars; // ȫ�ֱ���
};