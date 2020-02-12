#include "CodeGenVisitor.h"
#include "../ByxParser.h"

#include <iostream>

using namespace std;

CodeGenVisitor::CodeGenVisitor(ByxParser& parser, bool globalCode)
	: parser(parser), globalCode(globalCode)
{
	inGlobal = true;
	inLoop = false;
}

CodeSeg CodeGenVisitor::getCode() const
{
	return codeSeg;
}

std::vector<RelocEntry> CodeGenVisitor::getRelocTable() const
{
	return relocTable;
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
	if (globalCode)
	{
		node.expr->visit(*this);
		int index = parser.globalVarInfo[node.name].index;
		codeSeg.add(Opcode::gstore, index);
	}
	else
	{
		if (inGlobal)
		{
			return;
		}
		else
		{
			node.expr->visit(*this);
			codeSeg.add(Opcode::store, node.index);
		}
	}
}

void CodeGenVisitor::visit(FunctionDeclareNode& node)
{
	// ��ȡ������Ϣ
	FunctionInfo info = parser.functionInfo[node.name];

	// ���ú�����ʼ��ַ
	parser.functionInfo[node.name].addr = codeSeg.getSize();

	// ���ɲ�����ȡָ��
	int cnt = 0;
	for (int i = 0; i < (int)node.paramName.size(); ++i)
	{
		codeSeg.add(Opcode::load, cnt);
		cnt++;
	}

	// ���ɺ��������
	inGlobal = false;
	for (int i = 0; i < (int)node.body.size(); ++i)
	{
		node.body[i]->visit(*this);
	}

	// ���retָ��
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
		// ȫ�ֱ���δ����
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
			if (parser.globalVarInfo.count(node.name) == 0)
			{
				relocTable.push_back(RelocEntry(codeSeg.getSize(), node.name));
				codeSeg.add(Opcode::gload, 0);
			}
			else
			{
				codeSeg.add(Opcode::gload, parser.globalVarInfo[node.name].index);
			}
		}
		else
		{
			codeSeg.add(Opcode::load, node.index);
		}
	}
}

void CodeGenVisitor::visit(VarAssignNode& node)
{
	// ���ɸ�ֵ���ʽ����
	node.expr->visit(*this);

	if (node.index == -1)
	{
		relocTable.push_back(RelocEntry(codeSeg.getSize(), node.name));
		codeSeg.add(Opcode::store, 0);
	}
	else
	{
		codeSeg.add(Opcode::store, node.index);
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

void CodeGenVisitor::visit(ReturnNode& node)
{
	if (node.hasExpr)
	{
		node.expr->visit(*this);
	}
	codeSeg.add(Opcode::ret);
}

void CodeGenVisitor::visit(FunctionCallStmtNode& node)
{
	// ���ɼ�������Ĵ��루����
	for (int i = (int)node.exprs.size() - 1; i >= 0; --i)
	{
		node.exprs[i]->visit(*this);
	}

	// �����ض�λ��Ŀ
	relocTable.push_back(RelocEntry(codeSeg.getSize(), node.name, node.exprs.size()));
	codeSeg.add(Opcode::call, 0);
}

void CodeGenVisitor::visit(FunctionCallExprNode& node)
{
	// ���ɼ�������Ĵ��루����
	for (int i = (int)node.exprs.size() - 1; i >= 0; --i)
	{
		node.exprs[i]->visit(*this);
	}

	// �����ض�λ��Ŀ
	relocTable.push_back(RelocEntry(codeSeg.getSize(), node.name, node.exprs.size()));
	codeSeg.add(Opcode::call, 0);
}

void CodeGenVisitor::visit(IfNode& node)
{
	// �����������ʽ����
	node.cond->visit(*this);

	// ������Ϊ�٣�����ת��false��֧����תĿ�������
	int index1 = codeSeg.add(Opcode::je, 0);

	// ����true��֧����
	node.tBranch->visit(*this);

	// ִ����true��֧����ת��if����������תĿ�������
	int index2 = codeSeg.add(Opcode::jmp, 0);

	// ������תĿ��1
	codeSeg.setIntParam(index1, codeSeg.getSize());

	//����false��֧����
	node.fBranch->visit(*this);

	// ������תĿ��2
	codeSeg.setIntParam(index2, codeSeg.getSize());
}

void CodeGenVisitor::visit(WhileNode& node)
{
	vector<int> oldBreakStmtIndex = breakStmtIndex;
	breakStmtIndex.clear();
	vector<int> oldContinueBreakStmtIndex = continueStmtIndex;
	continueStmtIndex.clear();
	bool oldInLoop = inLoop;

	inLoop = true;

	// ���������ж���ʼ��ַ
	int addr = codeSeg.getSize();

	// �����������ʽ����
	node.cond->visit(*this);

	// ������Ϊ�٣�����ת��ѭ��������Ŀ�������
	int index = codeSeg.add(Opcode::je, 0);

	// ����ѭ�������
	node.body->visit(*this);

	// ѭ����ִ�������ת�������ж�
	codeSeg.add(Opcode::jmp, addr);

	// ������תĿ��
	codeSeg.setIntParam(index, codeSeg.getSize());

	// ����break��䣬������break�����תĿ��
	for (int i = 0; i < (int)breakStmtIndex.size(); ++i)
	{
		codeSeg.setIntParam(breakStmtIndex[i], codeSeg.getSize());
	}
	breakStmtIndex = oldBreakStmtIndex;

	// ����continue��䣬������continue�����תĿ��
	for (int i = 0; i < (int)continueStmtIndex.size(); ++i)
	{
		codeSeg.setIntParam(continueStmtIndex[i], addr);
	}
	continueStmtIndex = oldContinueBreakStmtIndex;

	inLoop = oldInLoop;
}

void CodeGenVisitor::visit(ForNode& node)
{
	vector<int> oldBreakStmtIndex = breakStmtIndex;
	breakStmtIndex.clear();
	vector<int> oldContinueBreakStmtIndex = continueStmtIndex;
	continueStmtIndex.clear();
	bool oldInLoop = inLoop;
	inLoop = true;

	// ���ɳ�ʼ������
	node.init->visit(*this);

	// ��¼�����ж���ʼ��ַ
	int addrCond = codeSeg.getSize();

	// ���������жϴ���
	node.cond->visit(*this);

	// ������Ϊ�٣�����ת��ѭ������
	int index = codeSeg.add(Opcode::je, 0);

	// ����ѭ�������
	node.body->visit(*this);

	// ��¼���´�����ʼ��ַ
	int addrUpdate = codeSeg.getSize();

	// ���ɸ��´���
	node.update->visit(*this);

	// ��ת�������ж�
	codeSeg.add(Opcode::jmp, addrCond);

	// ������ת��ַ
	codeSeg.setIntParam(index, codeSeg.getSize());

	// ����break��䣬������break�����תĿ��
	for (int i = 0; i < (int)breakStmtIndex.size(); ++i)
	{
		codeSeg.setIntParam(breakStmtIndex[i], codeSeg.getSize());
	}
	breakStmtIndex = oldBreakStmtIndex;

	// ����continue��䣬������continue�����תĿ��
	for (int i = 0; i < (int)continueStmtIndex.size(); ++i)
	{
		codeSeg.setIntParam(continueStmtIndex[i], addrUpdate);
	}
	continueStmtIndex = oldContinueBreakStmtIndex;

	inLoop = oldInLoop;
}

void CodeGenVisitor::visit(BreakNode& node)
{
	// ����ѭ����
	if (!inLoop)
	{
		throw ByxParser::ParseError("Break statement must be in a loop.", node.row(), node.col());
	}

	int index = codeSeg.add(Opcode::jmp, 0);
	breakStmtIndex.push_back(index);
}

void CodeGenVisitor::visit(ContinueNode& node)
{
	// ����ѭ����
	if (!inLoop)
	{
		throw ByxParser::ParseError("Break statement must be in a loop.", node.row(), node.col());
	}

	int index = codeSeg.add(Opcode::jmp, 0);
	continueStmtIndex.push_back(index);
}

void CodeGenVisitor::visit(CodeBlockNode& node)
{
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
	}
}
