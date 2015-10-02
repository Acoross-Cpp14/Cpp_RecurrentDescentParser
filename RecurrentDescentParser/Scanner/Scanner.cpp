#include <iostream>
#include <iterator>
#include <regex>

#include "Scanner.h"

using namespace std;

namespace AcorossScanner
{
	/*void get_next_char(wchar_t*& input)
	{
		++(input);
	}*/

	/*static bool isID(wchar_t wch)
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

	static bool isAnyLetter(wchar_t wch)
	{
		if (wch == L'\0' || wch == L' ' || wch == L'\t' || wch == L'\n')
			return false;

		if (wch == L'(' || wch == L')' || wch == L',')
			return false;

		return true;
	}*/
	

	// 토큰의 이름을 얻어내는 함수.
	/*const wchar_t* TokenToName(TKTypeOut type)
	{
#define add_token(type, rex) L#type
		const wchar_t* nameMap[] = {
			token_list
		};
		return nameMap[type];
#undef add_token
	}*/


//	Scanner::Token Scanner::Scan(wchar_t*& input)
//	{
//		Token ret;
//		ret.type = TK_NULL;
//		ret.data = L"";
//
//		if (nullptr == input)
//		{
//			return ret;
//		}
//		
//		// skip any initial white space (' ', '/t', '/n')
//		{
//			bool bContinue = true;
//			while (bContinue)
//			{
//				switch (*input)
//				{
//				case L' ':
//				case L'\t':
//				case L'\n':
//					get_next_char(input);
//					break;
//				default:
//					bContinue = false;
//					break;
//				}
//			}
//		}
//		
//		wchar_t* pStart = input;
//
//		// Comment 
//		{
//			if (*input == L'/')
//			{
//				get_next_char(input);
//				if (*input == L'/')
//				{
//					get_next_char(input);
//					while (*input != L'\n' && *input != L'\0')
//					{
//						get_next_char(input);
//					}
//
//					ret.type = COMMENT;
//					ret.data = L"";
//
//					return ret;
//				}
//			}			
//		}
//
//		// TokenDefines 를 이용하여 regular expression 으로 token get.
//		unsigned int nTDCnt = _countof(m_TokenDefines);
//		for (unsigned int i = 1; i < nTDCnt; ++i)	// 0 -> null
//		{
//			const TokenDefine& td = m_TokenDefines[i];
//
//			if (td.type == TK_NULL)
//			{
//				continue;
//			}
//
//			std::wcmatch wcm;
//			std::wregex word_regex(td.rx);
//			//if (std::regex_match(input, wcm, word_regex, std::regex_constants::match_continuous))
//			if (std::regex_search(input, wcm, word_regex, std::regex_constants::match_continuous))
//			{
//				input = input + wcm[0].length();
//				ret.type = td.type;
//				ret.data = wcm[0];
//
//				break;
//			}
//		}
///*
//		if (*input == L'(')
//		{
//			get_next_char(input);
//			ret.type = LPAREN;
//			ret.data = wstring(pStart, 1);
//
//			return ret;
//		}
//		if (*input == L')')
//		{
//			get_next_char(input);
//			ret.type = RPAREN;
//			ret.data = wstring(pStart, 1);
//
//			return ret;
//		}
//		if (*input == L';')
//		{
//			get_next_char(input);
//			ret.type = SEMICOLON;
//			ret.data = wstring(pStart, 1);
//
//			return ret;
//		}
//		if (*input == L',')
//		{
//			get_next_char(input);
//			ret.type = COMMA;
//			ret.data = wstring(pStart, 1);
//
//			return ret;
//		}
//
//		if (*input == L'-')
//		{
//			get_next_char(input);
//			while (isDigit(*input))
//			{
//				get_next_char(input);
//			}
//
//			if (false == isLetter(*input))
//			{
//				ret.type = DIGIT;
//				ret.data = wstring(pStart, static_cast<int>(input - pStart));
//				return ret;
//			}
//
//			while (isLetter(*input))
//			{
//				get_next_char(input);
//			}
//
//			ret.type = ID;
//			ret.data = wstring(pStart, static_cast<int>(input - pStart));
//			return ret;
//		}
//
//		if (isDigit(*input))
//		{
//			get_next_char(input);
//			while (isDigit(*input))
//			{
//				get_next_char(input);
//			}
//
//			if (false == isLetter(*input))
//			{
//				ret.type = DIGIT;
//				ret.data = wstring(pStart, static_cast<int>(input - pStart));
//				return ret;
//			}
//
//			while (isLetter(*input))
//			{
//				get_next_char(input);
//			}
//
//			ret.type = ID;
//			ret.data = wstring(pStart, static_cast<int>(input - pStart));
//			return ret;
//		}
//
//		if (isLetter(*input))
//		{
//			get_next_char(input);
//			while (isDigit(*input) || isLetter(*input))
//			{
//				get_next_char(input);
//			}
//
//			ret.type = STR;
//			ret.data = wstring(pStart, static_cast<int>(input - pStart));
//			return ret;
//		}
//
//		if (isAnyLetter(*input))
//		{
//			get_next_char(input);
//			while (isAnyLetter(*input))
//			{
//				get_next_char(input);
//			}
//
//			ret.type = ANYWORD;
//			ret.data = wstring(pStart, static_cast<int>(input - pStart));
//			return ret;
//		}*/
//
//		return ret;
//	}
}