#include <iostream>
#include "ByxLexer/Test/ByxLexerTest.h"
#include "ByxParser/ByxParser.h"

using namespace std;

int main()
{
	// ÄÚ´æÐ¹Â©¼ì²â
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ByxLexer²âÊÔ
	ByxLexerTest::Run();

	try
	{
		ByxParser parser("var g1 = 100; public var g2 = 3.14; function f(n) { return n * n; } public function main(){ var i = 100; var j = 3.14; var s = 0; for (var i = 0; i < 6; i = i + 1) { s = s + i; } }");
		parser.parse();
		cout << parser.getASTString() << endl;
	}
	catch (ByxParser::ParseError err)
	{
		cout << err.msg << endl;
		cout << "row: " << err.row << endl;
		cout << "col: " << err.col << endl;
	}

	return 0;
}