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
	// 为main函数创建栈帧，返回地址为-1
	callStack.push(functionTable.getAllInfo(0), -1);

	// 跳转到main函数
	code.setPC(functionTable.getAddr(0));

	try // 捕获所有组件的异常
	{

	// 按顺序执行指令
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
			if (index < 0 || index >= (int)globalVars.size()) // 下标合法性检查
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
			if (index < 0 || index >= (int)globalVars.size()) // 下标合法性检查
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
		case Opcode::jmp: // 无条件跳转
		{
			int addr = inst.getIntParam();
			code.setPC(addr);
			break;
		}
		case Opcode::jl: // 小于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val < Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jle: // 小于等于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val <= Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jg: // 大于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val > Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jge: // 大于等于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val >= Value(0))
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::je: // 等于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (!val)
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::jne: // 不等于0时跳转
		{
			int addr = inst.getIntParam();
			Value val = operandStack.pop();
			if (val)
			{
				code.setPC(addr);
			}
			break;
		}
		case Opcode::call: // 函数调用
		{
			int funIndex = inst.getIntParam();
			const FunctionTable::Entry& info = functionTable.getAllInfo(funIndex);
			int retAddr = code.getPC(); callStack.push(info, retAddr);
			code.setPC(info.addr);

			break;
		}
		case Opcode::ret: // 函数返回
		{
			int retAddr = callStack.pop();
			if (retAddr == -1) // main函数返回
			{
				return;
			}
			code.setPC(retAddr);
			break;
		}
		case Opcode::toi: // 将栈顶元素强制转换成整数
		{
			Value v = operandStack.pop();
			operandStack.push(v.toInt());
			break;
		}
		case Opcode::tod: // 将栈顶元素强制转换成浮点数
		{
			Value v = operandStack.pop();
			operandStack.push(v.toDouble());
			break;
		}
		case Opcode::pop: // 弹出栈顶操作数
		{
			operandStack.pop();
			break;
		}
		case Opcode::land: // 逻辑与
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a && b);
			break;
		}
		case Opcode::lor: // 逻辑或
		{
			Value b = operandStack.pop();
			Value a = operandStack.pop();
			operandStack.push(a || b);
			break;
		}
		case Opcode::lnot: // 逻辑非
		{
			Value v = operandStack.pop();
			operandStack.push(!v);
			break;
		}
		default: // 未识别的指令
			throw ByxVMError("Illegal instruction: " + OpcodeToString(op) + ".");
			break;
		}

		inst = code.next();
		op = inst.getOp();
	}

	} 
	// 捕获Value异常
	catch (Value::ReadUnknownDataType)
	{
		throw ByxVMError("Read unknown value type.");
	}
	// 捕获OperandStack异常
	catch (OperandStack::PopWhenStackEmpty)
	{
		throw ByxVMError("Operand stack is empty when pop.");
	}
	// 捕获CallStack异常
	catch (CallStack::PopWhenStackEmpty)
	{
		throw ByxVMError("Call stack is empty when pop.");
	}
	catch (CallStack::VarIndexOutOfBoundary)
	{
		throw ByxVMError("Illegal var index.");
	}
	// 捕获Code异常
	catch (Code::IllegalPCValue)
	{
		throw ByxVMError("Illegal PC value to set.");
	}
	// 捕获FunctionTable异常
	catch (FunctionTable::FunctionIndexOutOfBoundary)
	{
		throw ByxVMError("Illegal function index.");
	}
	// 捕获ByxVM异常
	catch (ByxVMError err)
	{
		throw err;
	}
	// 未知异常
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
