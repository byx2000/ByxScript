#include "AST.h"
#include "ASTVisitor.h"

using namespace std;

ASTNode::~ASTNode()
{

}

int ASTNode::row() const
{
	return token.row;
}

int ASTNode::col() const
{
	return token.col;
}

ProgramNode::ProgramNode(const std::vector<shared_ptr<ASTNode>>& stmts)
	: stmts(stmts)
{

}

void ProgramNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

void EmptyNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

IntegerNode::IntegerNode(int val, const Token& token)
	: val(val)
{
	this->token = token;
}

void IntegerNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

DoubleNode::DoubleNode(double val, const Token& token)
	: val(val)
{
	this->token = token;
}

void DoubleNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

VarDeclareNode::VarDeclareNode(const std::string& name, std::shared_ptr<Expression> expr, bool isExport, const Token& token)
	: name(name), expr(expr), isExport(isExport)
{
	this->token = token;
	index = -1;
}

void VarDeclareNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

FunctionDeclareNode::FunctionDeclareNode(const std::string& name, const std::vector<std::string>& paramName, const std::vector<std::shared_ptr<Statement>>& body, bool isExport, const Token& token)
	: name(name), paramName(paramName), body(body), isExport(isExport)
{
	this->token = token;
}

void FunctionDeclareNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

VarNode::VarNode(const std::string& name, const Token& token)
	: name(name)
{
	this->token = token;
	index = -1;
}

void VarNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

VarAssignNode::VarAssignNode(const std::string& name, std::shared_ptr<Expression> expr, const Token& token)
	: name(name), expr(expr)
{
	this->token = token;
	index = -1;
}

void VarAssignNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

ReturnNode::ReturnNode(bool hasExpr, std::shared_ptr<Expression> expr, const Token& token)
	: hasExpr(hasExpr), expr(expr)
{
	this->token = token;
}

void ReturnNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

FunctionCallStmtNode::FunctionCallStmtNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token)
	: name(name), exprs(exprs)
{
	this->token = token;
}

void FunctionCallStmtNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

FunctionCallExprNode::FunctionCallExprNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token)
	: name(name), exprs(exprs)
{
	this->token = token;
}

void FunctionCallExprNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

IfNode::IfNode(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> tBranch, std::shared_ptr<Statement> fBranch, const Token& token)
	: cond(cond), tBranch(tBranch), fBranch(fBranch)
{
	this->token = token;
}

void IfNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

BinaryOpNode::BinaryOpNode(BinaryOp opType, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs, const Token& token)
	: opType(opType), lhs(lhs), rhs(rhs)
{
	this->token = token;
}

void BinaryOpNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

UnaryOpNode::UnaryOpNode(UnaryOp opType, std::shared_ptr<Expression> expr, const Token& token)
	: opType(opType), expr(expr)
{
	this->token = token;
}

void UnaryOpNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

WhileNode::WhileNode(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> body, const Token& token)
	: cond(cond), body(body)
{
	this->token = token;
}

void WhileNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

BreakNode::BreakNode(const Token& token)
{
	this->token = token;
}

void BreakNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

ContinueNode::ContinueNode(const Token& token)
{
	this->token = token;
}

void ContinueNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

ForNode::ForNode(std::shared_ptr<Statement> init, std::shared_ptr<Expression> cond, std::shared_ptr<Statement> update, std::shared_ptr<Statement> body, const Token& token)
	: init(init), cond(cond), update(update), body(body)
{
	this->token = token;
}

void ForNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}

CodeBlockNode::CodeBlockNode(const std::vector<shared_ptr<Statement>>& stmts)
	: stmts(stmts)
{

}

void CodeBlockNode::visit(ASTVisitor& visitor)
{
	visitor.visit(*this);
}