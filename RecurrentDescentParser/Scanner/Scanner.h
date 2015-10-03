#pragma once

// STL headers
#include <string>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <regex>

// My Project's headers
#include "../default.h"

#define SCANNERDEF_BEGIN(T) struct T
#define SCANNERDEF_END	;

using namespace std;

namespace AcorossScanner
{
	template <typename TkTy>
	struct TokenDefine
	{
		TkTy type;
		const wchar_t* rx;
	};

	//void get_next_char(wchar_t*& input);
		
	template <typename ScannerDefine>
	class Scanner : public ScannerDefine
	{
	public:
		typedef ScannerDefine _MyDef;
		typedef typename _MyDef::TokenType TokenType;
		typedef typename _MyDef::_TkDef _MyTkDef;

		struct Token
		{
			TokenType type;
			wstring data;
		};
		
	public:
		Scanner()
		{
		}

		Token Scan(wchar_t*& input);

	private:
		void get_next_char(wchar_t*& input)
		{
			++(input);
		}

	private:
		NO_COPY(Scanner);
	};
	
	template <typename ScannerDefine>
	typename Scanner<ScannerDefine>::Token Scanner<ScannerDefine>::Scan(wchar_t*& input)
	{
		Token ret;
		ret.type = TK_MAX;
		ret.data = L"";

		if (nullptr == input)
		{
			return ret;
		}

		// skip white spaces and comments
		{
			// skip any initial white space (' ', '/t', '/n')
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

			// skip Comment 
			if (*input == L'/')
			{
				get_next_char(input);
				if (*input == L'/')
				{
					get_next_char(input);
					while (*input != L'\n' && *input != L'\0')
					{
						get_next_char(input);
					}
				}
			}
		}

		wchar_t* pStart = input;

		// TokenDefines 를 이용하여 regular expression 으로 token get.
		unsigned int nTDCnt = TokenType::TK_MAX;
		for (unsigned int i = 0; i < nTDCnt; ++i)	// 0 -> null
		{
			const _MyTkDef& td = GetTokenDefine(static_cast<TokenType>(i));

			if (td.type == TokenType::TK_MAX)
			{
				continue;
			}

			std::wcmatch wcm;
			std::wregex word_regex(td.rx);
			//if (std::regex_match(input, wcm, word_regex, std::regex_constants::match_continuous))
			if (std::regex_search(input, wcm, word_regex, std::regex_constants::match_continuous))
			{
				input = input + wcm[0].length();
				ret.type = td.type;
				ret.data = wcm[0];

				break;
			}
		}

		return ret;
	}
}