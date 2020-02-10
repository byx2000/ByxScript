#pragma once

#include "Instruction.h"
#include "CodeSeg.h"

#include <vector>

class Code
{
public:
	Code(const CodeSeg& seg = CodeSeg());
	Instruction& next(); // ��ȡ��һ��ָ��
	int getPC() const; // ��ȡ���������
	void setPC(int addr); // ���ó��������

	std::string toString() const;

	// �쳣
	class IllegalPCValue {};

private:
	std::vector<Instruction> insts;
	int PC;
};