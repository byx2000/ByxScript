#pragma once

#include "Opcode.h"

class Instruction
{
public:
	Instruction(Opcode op); // 无参数指令
	Instruction(Opcode op, int val); // 整数参数指令
	Instruction(Opcode op, double val); // 浮点数参数指令
	Opcode getOp() const; // 获取操作码
	int getIntParam() const; // 获取整数参数
	double getDoubleParam() const; // 获取浮点数参数
	void setIntParam(int p); // 设置整数参数
	void setDoubleParam(double p); // 设置浮点数参数

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