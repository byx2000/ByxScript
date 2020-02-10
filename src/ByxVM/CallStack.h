#pragma once

#include "../Common/FunctionTable.h"
#include "../Common/Value.h"

#include <stack>

class CallStack
{
public:
	// ����ջ��ջ֡
	class Frame
	{
	public:
		Frame(const FunctionTable::Entry& info, int retAddr);
		Value getVar(int index) const; // ��ȡ����
		void setVar(int index, const Value& value); // ���ñ���
		int getReturnAddr() const; // ��ȡ���ص�ַ
		int getVarSpace() const; // ��ȡ�����ռ�

		std::string toString() const;

	private:
		const FunctionTable::Entry& info; // ������Ϣ
		std::vector<Value> varTable; // ������
		int retAddr; // ���ص�ַ

		void checkIndex(int index) const; // �±���
	};

	void push(const FunctionTable::Entry& info, int retAddr); // ѹ��ջ֡
	int pop(); // ����ջ֡�����ط��ص�ַ
	Frame& top(); // ��ȡ����ջջ��

	// �쳣��
	class PopWhenStackEmpty {}; // popʱ����ջΪ��
	class VarIndexOutOfBoundary {}; // ��������Խ��

private:
	std::stack<Frame> calls; // ����ջ
};