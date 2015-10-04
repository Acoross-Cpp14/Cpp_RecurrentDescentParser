#include <iostream>
#include <iterator>
#include <string>
#include <regex>

#include "Scanner/Scanner.h"
#include "Parser/Parser.h"

//temp
#include <fstream>
#include <locale>
#include <codecvt>
#include <list>
//temp

using namespace AcorossScanner;

class ParserTemp
{
public:
	enum ParserSymbolType
	{

	};

	struct ParserSymbol
	{
		ParserSymbolType eType;

	};

	// derivation
	bool Derivation()
	{
		// or 조건은 한 함수로 표현하지 않는다.
		list<ParserSymbol> symbol_list;
		for (const auto it_symbol = symbol_list.begin(); it_symbol != symbol_list.end(); ++it_symbol)
		{
			// 
		}

		return true;
	}

	void Run()
	{
		FuncScanner scanner;
		setlocale(LC_ALL, "");

		std::wifstream wis(L"monster.csv", std::ifstream::binary);
		if (false == wis.is_open())
			return;
		
		// apply BOM-sensitive UTF-16 facet
		wis.imbue(std::locale(wis.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

		int nScriptLine = 0;
		std::wstring wline;

		bool ret = true;
		if (std::getline(wis, wline))	// 한 줄 읽어들인다.
		{
			memset(m_buf, 0, sizeof(m_buf));
			wline._Copy_s(m_buf, 2000, wline.size(), 0);
			wchar_t* input = m_buf;
			input = m_buf;

			scanner.Scan(input);
		}
	}

private:
	wchar_t m_buf[2000];
};

void RegexSample()
{
	std::string s = "Some people, 0192 23222 with a problem, think "
		"\"I know, I'll use 1123 expressions.\" "
		"Now they have two problems.";

	std::regex word_regex("[1-9]([0-9]*)");
	auto words_begin =
		std::sregex_iterator(s.begin(), s.end(), word_regex);
	auto words_end = std::sregex_iterator();

	std::cout << "Found "
		<< std::distance(words_begin, words_end)
		<< " words\n";

	for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
		std::smatch match = *i;
		std::string match_str = match.str();
		std::cout << "  " << match_str << '\n';
	}
}

int main()
{
	/*wchar_t* input = L" dfdg 12322 12dd id3d ,83k(dfd dddd)";
	std::wstring buf = input;

	std::wcout << L"input: " << input << std::endl;

	FuncScanner::Token ret;
	FuncScanner scanner;

	ret = scanner.Scan(input);
	std::wcout << "ret - " << FuncScanner::Token::TokenToName(ret.type) << L":" << ret.data << std::endl << std::endl;

	while (ret.type != FuncScanner::TK_NULL)
	{
		ret = scanner.Scan(input);
		std::wcout << "ret - " << FuncScanner::Token::TokenToName(ret.type) << L":" << ret.data << std::endl << std::endl;
	}
*/

	FuncScannerDefine fsd;
	fsd.GetTokenDefine(fsd.ANYWORD);
	
	////////////////////////
	AcorossParser::Parser parser;
	parser.Load(L"monster.csv");

	system("pause");
}