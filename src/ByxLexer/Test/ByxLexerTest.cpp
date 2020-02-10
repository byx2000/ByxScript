#include "ByxLexerTest.h"
#include "../../ByxLexer/ByxLexer.h"
#include "../../Common/TestUtil.h"

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

void ByxLexerTest::Run()
{
	StopWatch watch;
	watch.begin();

	string in = "src\\ByxLexer\\Test\\TestCase\\Lexer_in.txt";
	string out = "src\\ByxLexer\\Test\\TestCase\\Lexer_out.txt";
	ifstream f1(in);
	ifstream f2(out);

	if (!f1)
	{
		cout << "Failed to open file: " << in << endl;
		exit(0);
	}
	if (!f2)
	{
		cout << "Failed to open file: " << out << endl;
		exit(0);
	}

	string input;
	while (getline(f1, input))
	{
		if (input == "") continue;

		ByxLexer lexer(input);
		try
		{
			lexer.lex();
		}
		catch (ByxLexer::LexError err)
		{
			cout << "ByxLexer test failed: runtime error" << endl;
			cout << "prog: " << input << endl;
			exit(0);
		}

		Token token;
		while ((token = lexer.next()).type != TokenType::End)
		{
			string output;
			do
			{
				getline(f2, output);
			} while (output == "");

			if (token.toString() != output)
			{
				cout << "ByxLexer test failed: wrong answer" << endl;
				cout << "prog: " << input << endl;
				cout << "output: " << token.toString() << endl;
				cout << "ans: " << output << endl;
				exit(0);
			}
		}
	}

	watch.end();
	cout << "ByxLexer tests passed!\ttime: " << watch.duration() << "s" << endl;
}