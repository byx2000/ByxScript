#include "ToStringVisitor.h"

using namespace std;

std::string ToStringVisitor::getString() const
{
	return str;
}

void ToStringVisitor::visit(ProgramNode& node)
{
	str += "Program:\n";
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
		str += "\n";
	}
}

void ToStringVisitor::visit(IntegerNode& node)
{
	str += to_string(node.val);
}

void ToStringVisitor::visit(DoubleNode& node)
{
	str += to_string(node.val);
}

void ToStringVisitor::visit(GlobalVarDeclareNode& node)
{
	str += "GlobalVarDec(";
	str += node.name;
	str += ",";
	node.expr->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(LocalVarDeclareNode& node)
{
	str += "LocalVarDec(";
	str += node.name;
	str += ",";
	node.expr->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(FunctionDeclareNode& node)
{
	str += "FuncDec(";
	str += node.name;
	str += ",";
	for (int i = 0; i < (int)node.paramName.size(); ++i)
	{
		str += node.paramName[i];
		str += ",";
	}
	str += "body(";
	for (int i = 0; i < (int)node.body.size(); ++i)
	{
		node.body[i]->visit(*this);
		if (i != (int)node.body.size() - 1)
		{
			str += ",";
		}
	}
	str += "))";
}

void ToStringVisitor::visit(CodeBlockNode& node)
{
	str += "block(";
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
		if (i != (int)node.stmts.size() - 1)
		{
			str += ",";
		}
	}
	str += ")";
}

void ToStringVisitor::visit(VarNode& node)
{
	str += node.name;
}

void ToStringVisitor::visit(VarAssignNode& node)
{
	str += "=(";
	str += node.name;
	str += ",";
	node.expr->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(ReturnNode& node)
{
	str += "ret(";
	if (node.hasExpr)
	{
		node.expr->visit(*this);
	}
	str += ")";
}

void ToStringVisitor::visit(FunctionCallStmtNode& node)
{
	str += node.name;
	str += "(";
	for (int i = 0; i < (int)node.exprs.size(); ++i)
	{
		node.exprs[i]->visit(*this);
		if (i != (int)node.exprs.size() - 1)
		{
			str += ",";
		}
	}
	str += ")";
}

void ToStringVisitor::visit(FunctionCallExprNode& node)
{
	str += node.name;
	str += "(";
	for (int i = 0; i < (int)node.exprs.size(); ++i)
	{
		node.exprs[i]->visit(*this);
		if (i != (int)node.exprs.size() - 1)
		{
			str += ",";
		}
	}
	str += ")";
}

void ToStringVisitor::visit(IfNode& node)
{
	str += "if(";
	node.cond->visit(*this);
	str += ",";
	node.tBranch->visit(*this);
	str += ",";
	node.fBranch->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(BinaryOpNode& node)
{
	string op[] = { "+", "-", "*", "/", "%", "==", "!= ", ">", "<", ">=", "<=", "&&", "||" };

	str += op[(int)node.opType];
	str += "(";
	node.lhs->visit(*this);
	str += ",";
	node.rhs->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(WhileNode& node)
{
	str += "while(";
	node.cond->visit(*this);
	str += ",";
	node.body->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(BreakNode& node)
{
	str += "break";
}

void ToStringVisitor::visit(ContinueNode& node)
{
	str += "continue";
}

void ToStringVisitor::visit(UnaryOpNode& node)
{
	string op[] = { "+", "-", "!" };
	str += op[(int)node.opType];
	str += "(";
	node.expr->visit(*this);
	str += ")";
}

void ToStringVisitor::visit(ForNode& node)
{
	str += "for(";
	node.init->visit(*this);
	str += ",";
	node.cond->visit(*this);
	str += ",";
	node.update->visit(*this);
	str += ",";
	node.body->visit(*this);
	str += ")";
}
