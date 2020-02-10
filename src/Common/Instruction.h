#pragma once

#include "Opcode.h"

class Instruction
{
public:
	Instruction(Opcode op); // �޲���ָ��
	Instruction(Opcode op, int val); // ��������ָ��
	Instruction(Opcode op, double val); // ����������ָ��
	Opcode getOp() const; // ��ȡ������
	int getIntParam() const; // ��ȡ��������
	double getDoubleParam() const; // ��ȡ����������
	void setIntParam(int p); // ������������
	void setDoubleParam(double p); // ���ø���������

	std::string toString() const;

private:
	Opcode op;
	union
	{
		int iVal;
		double dVal;
	};

	enum
	{
		Integer,
		Double,
		None
	} type;
};