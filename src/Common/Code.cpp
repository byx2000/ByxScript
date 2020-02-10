#include "Code.h"

#include <sstream>

using namespace std;

Code::Code(const CodeSeg& seg)
{
	insts = seg.insts;
	PC = 0;
}

Instruction& Code::next()
{
	static Instruction last(Opcode::halt);

	// 到达指令流末尾，返回halt指令
	if (PC == insts.size())
	{
		return last;
	}

	return insts[PC++];
}

int Code::getPC() const
{
	return PC;
}

void Code::setPC(int addr)
{
	if (addr < 0 || addr >= (int)insts.size())
	{
		throw IllegalPCValue();
	}
	PC = addr;
}

std::string Code::toString() const
{
	string s = "code:\n";
	for (int i = 0; i < (int)insts.size(); ++i)
	{
		s += to_string(i);
		s += "\t";
		s += insts[i].toString();
		s += "\n";
	}
	return s;
}
