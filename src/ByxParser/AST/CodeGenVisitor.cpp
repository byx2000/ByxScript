#include "CodeGenVisitor.h"
#include "../ByxParser.h"

#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(ByxParser& parser)
	: parser(parser)
{
	inGlobal = true;
	inLoop = false;
}

CodeSeg CodeGenVisitor::getCode() const
{
	return codeSeg;
}

void CodeGenVisitor::visit(ProgramNode& node)
{
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		inGlobal = true;
		node.stmts[i]->visit(*this);
	}
}

void CodeGenVisitor::visit(VarDeclareNode& node)
{
	node.expr->visit(*this);

	if (inGlobal)
	{
		int index = parser.globalVarInfo[node.name].index;
		codeSeg.add(Opcode::gstore, index);
	}
	else
	{
		codeSeg.add(Opcode::store, node.index);
	}
}

void CodeGenVisitor::visit(FunctionDeclareNode& node)
{
	// 获取函数信息
	FunctionInfo info = parser.functionInfo[node.name];

	// 设置函数起始地址
	parser.functionInfo[node.name].addr = codeSeg.getSize();

	// 生成参数读取指令
	int cnt = 0;
	for (int i = 0; i < (int)node.paramName.size(); ++i)
	{
		codeSeg.add(Opcode::load);
		cnt++;
	}

	// 生成函数体代码
	inGlobal = false;
	for (int i = 0; i < (int)node.body.size(); ++i)
	{
		node.body[i]->visit(*this);
	}

	// 添加ret指令
	codeSeg.add(Opcode::ret);
}

void CodeGenVisitor::visit(IntegerNode& node)
{
	codeSeg.add(Opcode::iconst, node.val);
}

void CodeGenVisitor::visit(DoubleNode& node)
{
	codeSeg.add(Opcode::dconst, node.val);
}

void CodeGenVisitor::visit(VarNode& node)
{
	if (inGlobal)
	{
		// 全局变量未定义
		if (parser.globalVarInfo.count(node.name) == 0)
		{
			throw ByxParser::ParseError(string("Global var '") + node.name + "' is undefined.", node.row(), node.col());
		}
		codeSeg.add(Opcode::gload, parser.globalVarInfo[node.name].index);
	}
	else
	{
		if (node.index == -1)
		{
			relocTable.push_back(RelocEntry(codeSeg.getSize(), node.name));
		}
		else
		{
			codeSeg.add(Opcode::load, node.index);
		}
	}
}

void CodeGenVisitor::visit(BinaryOpNode& node)
{
	if (node.opType != BinaryOp::And && node.opType != BinaryOp::Or)
	{
		node.lhs->visit(*this);
		node.rhs->visit(*this);
	}

	switch (node.opType)
	{
	case BinaryOp::Add:
		codeSeg.add(Opcode::add);
		break;
	case BinaryOp::Sub:
		codeSeg.add(Opcode::sub);
		break;
	case BinaryOp::Mul:
		codeSeg.add(Opcode::mul);
		break;
	case BinaryOp::Div:
		codeSeg.add(Opcode::div);
		break;
	case BinaryOp::Rem:
		codeSeg.add(Opcode::rem);
		break;
	case BinaryOp::Equ:
		codeSeg.add(Opcode::equ);
		break;
	case BinaryOp::NotEqu:
		codeSeg.add(Opcode::neq);
		break;
	case BinaryOp::Larger:
		codeSeg.add(Opcode::g);
		break;
	case BinaryOp::Less:
		codeSeg.add(Opcode::l);
		break;
	case BinaryOp::LargerEqu:
		codeSeg.add(Opcode::ge);
		break;
	case BinaryOp::LessEqu:
		codeSeg.add(Opcode::le);
		break;
	case BinaryOp::And:
	{
		node.lhs->visit(*this);
		int index1 = codeSeg.add(Opcode::je, 0);
		codeSeg.add(Opcode::iconst, 1);
		node.rhs->visit(*this);
		codeSeg.add(Opcode::land);
		int index2 = codeSeg.add(Opcode::jmp, 0);
		codeSeg.setIntParam(index1, codeSeg.getSize());
		codeSeg.add(Opcode::iconst, 0);
		codeSeg.setIntParam(index2, codeSeg.getSize());
		break;
	}
	case BinaryOp::Or:
	{
		node.lhs->visit(*this);
		int index1 = codeSeg.add(Opcode::jne, 0);
		codeSeg.add(Opcode::iconst, 0);
		node.rhs->visit(*this);
		codeSeg.add(Opcode::lor);
		int index2 = codeSeg.add(Opcode::jmp, 0);
		codeSeg.setIntParam(index1, codeSeg.getSize());
		codeSeg.add(Opcode::iconst, 1);
		codeSeg.setIntParam(index2, codeSeg.getSize());
		break;
	}
	default:
		break;
	}
}

void CodeGenVisitor::visit(UnaryOpNode& node)
{
	node.expr->visit(*this);

	switch (node.opType)
	{
	case UnaryOp::Pos:
		break;
	case UnaryOp::Neg:
		codeSeg.add(Opcode::neg);
		break;
	case UnaryOp::Not:
		codeSeg.add(Opcode::lnot);
		break;
	default:
		break;
	}
}
