#include <iostream>
#include <iterator>
#include <string>
#include <regex>

#include "Scanner.h"


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
	wchar_t* input = L" dfdg 12322 12dd id3d ,83k(dfd dddd)";
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

	system("pause");
}