#include "Instruction.h"

#include <sstream>

using namespace std;

Instruction::Instruction(Opcode op)
	: op(op), dVal(0.0), type(None)
{

}

Instruction::Instruction(Opcode op, int val)
	: op(op), iVal(val), type(Integer)
{

}

Instruction::Instruction(Opcode op, double val)
	: op(op), dVal(val), type(Double)
{

}

Opcode Instruction::getOp() const
{
	return op;
}

int Instruction::getIntParam() const
{
	return iVal;
}

double Instruction::getDoubleParam() const
{
	return dVal;
}

void Instruction::setIntParam(int p)
{
	iVal = p;
}

void Instruction::setDoubleParam(double p)
{
	dVal = p;
}

std::string Instruction::toString() const
{
	string s = GetOpcodeSymbol(op);
	if (type == Integer)
	{
		s += " ";
		s += to_string(iVal);
	}
	else if (type == Double)
	{
		s += " ";
		s += (stringstream() << dVal).str();
	}
	return s;
}