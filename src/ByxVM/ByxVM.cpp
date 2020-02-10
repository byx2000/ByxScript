#include "ByxVM.h"

#include <iostream>

using namespace std;

ByxVM::ByxVM(const Code& code, const FunctionTable& functionTable, int globalSpace)
	: code(code), functionTable(functionTable)
{
	globalVars.resize(globalSpace);
}

void ByxVM::exec()
{
	// Ϊmain��������ջ֡�����ص�ַΪ-1
	callStack.push(functionTable.getAllInfo(0), -1);

	// ��ת��main����
	code.setPC(functionTable.getAddr(0));

	try // ��������������쳣
	{

	// ��˳��ִ��ָ��
	int cntFunction = functionTable.getCount();
	Instruction& inst = code.next();
	Opcode op = inst.getOp();
	while (op != Opcode::halt)
	{
		switch (op)
		{
		case Opcode::nop:
		{
			break;
		}	
		case Opcode::halt:
		{
			return;
		}
		case Opcode::iconst:
		{
			int val = inst.getIntParam();
			operandStack.push(Value(val));
			break;
		}
		case Opcode::dconst:
		{
			double val = inst.getDoubleParam();
			operandStack.push(Value(val));
			break;
		}
		case Opcode::load:
		{
			int index = inst.getIntParam();
			Value v = callStack.top().getVar(index);
			operandStack.push(v);
			break;
		}
		case Opcode::gload:
		{
			int index = inst.getIntParam();
			if (index < 0 || index >= (int)globalVars.size()) // �±�Ϸ��Լ��
			{
				throw ByxVMError("Global var index out of range.");
			}
			Value v = globalVars[index];
			operandStack.push(v);
			break;
		}
		case Opcode::store:
		{
			int index = inst.getIntParam();
			Value v = operandStack.pop();
			callStack.top().setVar(index, v);
			break;
		}
		case Opcode::gstore:
		{
			int index = inst.getIntParam();
			if (index < 0 || index >= (int)globalVars.size()) // �±�Ϸ��Լ��
			{
				throw ByxVMError("Global var index out of range.");
			}
			Value v = operandStack.pop();
			globalVars[index] = v;
			break;
		}
		case Opcode::add:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a + b);
			break;
		}
		case Opcode::sub:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a - b);
			break;
		}
		case Opcode::mul:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a * b);
			break;
		}
		case Opcode::div:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a / b);
			break;
		}
		case Opcode::rem:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a % b);
			break;
		}
		case Opcode::equ:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a == b);
			break;
		}
		case Opcode::neq:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a != b);
			break;
		}
		case Opcode::g:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a > b);
			break;
		}
		case Opcode::l:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a < b);
			break;
		}
		case Opcode::ge:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a >= b);
			break;
		}
		case Opcode::le:
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a <= b);
			break;
		}
		case Opcode::neg:
		{
			Value v = operandStack.pop();
			operandStack.push(-v);
			break;
		}
		case Opcode::jmp: // ��������ת
		{
			int addr = inst.getIntParam();
			code.setPC(addr);
			break;
		}
		case Opcode::jl: // С��0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val < Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jle: // С�ڵ���0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val <= Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jg: // ����0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val > Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jge: // ���ڵ���0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val >= Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::je: // ����0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (!val)
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jne: // ������0ʱ��ת
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val)
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::call: // ��������
		{
			int funIndex = inst.getIntParam();
			const FunctionTable::Entry& info = functionTable.getAllInfo(funIndex);
			int retAddr = code.getPC(); callStack.push(info, retAddr);
			code.setPC(info.addr);

			break;
		}
		case Opcode::ret: // ��������
		{
			int retAddr = callStack.pop();
			if (retAddr == -1) // main��������
			{
				return;
			}
			code.setPC(retAddr);
			break;
		}
		case Opcode::toi: // ��ջ��Ԫ��ǿ��ת��������
		{
			Value v = operandStack.pop();
			operandStack.push(v.toInt());
			break;
		}
		case Opcode::tod: // ��ջ��Ԫ��ǿ��ת���ɸ�����
		{
			Value v = operandStack.pop();
			operandStack.push(v.toDouble());
			break;
		}
		case Opcode::pop: // ����ջ��������
		{
			operandStack.pop();
			break;
		}
		case Opcode::land: // �߼���
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a && b);
			break;
		}
		case Opcode::lor: // �߼���
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a || b);
			break;
		}
		case Opcode::lnot: // �߼���
		{
			Value v = operandStack.pop();
			operandStack.push(!v);
			break;
		}
		default: // δʶ���ָ��
			throw ByxVMError("Illegal instruction: " + OpcodeToString(op) + ".");
			break;
		}

		inst = code.next();
		op = inst.getOp();
	}

	} 
	// ����Value�쳣
	catch (Value::ReadUnknownDataType)
	{
		throw ByxVMError("Read unknown value type.");
	}
	// ����OperandStack�쳣
	catch (OperandStack::PopWhenStackEmpty)
	{
		throw ByxVMError("Operand stack is empty when pop.");
	}
	// ����CallStack�쳣
	catch (CallStack::PopWhenStackEmpty)
	{
		throw ByxVMError("Call stack is empty when pop.");
	}
	catch (CallStack::VarIndexOutOfBoundary)
	{
		throw ByxVMError("Illegal var index.");
	}
	// ����Code�쳣
	catch (Code::IllegalPCValue)
	{
		throw ByxVMError("Illegal PC value to set.");
	}
	// ����FunctionTable�쳣
	catch (FunctionTable::FunctionIndexOutOfBoundary)
	{
		throw ByxVMError("Illegal function index.");
	}
	// ����ByxVM�쳣
	catch (ByxVMError err)
	{
		throw err;
	}
	// δ֪�쳣
	catch (...)
	{
		throw ByxVMError("Unknown error.");
	}
}

OperandStack ByxVM::getOpStack() const
{
	return operandStack;
}

std::vector<Value> ByxVM::getGlobalVars() const
{
	return globalVars;
}

ByxVM::ByxVMError::ByxVMError(const std::string& msg)
	: msg(msg)
{

}
