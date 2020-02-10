#include "CodeSeg.h"

using namespace std;

int CodeSeg::add(Opcode op)
{
	insts.push_back(Instruction(op));
	return insts.size() - 1;
}

int CodeSeg::add(Opcode op, int p)
{
	insts.push_back(Instruction(op, p));
	return insts.size() - 1;
}

int CodeSeg::add(Opcode op, double p)
{
	insts.push_back(Instruction(op, p));
	return insts.size() - 1;
}

int CodeSeg::add(const Instruction& inst)
{
	insts.push_back(inst);
	return insts.size() - 1;
}

void CodeSeg::add(const CodeSeg& seg)
{
	int base = insts.size();
	for (int i = 0; i < (int)seg.insts.size(); ++i)
	{
		insts.push_back(seg.insts[i]);
		Opcode op = seg.insts[i].getOp();
		if (op == Opcode::jmp || op == Opcode::jl || op == Opcode::jle ||
			op == Opcode::jg || op == Opcode::jge || op == Opcode::je ||
			op == Opcode::jne)
		{
			insts[insts.size() - 1].setIntParam(seg.insts[i].getIntParam() + base);
		}
	}
}

void CodeSeg::setIntParam(int index, int p)
{
	insts[index].setIntParam(p);
}

int CodeSeg::getSize() const
{
	return insts.size();
}

std::string CodeSeg::toString() const
{
	string s;
	for (int i = 0; i < (int)insts.size(); ++i)
	{
		s += to_string(i);
		s += "\t";
		s += insts[i].toString();
		s += "\n";
	}
	return s;
}
