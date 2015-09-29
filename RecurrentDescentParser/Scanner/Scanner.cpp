#include <iostream>
#include <iterator>
#include <regex>

#include "Scanner.h"

using namespace std;

namespace AcorossScanner
{
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

	static bool isID(wchar_t wch)
	{
		if (wch >= L'0' && wch <= L'9')
			return true;
		else
			return false;
	}

	static bool isDigit(wchar_t wch)
	{
		if (wch >= L'0' && wch <= L'9')
			return true;
		else
			return false;
	}

	static bool isLetter(wchar_t wch)
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

		//// TokenDefines 를 이용하여 regular expression 으로 token get.
		//unsigned int nTDCnt = _countof(m_TokenDefines);
		//for (unsigned int i = 1; i < nTDCnt; ++i)	// 0 -> null
		//{
		//	const TokenDefine& td = m_TokenDefines[i];

		//	if (td.type == TK_NULL)
		//	{
		//		continue;
		//	}

		//	std::wcmatch wcm;
		//	std::wregex word_regex(td.rx);
		//	//if (std::regex_match(input, wcm, word_regex, std::regex_constants::match_continuous))
		//	if (std::regex_search(input, wcm, word_regex, std::regex_constants::match_continuous))
		//	{
		//		for (int j = 0; j < wcm.size(); ++j)
		//		{
		//			std::wcout << wcm[j] << ",";
		//		}
		//		break;
		//	}
		//}

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

			if (false == isLetter(*input))
			{
				ret.type = DIGIT;
				ret.data = wstring(pStart, static_cast<int>(input - pStart));
				return ret;
			}

			while (isLetter(*input))
			{
				get_next_char(input);
			}

			ret.type = ID;
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
}