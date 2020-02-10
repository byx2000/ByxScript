#pragma once

#include "../Common/Instruction.h"

#include <vector>
#include <map>

class CodeSeg
{
	friend class Code;
public:
	int add(Opcode op);
	int add(Opcode op, int p);
	int add(Opcode op, double p);
	int add(const Instruction& inst);
	void add(const CodeSeg& seg);
	void setIntParam(int index, int p);
	int getSize() const;
	std::string toString() const;

private:
	std::vector<Instruction> insts;
};