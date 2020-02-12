#pragma once

#include "../../ByxLexer/Token.h"

#include <vector>
#include <memory>

class ASTVisitor;

// �﷨���ڵ����
class ASTNode
{
public:
	virtual void visit(ASTVisitor& visitor) = 0; // ������ģʽ
	virtual ~ASTNode();
	int row() const;
	int col() const;

	Token token; // ԭʼ�ʷ���Ԫ
};

// ������
class Statement : public ASTNode
{

};

// ���ʽ����
class Expression : public ASTNode
{

};

// ����
class ProgramNode : public ASTNode
{
public:
	ProgramNode(const std::vector<std::shared_ptr<ASTNode>>& stmts);
	virtual void visit(ASTVisitor& visitor) override;

	std::vector<std::shared_ptr<ASTNode>> stmts;
};

// �����
class EmptyNode : public Statement
{
public:
	virtual void visit(ASTVisitor& visitor) override;
};

// ��������
class IntegerNode : public Expression
{
public:
	IntegerNode(int val, const Token& token = Token());
	virtual void visit(ASTVisitor& visitor) override;

	int val;
};

// ����������
class DoubleNode : public Expression
{
public:
	DoubleNode(double val, const Token& token = Token());
	virtual void visit(ASTVisitor& visitor) override;

	double val;
};

// ȫ�ֱ�������
class GlobalVarDeclareNode : public Statement
{
public:
	GlobalVarDeclareNode(const std::string& name, std::shared_ptr<Expression> expr, bool isExport, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name; // ������
	std::shared_ptr<Expression> expr; // ��ʼ�����ʽ
	bool isExport; // �Ƿ�Ϊ��������
	int index; // ��������
};

// �ֲ���������
class LocalVarDeclareNode : public Statement
{
public:
	LocalVarDeclareNode(const std::string& name, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name; // ������
	std::shared_ptr<Expression> expr; // ��ʼ�����ʽ
	int index; // ��������
};

// ��������
class FunctionDeclareNode : public Statement
{
public:
	FunctionDeclareNode(const std::string& name, const std::vector<std::string>& paramName, const std::vector<std::shared_ptr<Statement>>& body, bool isExport, bool hasRetVal, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name; // ������
	std::vector<std::string> paramName; // ������
	std::vector<std::shared_ptr<Statement>> body; // ������
	bool isExport; // �Ƿ�Ϊ��������
	bool hasRetVal; // �Ƿ��з���ֵ
};

// ����
class VarNode : public Expression
{
public:
	VarNode(const std::string& name, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name; // ������
	int index; // ��������
	bool isGlobal; // �Ƿ�Ϊȫ�ֱ���
};

// ��ֵ
class VarAssignNode : public Statement
{
public:
	VarAssignNode(const std::string& name, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name; // ������
	std::shared_ptr<Expression> expr; // ��ֵ���ʽ
	int index; // ��������
	bool isGlobal; // �Ƿ�Ϊȫ�ֱ���
};

// ��������
class ReturnNode : public Statement
{
public:
	ReturnNode(bool hasExpr, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	bool hasExpr;
	std::shared_ptr<Expression> expr;
};

// �������ã���䣩
class FunctionCallStmtNode : public Statement
{
public:
	FunctionCallStmtNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::vector<std::shared_ptr<Expression>> exprs;
};

//�������ã����ʽ��
class FunctionCallExprNode : public Expression
{
public:
	FunctionCallExprNode(const std::string& name, const std::vector<std::shared_ptr<Expression>>& exprs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::string name;
	std::vector<std::shared_ptr<Expression>> exprs;
};

// if���
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

// ��Ԫ������
class BinaryOpNode : public Expression
{
public:
	BinaryOpNode(BinaryOp opType, std::shared_ptr<Expression> lhs, std::shared_ptr<Expression> rhs, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	BinaryOp opType;
	std::shared_ptr<Expression> lhs, rhs;
};

enum class UnaryOp { Pos, Neg, Not };

// һԪ������
class UnaryOpNode : public Expression
{
public:
	UnaryOpNode(UnaryOp opType, std::shared_ptr<Expression> expr, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	UnaryOp opType;
	std::shared_ptr<Expression> expr;
};

// while���
class WhileNode : public Statement
{
public:
	WhileNode(std::shared_ptr<Expression> cond, std::shared_ptr<Statement> body, const Token& token);
	virtual void visit(ASTVisitor& visitor) override;

	std::shared_ptr<Expression> cond;
	std::shared_ptr<Statement> body;
};

// break���
class BreakNode : public Statement
{
public:
	BreakNode(const Token& token);
	virtual void visit(ASTVisitor& visitor) override;
};

// continue���
class ContinueNode : public Statement
{
public:
	ContinueNode(const Token& token);
	virtual void visit(ASTVisitor& visitor) override;
};

// forѭ��
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

// �����
class CodeBlockNode : public Statement
{
public:
	CodeBlockNode(const std::vector<std::shared_ptr<Statement>>& stmts);
	virtual void visit(ASTVisitor& visitor) override;

	std::vector<std::shared_ptr<Statement>> stmts;
};