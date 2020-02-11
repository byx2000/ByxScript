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
		// �ʷ�����
		lexer.lex();
	}
	catch (ByxLexer::LexError err)
	{
		throw ParseError(err.msg, err.row, err.col);
	}

	// ���������﷨��
	ast = parseProgram();

	// ɨ��ȫ�ַ���
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
			if (lexer.nextVal() == "public") // �Ƿ��е�����־��public��
			{
				isExport = true;
				lexer.next();
			}

			if (lexer.nextVal() == "var") // ȫ�ֱ�������
			{
				stmts.push_back(parseVarDeclare(isExport));
			}
			else if (lexer.nextVal() == "function") // ��������
			{
				stmts.push_back(parseFunctionDeclare(isExport));
			}
			else // ����
			{
				throw ParseError("Global scope only allow function declare and var declare.", lexer.row(), lexer.col());
			}
		}
		else // ����ȫ�ַ�Χ��ֻ������������ͺ�������
		{
			throw ParseError("Global scope only allow function declare and var declare.", lexer.row(), lexer.col());
		}
	}
	return make_shared<ProgramNode>(stmts);
}

std::shared_ptr<Statement> ByxParser::parseFunctionDeclare(bool isExport)
{
	// ��ȡfunction�ؼ���
	Token token = lexer.next();

	// ��ȡ������
	string name = lexer.read(TokenType::Ident).val;

	// ��ȡ�����б�
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

	// ��ȡ������
	vector<shared_ptr<Statement>> body;
	lexer.read(TokenType::OpenBrace);
	while (lexer.nextType() != TokenType::CloseBrace)
	{
		body.push_back(parseStatement());
	}
	lexer.next();

	// ���캯�������ڵ�
	return make_shared<FunctionDeclareNode>(name, paramName, body, isExport, token);
}

std::shared_ptr<Statement> ByxParser::parseVarDeclare(bool isExport)
{
	// ��ȡvar�ؼ���
	Token token = lexer.next();

	// ��ȡ������
	string name = lexer.read(TokenType::Ident).val;

	// ��ȡ��ʼ�����ʽ
	shared_ptr<Expression> expr = make_shared<IntegerNode>(0);
	if (lexer.nextType() != TokenType::Semicolon)
	{
		token = lexer.read(TokenType::Assign);
		expr = parseExpr();
	}

	// ��ȡ�ֺ�
	readSemicolon();

	// ������������ڵ�
	return make_shared<VarDeclareNode>(name, expr, isExport, token);
}

std::shared_ptr<Statement> ByxParser::parseStatement()
{
	Token token = lexer.peek();
	if (token.type == TokenType::Keyword)
	{
		// ��������
		if (token.val == "var")
		{
			return parseVarDeclare(false);
		}
		// ��������
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
		// ����
		else
		{
			throw ParseError(string("Unexpected token: '") + token.val + "'", token.row, token.col);
		}
	}
	// ����
	else if (token.type == TokenType::OpenBrace)
	{
		return parseCodeBlock();
	}
	// �����
	else if (token.type == TokenType::Semicolon)
	{
		lexer.next();
		return make_shared<EmptyNode>();
	}
	// ������ֵ��������
	else if (token.type == TokenType::Ident)
	{
		lexer.next();
		// ������ֵ
		if (lexer.nextType() == TokenType::Assign)
		{
			lexer.back();
			return parseVarAssign();
		}
		// �����������
		else
		{
			lexer.back();
			return parseFunctionCallStmt();
		}
	}
	// ����
	else
	{
		throw ParseError(string("Unexpected token: '") + token.val + "'", token.row, token.col);
	}
}

std::shared_ptr<Statement> ByxParser::parseVarAssign()
{
	// ��ȡ������
	string name = lexer.next().val;

	// ��ȡ��ֵ��
	Token token = lexer.read(TokenType::Assign);

	// ��ȡ���ʽ
	shared_ptr<Expression> expr = parseExpr();

	// ��ȡ�ֺ�
	readSemicolon();

	// ���츳ֵ�ڵ�
	return make_shared<VarAssignNode>(name, expr, token);
}

std::shared_ptr<Statement> ByxParser::parseReturn()
{
	// ��ȡreturn�ؼ���
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
	// ��ȡ�������
	lexer.next();

	// ��ȡ�������
	vector<shared_ptr<Statement>> stmts;
	while (lexer.nextType() != TokenType::CloseBrace)
	{
		stmts.push_back(parseStatement());
	}

	// ��ȡ�Ҵ�����
	lexer.next();

	// ��������ڵ�
	return make_shared<CodeBlockNode>(stmts);
}

std::shared_ptr<Statement> ByxParser::parseFunctionCallStmt()
{
	// ��ȡ������
	Token token = lexer.next();
	string name = token.val;

	// ��ȡ������
	lexer.read(TokenType::OpenBracket);

	// ��ȡ�����б�
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

	// ��ȡ�ֺ�
	readSemicolon();

	// ���캯���������ڵ�
	return make_shared<FunctionCallStmtNode>(name, exprs, token);
}

std::shared_ptr<Statement> ByxParser::parseIf()
{
	// ��ȡif�ؼ���
	Token token = lexer.next();

	// ��ȡ������
	lexer.read(TokenType::OpenBracket);

	// ��ȡ���ʽ
	shared_ptr<Expression> cond = parseExpr();

	// ��ȡ������
	lexer.read(TokenType::CloseBracket);

	// ��ȡtrue branch
	shared_ptr<Statement> tBranch = parseStatement();

	// ��ȡfalse branch
	shared_ptr<Statement> fBranch = make_shared<EmptyNode>();
	if (lexer.nextType() == TokenType::Keyword && lexer.nextVal() == "else")
	{
		lexer.next();
		fBranch = parseStatement();
	}

	// ����if�ڵ�
	return make_shared<IfNode>(cond, tBranch, fBranch, token);
}

std::shared_ptr<Statement> ByxParser::parseWhile()
{
	// ��ȡwhile�ؼ���
	Token token = lexer.next();

	// ��ȡ������
	lexer.read(TokenType::OpenBracket);

	// ��ȡ�������ʽ
	shared_ptr<Expression> cond = parseExpr();

	// ��ȡ������
	lexer.read(TokenType::CloseBracket);

	// ��ȡѭ����
	shared_ptr<Statement> body = parseStatement();

	// ����whileѭ���ڵ�
	return make_shared<WhileNode>(cond, body, token);
}

std::shared_ptr<Statement> ByxParser::parseFor()
{
	// ��ȡfor�ؼ���
	Token token = lexer.next();

	// ��ȡ������
	lexer.read(TokenType::OpenBracket);

	parsingForHeader = true;

	// ��ȡ��ʼ�����
	shared_ptr<Statement> init = parseStatement();

	// ��ȡ�ֺ�
	lexer.read(TokenType::Semicolon);

	// ��ȡ�������ʽ
	shared_ptr<Expression> cond = parseExpr();

	// ��ȡ�ֺ�
	lexer.read(TokenType::Semicolon);

	// ��ȡ�������
	shared_ptr<Statement> update = parseStatement();

	parsingForHeader = false;

	// ��ȡ������
	lexer.read(TokenType::CloseBracket);

	// ��ȡѭ����
	shared_ptr<Statement> body = parseStatement();

	// ����for�ڵ�
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
	if (token.type == TokenType::Integer) // ���ͳ���
	{
		return make_shared<IntegerNode>(StrToInt(token.val));
	}
	else if (token.type == TokenType::Double) // �����ͳ���
	{
		return make_shared<DoubleNode>(StrToDouble(token.val));
	}
	else if (token.type == TokenType::OpenBracket) // ���ű��ʽ
	{
		shared_ptr<Expression> res = parseExpr();
		lexer.read(TokenType::CloseBracket);
		return res;
	}
	else if (token.type == TokenType::Add) // ����
	{
		return make_shared<UnaryOpNode>(UnaryOp::Pos, parseFactor(), token);
	}
	else if (token.type == TokenType::Sub) // ����
	{
		return make_shared<UnaryOpNode>(UnaryOp::Neg, parseFactor(), token);
	}
	else if (token.type == TokenType::Not) // ������
	{
		return make_shared<UnaryOpNode>(UnaryOp::Not, parseFactor(), token);
	}
	else if (token.type == TokenType::Ident) // ������������
	{
		if (lexer.nextType() == TokenType::OpenBracket) // ��������
		{
			lexer.back();
			return parseFunctionCallExpr();
		}
		else // ����
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
	// ��ȡ������
	Token token = lexer.next();
	string name = token.val;

	// ��ȡ������
	lexer.read(TokenType::OpenBracket);

	// ��ȡ�����б�
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

	// ���캯�����ñ��ʽ�ڵ�
	return make_shared<FunctionCallExprNode>(name, exprs, token);
}

void ByxParser::readSemicolon()
{
	if (!parsingForHeader)
	{
		lexer.read(TokenType::Semicolon);
	}
}