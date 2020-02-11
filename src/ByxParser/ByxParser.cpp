#include "ByxParser.h"
#include "AST/ToStringVisitor.h"
#include "AST/GlobalSymbolVisitor.h"

#include <sstream>
#include <iostream>

using namespace std;

ByxParser::ByxParser(const std::string& input)
	: input(input), lexer(input)
{
	parsingForHeader = false;
}

ByxParser::ParseError::ParseError(const std::string& msg, int row, int col)
	: msg(msg), row(row), col(col)
{

}

ByxParser& ByxParser::parse()
{
	try
	{
		// 词法分析
		lexer.lex();
	}
	catch (ByxLexer::LexError err)
	{
		throw ParseError(err.msg, err.row, err.col);
	}

	// 构建抽象语法树
	ast = parseProgram();

	// 扫描全局符号
	GlobalSymbolVisitor globalSymbolVisitor;
	ast->visit(globalSymbolVisitor);
	globalVarIndex = globalSymbolVisitor.getGlobalVarIndex();
	funcIndex = globalSymbolVisitor.getFuncIndex();
	funcParamCnt = globalSymbolVisitor.getFuncParamCount();

	cout << "globalVarIndex: " << endl;
	for (auto i = globalVarIndex.begin(); i != globalVarIndex.end(); ++i)
	{
		cout << i->first << " " << i->second << endl;
	}
	cout << endl;

	cout << "funcIndex: " << endl;
	for (auto i = funcIndex.begin(); i != funcIndex.end(); ++i)
	{
		cout << i->first << " " << i->second << endl;
	}
	cout << endl;

	cout << "funcParamCnt: " << endl;
	for (auto i = funcParamCnt.begin(); i != funcParamCnt.end(); ++i)
	{
		cout << i->first << " " << i->second << endl;
	}
	cout << endl;

	return *this;
}

std::string ByxParser::getASTString()
{
	ToStringVisitor visitor;
	ast->visit(visitor);
	return visitor.getString();
}

static int StrToInt(const std::string& str)
{
	stringstream ss(str);
	int val;
	ss >> val;
	return val;
}

static double StrToDouble(const std::string& str)
{
	stringstream ss(str);
	double val;
	ss >> val;
	return val;
}

std::shared_ptr<ASTNode> ByxParser::parseProgram()
{
	vector<shared_ptr<ASTNode>> stmts;
	while (lexer.nextType() != TokenType::End)
	{
		if (lexer.nextType() == TokenType::Keyword)
		{
			bool isExport = false;
			if (lexer.nextVal() == "public") // 是否有导出标志（public）
			{
				isExport = true;
				lexer.next();
			}

			if (lexer.nextVal() == "var") // 全局变量声明
			{
				stmts.push_back(parseVarDeclare(isExport));
			}
			else if (lexer.nextVal() == "function") // 函数声明
			{
				stmts.push_back(parseFunctionDeclare(isExport));
			}
			else // 出错
			{
				throw ParseError("Global scope only allow function declare and var declare.", lexer.row(), lexer.col());
			}
		}
		else // 出错：全局范围内只允许变量声明和函数声明
		{
			throw ParseError("Global scope only allow function declare and var declare.", lexer.row(), lexer.col());
		}
	}
	return make_shared<ProgramNode>(stmts);
}

std::shared_ptr<Statement> ByxParser::parseFunctionDeclare(bool isExport)
{
	// 读取function关键字
	Token token = lexer.next();

	// 读取函数名
	string name = lexer.read(TokenType::Ident).val;

	// 读取参数列表
	vector<string> paramName;
	lexer.read(TokenType::OpenBracket);
	while (lexer.nextType() != TokenType::CloseBracket)
	{
		paramName.push_back(lexer.read(TokenType::Ident).val);
		if (lexer.nextType() == TokenType::CloseBracket)
		{
			break;
		}
		else
		{
			lexer.read(TokenType::Comma);
		}
	}
	lexer.next();

	// 读取函数体
	vector<shared_ptr<Statement>> body;
	lexer.read(TokenType::OpenBrace);
	while (lexer.nextType() != TokenType::CloseBrace)
	{
		body.push_back(parseStatement());
	}
	lexer.next();

	// 构造函数声明节点
	return make_shared<FunctionDeclareNode>(name, paramName, body, isExport, token);
}

std::shared_ptr<Statement> ByxParser::parseVarDeclare(bool isExport)
{
	// 读取var关键字
	Token token = lexer.next();

	// 读取变量名
	string name = lexer.read(TokenType::Ident).val;

	// 读取初始化表达式
	shared_ptr<Expression> expr = make_shared<IntegerNode>(0);
	if (lexer.nextType() != TokenType::Semicolon)
	{
		token = lexer.read(TokenType::Assign);
		expr = parseExpr();
	}

	// 读取分号
	readSemicolon();

	// 构造变量声明节点
	return make_shared<VarDeclareNode>(name, expr, isExport, token);
}

std::shared_ptr<Statement> ByxParser::parseStatement()
{
	Token token = lexer.peek();
	if (token.type == TokenType::Keyword)
	{
		// 变量声明
		if (token.val == "var")
		{
			return parseVarDeclare(false);
		}
		// 函数返回
		else if (token.val == "return")
		{
			if (parsingForHeader)
			{
				throw ByxParser::ParseError("Return statement cannot appear in for header.", token.row, token.col);
			}

			return parseReturn();
		}
		// if
		else if (token.val == "if")
		{
			if (parsingForHeader)
			{
				throw ByxParser::ParseError("If statement cannot appear in for header.", token.row, token.col);
			}

			return parseIf();
		}
		// while
		else if (token.val == "while")
		{
			if (parsingForHeader)
			{
				throw ByxParser::ParseError("While statement cannot appear in for header.", token.row, token.col);
			}

			return parseWhile();
		}
		// break
		else if (token.val == "break")
		{
			if (parsingForHeader)
			{
				throw ByxParser::ParseError("Break statement cannot appear in for header.", token.row, token.col);
			}

			Token t = lexer.next();
			lexer.read(TokenType::Semicolon);
			return make_shared<BreakNode>(t);
		}
		// continue
		else if (token.val == "continue")
		{
			if (parsingForHeader)
			{
				throw ByxParser::ParseError("Continue statement cannot appear in for header.", token.row, token.col);
			}

			Token t = lexer.next();
			lexer.read(TokenType::Semicolon);
			return make_shared<ContinueNode>(t);
		}
		// for
		else if (token.val == "for")
		{
			return parseFor();
		}
		// 出错
		else
		{
			throw ParseError(string("Unexpected token: '") + token.val + "'", token.row, token.col);
		}
	}
	// 语句块
	else if (token.type == TokenType::OpenBrace)
	{
		return parseCodeBlock();
	}
	// 空语句
	else if (token.type == TokenType::Semicolon)
	{
		lexer.next();
		return make_shared<EmptyNode>();
	}
	// 变量赋值或函数调用
	else if (token.type == TokenType::Ident)
	{
		lexer.next();
		// 变量赋值
		if (lexer.nextType() == TokenType::Assign)
		{
			lexer.back();
			return parseVarAssign();
		}
		// 函数调用语句
		else
		{
			lexer.back();
			return parseFunctionCallStmt();
		}
	}
	// 出错
	else
	{
		throw ParseError(string("Unexpected token: '") + token.val + "'", token.row, token.col);
	}
}

std::shared_ptr<Statement> ByxParser::parseVarAssign()
{
	// 读取变量名
	string name = lexer.next().val;

	// 读取赋值符
	Token token = lexer.read(TokenType::Assign);

	// 读取表达式
	shared_ptr<Expression> expr = parseExpr();

	// 读取分号
	readSemicolon();

	// 构造赋值节点
	return make_shared<VarAssignNode>(name, expr, token);
}

std::shared_ptr<Statement> ByxParser::parseReturn()
{
	// 读取return关键字
	Token token = lexer.next();

	shared_ptr<Expression> expr = NULL;

	if (lexer.nextType() != TokenType::Semicolon)
	{
		expr = parseExpr();
		lexer.read(TokenType::Semicolon);
		return make_shared<ReturnNode>(true, expr, token);
	}
	else
	{
		lexer.next();
		return make_shared<ReturnNode>(false, expr, token);
	}
}

std::shared_ptr<Statement> ByxParser::parseCodeBlock()
{
	// 读取左大括号
	lexer.next();

	// 读取语句序列
	vector<shared_ptr<Statement>> stmts;
	while (lexer.nextType() != TokenType::CloseBrace)
	{
		stmts.push_back(parseStatement());
	}

	// 读取右大括号
	lexer.next();

	// 构造语句块节点
	return make_shared<CodeBlockNode>(stmts);
}

std::shared_ptr<Statement> ByxParser::parseFunctionCallStmt()
{
	// 读取函数名
	Token token = lexer.next();
	string name = token.val;

	// 读取左括号
	lexer.read(TokenType::OpenBracket);

	// 读取参数列表
	vector<shared_ptr<Expression>> exprs;
	while (lexer.nextType() != TokenType::CloseBracket)
	{
		exprs.push_back(parseExpr());
		if (lexer.nextType() == TokenType::CloseBracket)
		{
			break;
		}
		lexer.read(TokenType::Comma);
	}
	lexer.next();

	// 读取分号
	readSemicolon();

	// 构造函数调用语句节点
	return make_shared<FunctionCallStmtNode>(name, exprs, token);
}

std::shared_ptr<Statement> ByxParser::parseIf()
{
	// 读取if关键字
	Token token = lexer.next();

	// 读取左括号
	lexer.read(TokenType::OpenBracket);

	// 读取表达式
	shared_ptr<Expression> cond = parseExpr();

	// 读取右括号
	lexer.read(TokenType::CloseBracket);

	// 读取true branch
	shared_ptr<Statement> tBranch = parseStatement();

	// 读取false branch
	shared_ptr<Statement> fBranch = make_shared<EmptyNode>();
	if (lexer.nextType() == TokenType::Keyword && lexer.nextVal() == "else")
	{
		lexer.next();
		fBranch = parseStatement();
	}

	// 构造if节点
	return make_shared<IfNode>(cond, tBranch, fBranch, token);
}

std::shared_ptr<Statement> ByxParser::parseWhile()
{
	// 读取while关键字
	Token token = lexer.next();

	// 读取左括号
	lexer.read(TokenType::OpenBracket);

	// 读取条件表达式
	shared_ptr<Expression> cond = parseExpr();

	// 读取右括号
	lexer.read(TokenType::CloseBracket);

	// 读取循环体
	shared_ptr<Statement> body = parseStatement();

	// 构造while循环节点
	return make_shared<WhileNode>(cond, body, token);
}

std::shared_ptr<Statement> ByxParser::parseFor()
{
	// 读取for关键字
	Token token = lexer.next();

	// 读取左括号
	lexer.read(TokenType::OpenBracket);

	parsingForHeader = true;

	// 读取初始化语句
	shared_ptr<Statement> init = parseStatement();

	// 读取分号
	lexer.read(TokenType::Semicolon);

	// 读取条件表达式
	shared_ptr<Expression> cond = parseExpr();

	// 读取分号
	lexer.read(TokenType::Semicolon);

	// 读取更新语句
	shared_ptr<Statement> update = parseStatement();

	parsingForHeader = false;

	// 读取右括号
	lexer.read(TokenType::CloseBracket);

	// 读取循环体
	shared_ptr<Statement> body = parseStatement();

	// 构造for节点
	return make_shared<ForNode>(init, cond, update, body, token);
}

std::shared_ptr<Expression> ByxParser::parseExpr()
{
	shared_ptr<Expression> res = parseCmpExpr();
	while (lexer.nextType() == TokenType::And || lexer.nextType() == TokenType::Or)
	{
		Token token = lexer.next();
		if (token.type == TokenType::And)
		{
			res = make_shared<BinaryOpNode>(BinaryOp::And, res, parseCmpExpr(), token);
		}
		else
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Or, res, parseCmpExpr(), token);
		}
	}
	return res;
}

std::shared_ptr<Expression> ByxParser::parseCmpExpr()
{
	shared_ptr<Expression> res = parseArithExpr();
	if (lexer.nextType() == TokenType::Equal)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::Equ, res, parseArithExpr(), token);
	}
	else if (lexer.nextType() == TokenType::NotEqual)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::NotEqu, res, parseArithExpr(), token);
	}
	else if (lexer.nextType() == TokenType::LargerThan)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::Larger, res, parseArithExpr(), token);
	}
	else if (lexer.nextType() == TokenType::LessThan)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::Less, res, parseArithExpr(), token);
	}
	else if (lexer.nextType() == TokenType::LargerEqualThan)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::LargerEqu, res, parseArithExpr(), token);
	}
	else if (lexer.nextType() == TokenType::LessEqualThan)
	{
		Token token = lexer.next();
		return make_shared<BinaryOpNode>(BinaryOp::LessEqu, res, parseArithExpr(), token);
	}
	else
	{
		return res;
	}
}

std::shared_ptr<Expression> ByxParser::parseArithExpr()
{
	shared_ptr<Expression> res = parseTerm();
	while (lexer.nextType() == TokenType::Add || lexer.nextType() == TokenType::Sub)
	{
		Token token = lexer.next();
		if (token.type == TokenType::Add)
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Add, res, parseTerm(), token);
		}
		else
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Sub, res, parseTerm(), token);
		}
	}
	return res;
}

std::shared_ptr<Expression> ByxParser::parseTerm()
{
	shared_ptr<Expression> res = parseFactor();
	while (lexer.nextType() == TokenType::Mul || lexer.nextType() == TokenType::Div || lexer.nextType() == TokenType::Rem)
	{
		Token token = lexer.next();
		if (token.type == TokenType::Mul)
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Mul, res, parseFactor(), token);
		}
		else if (token.type == TokenType::Div)
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Div, res, parseFactor(), token);
		}
		else
		{
			res = make_shared<BinaryOpNode>(BinaryOp::Rem, res, parseFactor(), token);
		}
	}
	return res;
}

std::shared_ptr<Expression> ByxParser::parseFactor()
{
	Token token = lexer.next();
	if (token.type == TokenType::Integer) // 整型常量
	{
		return make_shared<IntegerNode>(StrToInt(token.val));
	}
	else if (token.type == TokenType::Double) // 浮点型常量
	{
		return make_shared<DoubleNode>(StrToDouble(token.val));
	}
	else if (token.type == TokenType::OpenBracket) // 括号表达式
	{
		shared_ptr<Expression> res = parseExpr();
		lexer.read(TokenType::CloseBracket);
		return res;
	}
	else if (token.type == TokenType::Add) // 正号
	{
		return make_shared<UnaryOpNode>(UnaryOp::Pos, parseFactor(), token);
	}
	else if (token.type == TokenType::Sub) // 负号
	{
		return make_shared<UnaryOpNode>(UnaryOp::Neg, parseFactor(), token);
	}
	else if (token.type == TokenType::Not) // 非运算
	{
		return make_shared<UnaryOpNode>(UnaryOp::Not, parseFactor(), token);
	}
	else if (token.type == TokenType::Ident) // 变量或函数调用
	{
		if (lexer.nextType() == TokenType::OpenBracket) // 函数调用
		{
			lexer.back();
			return parseFunctionCallExpr();
		}
		else // 变量
		{
			return make_shared<VarNode>(token.val, token);
		}
	}
	else
	{
		throw ParseError(string("Unexpected token: ") + token.toString(), token.row, token.col);
	}
}

std::shared_ptr<Expression> ByxParser::parseFunctionCallExpr()
{
	// 读取函数名
	Token token = lexer.next();
	string name = token.val;

	// 读取左括号
	lexer.read(TokenType::OpenBracket);

	// 读取参数列表
	vector<shared_ptr<Expression>> exprs;
	while (lexer.nextType() != TokenType::CloseBracket)
	{
		exprs.push_back(parseExpr());
		if (lexer.nextType() == TokenType::CloseBracket)
		{
			break;
		}
		lexer.read(TokenType::Comma);
	}
	lexer.next();

	// 构造函数调用表达式节点
	return make_shared<FunctionCallExprNode>(name, exprs, token);
}

void ByxParser::readSemicolon()
{
	if (!parsingForHeader)
	{
		lexer.read(TokenType::Semicolon);
	}
}