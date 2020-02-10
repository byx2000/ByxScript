#pragma once

#include "Instruction.h"
#include "CodeSeg.h"

#include <vector>

class Code
{
public:
	Code(const CodeSeg& seg = CodeSeg());
	Instruction& next(); // 获取下一条指令
	int getPC() const; // 获取程序计数器
	void setPC(int addr); // 设置程序计数器

	std::string toString() const;

	// 异常
	class IllegalPCValue {};

private:
	std::vector<Instruction> insts;
	int PC;
};