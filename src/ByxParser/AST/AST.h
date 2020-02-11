#pragma once

#include "../../ByxLexer/Token.h"

#include <vector>
#include <memory>

class ASTVisitor;

// 语法树节点基类
class ASTNode
{
public:
	virtual void visit(ASTVisitor& visitor) = 0; // 访问者模式
	virtual ~ASTNode();
	int row() const;
	int col() const;

	Token token; // 原始词法单元
};

// 语句基类
class Statement : public ASTNode
{

};

// 表达式基类
class Expression : public ASTNode
{

};

// 程序
class ProgramNode : public ASTNode
{
public:
	ProgramNode(const std::vector<std::shared_ptr<ASTNode>>& stmts);
	virtual void visit(ASTVisitor& visitor) override;

	std::vector<std::shared_ptr<ASTNode>> stmts;
};

// 空语句
class EmptyNode : public Statement
{
public:
	virtual void visit(ASTVisitor& visitor) override;
};

// 整数常量
class IntegerNode : public Expression
{
public:
	IntegerNode(int val, const Token& token = Token());
	virtual void visit(ASTVisitor& visitor) override;

	int val;
};

// 浮点数常量
class DoubleNode : public Expression
{
public:
	DoubleNode(double val, const Token& token = Token());
	virtual void visit(ASTVisitor& visitor) override;

	double val;
};

// 变量声明
class VarDeclareNode : public Statement
{
public:
	VarDeclareNode(const std::string& name, std::shared_ptr<Expression> expr, bool isExport, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::shared_ptr<Expression> expr;
	bool isExport;
	int index;
};

// 函数声明
class FunctionDeclareNode : public Statement
{
public:
	FunctionDeclareNode(const std::string& name, const std::vector<std::string>& paramName, const std::vector<std::shared_ptr<Statement>>& body, bool isExport, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::vector<std::string> paramName;
	std::vector<std::shared_ptr<Statement>> body;
	bool isExport;
};

// 变量
class VarNode : public Expression
{
public:
	VarNode(const std::string& name, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	int index;
};

// 赋值
class VarAssignNode : public Statement
{
public:
	VarAssignNode(const std::string& name, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::shared_ptr<Expression> expr;
	int index;
};

// 函数返回
class ReturnNode : public Statement
{
public:
	ReturnNode(bool hasExpr, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	bool hasExpr;
	std::shared_ptr<Expression> expr;
};

// 函数调用（语句）
class FunctionCallStmtNode : public Statement
{
public:
	FunctionCallStmtNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::vector<std::shared_ptr<Expression>> exprs;
};

//函数调用（表达式）
class FunctionCallExprNode : public Expression
{
public:
	FunctionCallExprNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::vector<std::shared_ptr<Expression>> exprs;
};

// if语句
class IfNode : public Statement
{
public:
	IfNode(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> tBranch, std::shared_ptr<Statement> fBranch, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> tBranch;
	std::shared_ptr<Statement> fBranch;
};

enum class BinaryOp { Add, Sub, Mul, Div, Rem, Equ, NotEqu, Larger, Less, LargerEqu, LessEqu, And, Or };

// 二元操作符
class BinaryOpNode : public Expression
{
public:
	BinaryOpNode(BinaryOp opType, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	BinaryOp opType;
	std::shared_ptr<Expression> lhs, rhs;
};

enum class UnaryOp { Pos, Neg, Not };

// 一元操作符
class UnaryOpNode : public Expression
{
public:
	UnaryOpNode(UnaryOp opType, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	UnaryOp opType;
	std::shared_ptr<Expression> expr;
};

// while语句
class WhileNode : public Statement
{
public:
	WhileNode(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> body, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> body;
};

// break语句
class BreakNode : public Statement
{
public:
	BreakNode(const Token& token);
	virtual void visit(ASTVisitor& visitor) override;
};

// continue语句
class ContinueNode : public Statement
{
public:
	ContinueNode(const Token& token);
	virtual void visit(ASTVisitor& visitor) override;
};

// for循环
class ForNode : public Statement
{
public:
	ForNode(std::shared_ptr<Statement> init, std::shared_ptr<Expression> cond, std::shared_ptr<Statement> update, std::shared_ptr<Statement> body, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::shared_ptr<Statement> init;
	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> update;
	std::shared_ptr<Statement> body;
};

// 代码块
class CodeBlockNode : public Statement
{
public:
	CodeBlockNode(const std::vector<std::shared_ptr<Statement>>& stmts);
	virtual void visit(ASTVisitor& visitor) override;

	std::vector<std::shared_ptr<Statement>> stmts;
};