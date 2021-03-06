#include "LocalSymbolVisitor.h"
#include "../ByxParser.h"

#include <iostream>

using namespace std;

LocalSymbolVisitor::LocalSymbolVisitor(ByxParser& parser)
	: parser(parser)
{
	
}

void LocalSymbolVisitor::visit(ProgramNode& node)
{
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
	}
}

void LocalSymbolVisitor::visit(FunctionDeclareNode& node)
{
	// 初始化作用域栈
	scopeStack = ScopeStack();
	scopeStack.push();

	// 将函数参数加入作用域栈
	for (int i = 0; i < (int)node.paramName.size(); ++i)
	{
		int index = scopeStack.define(node.paramName[i]);
		cout << "define parameter: " << node.paramName[i] << " " << index << endl;
	}

	// 处理函数体
	for (int i = 0; i < (int)node.body.size(); ++i)
	{
		node.body[i]->visit(*this);
	}

	// 设置函数局部变量空间
	parser.functionInfo[node.name].space = scopeStack.getSymbolCount();
}

void LocalSymbolVisitor::visit(GlobalVarDeclareNode& node)
{
	node.expr->visit(*this);
	node.index = parser.globalVarInfo[node.name].index;
}

void LocalSymbolVisitor::visit(LocalVarDeclareNode& node)
{
	try
	{
		node.index = scopeStack.define(node.name);
	}
	catch (...)
	{
		throw ByxParser::ParseError(string("Redefined local var: ") + node.name + ".", node.row(), node.col());
	}

	cout << "define local var: " << node.name << " " << node.index << endl;

	node.expr->visit(*this);
}

void LocalSymbolVisitor::visit(CodeBlockNode& node)
{
	scopeStack.push();
	for (int i = 0; i < (int)node.stmts.size(); ++i)
	{
		node.stmts[i]->visit(*this);
	}
	scopeStack.pop();
}

void LocalSymbolVisitor::visit(VarNode& node)
{
	try
	{
		// 查找局部作用域
		node.index = scopeStack.resolve(node.name);
		node.isGlobal = false;
		cout << "resolve local var: " << node.name << " " << node.index << endl;
	}
	catch (...)
	{
		// 查找本模块全局变量
		if (parser.globalVarInfo.count(node.name) > 0)
		{
			node.index = parser.globalVarInfo[node.name].index;
			node.isGlobal = true;
			cout << "resolve global var: " << node.name << " " << node.index << endl;
		}
		// 未找到，生成重定位条目
		else
		{
			node.index = -1;
			cout << "relocate " << node.name << endl;
		}
	}
}

void LocalSymbolVisitor::visit(VarAssignNode& node)
{
	try
	{
		// 查找局部作用域
		node.index = scopeStack.resolve(node.name);
		node.isGlobal = false;
		cout << "resolve local var: " << node.name << " " << node.index << endl;
	}
	catch (...)
	{
		// 查找本模块全局变量
		if (parser.globalVarInfo.count(node.name) > 0)
		{
			node.index = parser.globalVarInfo[node.name].index;
			node.isGlobal = true;
			cout << "resolve global var: " << node.name << " " << node.index << endl;
		}
		// 未找到，生成重定位条目
		else
		{
			node.index = -1;
			cout << "relocate " << node.name << endl;
		}
	}

	node.expr->visit(*this);
}

void LocalSymbolVisitor::visit(ReturnNode& node)
{
	if (node.hasExpr)
	{
		node.expr->visit(*this);
	}
}

void LocalSymbolVisitor::visit(FunctionCallStmtNode& node)
{
	for (int i = 0; i < (int)node.exprs.size(); ++i)
	{
		node.exprs[i]->visit(*this);
	}
}

void LocalSymbolVisitor::visit(FunctionCallExprNode& node)
{
	for (int i = 0; i < (int)node.exprs.size(); ++i)
	{
		node.exprs[i]->visit(*this);
	}
}

void LocalSymbolVisitor::visit(IfNode& node)
{
	node.cond->visit(*this);

	scopeStack.push();
	node.tBranch->visit(*this);
	scopeStack.pop();

	scopeStack.push();
	node.fBranch->visit(*this);
	scopeStack.pop();
}

void LocalSymbolVisitor::visit(BinaryOpNode& node)
{
	node.lhs->visit(*this);
	node.rhs->visit(*this);
}

void LocalSymbolVisitor::visit(WhileNode& node)
{
	node.cond->visit(*this);
	scopeStack.push();
	node.body->visit(*this);
	scopeStack.pop();
}

void LocalSymbolVisitor::visit(UnaryOpNode& node)
{
	node.expr->visit(*this);
}

void LocalSymbolVisitor::visit(ForNode& node)
{
	scopeStack.push();
	node.init->visit(*this);
	node.cond->visit(*this);
	node.update->visit(*this);
	node.body->visit(*this);
	scopeStack.pop();
}
