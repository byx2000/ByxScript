#pragma once

#include "../ByxLexer/ByxLexer.h"
#include "AST/AST.h"
#include "../Common/Module.h"

#include <map>

class ByxParser
{
	friend class CodeGenVisitor;
	friend class LocalSymbolVisitor;
public:
	// �쳣
	struct ParseError
	{
		std::string msg;
		int row;
		int col;
		ParseError(const std::string& msg, int row, int col);
	};

	ByxParser(const std::string& input);
	ByxParser& parse();
	std::string getASTString();

private:
	std::string input; // ԭʼ����
	ByxLexer lexer; // �ʷ�������
	std::shared_ptr<ASTNode> ast; // �����﷨�����ڵ�
	
	std::map<std::string, GlobalVarInfo> globalVarInfo; // ȫ�ֱ�����Ϣ
	std::map<std::string, FunctionInfo> functionInfo; // ������Ϣ
	CodeSeg globalCode; // ȫ�ֱ�����ʼ������

	bool parsingForHeader;

	void printGlobalVarInfo();
	void printFunctionInfo();

	std::shared_ptr<ASTNode> parseProgram();
	std::shared_ptr<Statement> parseFunctionDeclare(bool isExport);
	std::shared_ptr<Statement> parseVarDeclare(bool isExport);
	std::shared_ptr<Statement> parseStatement();
	std::shared_ptr<Statement> parseVarAssign();
	std::shared_ptr<Statement> parseReturn();
	std::shared_ptr<Statement> parseCodeBlock();
	std::shared_ptr<Statement> parseFunctionCallStmt();
	std::shared_ptr<Statement> parseIf();
	std::shared_ptr<Statement> parseWhile();
	std::shared_ptr<Statement> parseFor();
	std::shared_ptr<Expression> parseExpr();
	std::shared_ptr<Expression> parseCmpExpr();
	std::shared_ptr<Expression> parseArithExpr();
	std::shared_ptr<Expression> parseTerm();
	std::shared_ptr<Expression> parseFactor();
	std::shared_ptr<Expression> parseFunctionCallExpr();

	void readSemicolon();
};