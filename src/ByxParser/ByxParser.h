#pragma once

#include "../ByxLexer/ByxLexer.h"
#include "AST/AST.h"

#include <map>

class ByxParser
{
public:
	// 异常
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
	std::string input; // 原始输入
	ByxLexer lexer; // 词法分析器
	std::shared_ptr<ASTNode> ast; // 抽象语法树根节点
	std::map<std::string, int> globalVarIndex; // 全局变量索引
	std::map<std::string, int> funcIndex; // 函数索引
	std::map<std::string, int> funcParamCnt; // 函数参数数量

	bool parsingForHeader;

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