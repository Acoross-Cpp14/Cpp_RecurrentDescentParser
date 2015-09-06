#include <iostream>

#include <regex>
#include <iterator>

#include "RDParser.h"

using namespace std;


TokenDefine g_Tokens[] = {
#define add_token(type, rex) {type, rex}
	token_list
#undef add_token
};

const wchar_t* TokenToName(TKType type)
{
#define add_token(type, rex) L#type
	const wchar_t* nameMap[] = {
		token_list
	};

	return nameMap[type];
#undef add_token
}

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

void get_next_char(wchar_t*& input)
{
	++(input);
}

bool isDigit(wchar_t wch)
{
	if (wch >= L'0' && wch <= L'9')
		return true;
	else
		return false;
}

bool isLetter(wchar_t wch)
{
	if (wch >= L'A' && wch <= L'z')
		return true;
	else
		return false;
}

Token Scan(wchar_t*& input)
{
	Token ret;
	ret.type = TK_NULL;
	ret.data = L"";

	if (nullptr == input)
	{
		return ret;
	}

	// skil any initial white space (' ', '/t', '/n')
	{
		bool bContinue = true;
		while (bContinue)
		{
			switch (*input)
			{
			case L' ':
			case L'\t':
			case L'\n':
				get_next_char(input);
				break;
			default:
				bContinue = false;
				break;
			}
		}
	}

	wchar_t* pStart = input;

	if (*input == L'(')
	{
		get_next_char(input);
		ret.type = LPAREN;
		ret.data = wstring(pStart, 1);

		return ret;
	}
	if (*input == L')')
	{
		get_next_char(input);
		ret.type = RPAREN;
		ret.data = wstring(pStart, 1);

		return ret;
	}
	if (*input == L',')
	{
		get_next_char(input);
		ret.type = COMMA;
		ret.data = wstring(pStart, 1);

		return ret;
	}

	if (isDigit(*input))
	{
		get_next_char(input);
		while (isDigit(*input))
		{
			get_next_char(input);
		}

		ret.type = DIGIT;
		ret.data = wstring(pStart, static_cast<int>(input - pStart));
		return ret;
	}

	if (isLetter(*input))
	{
		get_next_char(input);
		while (isDigit(*input) || isLetter(*input))
		{
			get_next_char(input);
		}

		ret.type = STR;
		ret.data = wstring(pStart, static_cast<int>(input - pStart));
		return ret;
	}

	return ret;
}