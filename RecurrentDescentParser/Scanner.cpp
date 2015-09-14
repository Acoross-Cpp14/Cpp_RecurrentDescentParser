#include <iostream>
#include <iterator>
#include <regex>

#include "Scanner.h"

using namespace std;

// 사용 가능한 Token의 regex 를 정의.
const FuncScanner::TokenDefine FuncScanner::m_TokenDefines[] = {
#define add_token(type, rex) {type, rex}
	token_list
#undef add_token
};
	
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

// 토큰의 이름을 얻어내는 함수.
const wchar_t* FuncScanner::Token::TokenToName(TKType type)
{
#define add_token(type, rex) L#type
	const wchar_t* nameMap[] = {
		token_list
	};
	return nameMap[type];
#undef add_token
}

FuncScanner::Token FuncScanner::Scan(wchar_t*& input)
{
	Token ret;
	ret.type = TK_NULL;
	ret.data = L"";

	if (nullptr == input)
	{
		return ret;
	}

	// skip any initial white space (' ', '/t', '/n')
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

	std::wcout << input << std::endl;

	// TokenDefines 를 이용하여 regular expression 으로 token get.
	unsigned int nTDCnt = sizeof(m_TokenDefines) / sizeof(TokenDefine);
	for (unsigned int i = 1; i < nTDCnt; ++i)	// 0 -> null
	{
		const TokenDefine& td = m_TokenDefines[i];

		if (td.type == TK_NULL)
		{
			continue;
		}

		std::wcmatch wcm;
		std::wregex word_regex(td.rx);
		//if (std::regex_match(input, wcm, word_regex, std::regex_constants::match_continuous))
		if (std::regex_search(input, wcm, word_regex, std::regex_constants::match_continuous))
		{
			std::wcout << "true: ";
			std::wcout << Token::TokenToName(td.type);
			std::wcout << ":" << wcm.size() << ",";
			
			for (int j = 0; j < wcm.size(); ++j)
			{
				std::wcout << wcm[j] << ",";
			}
			
			std::wcout << std::endl;
			break;
		}
		/*else
		{
			std::wcout << "false: ";
			std::wcout << Token::TokenToName(td.type);
			std::wcout << ":" << wcm.str() << std::endl;
		}*/
	}

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
