#pragma once

#include <string>
#include <unordered_map>
#include <iostream>
#include <iterator>
#include <regex>

using namespace std;

namespace AcorossScanner
{
	void get_next_char(wchar_t*& input);

	template<typename T>
	struct TokenDefine
	{
		T type;
		const wchar_t* rx;
	};

	template <typename TKEnum>
	class Scanner
	{
	public:
		typedef TKEnum TKType;
		typedef TokenDefine<TKType> TKDefine;
		
		struct Token
		{
			TKType type;
			wstring data;
		};

		Scanner(TKDefine* tokenDefines)
		{
			m_pTokenDefines = tokenDefines;
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

			// Comment 
			{
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

						ret.type = COMMENT;
						ret.data = L"";

						return ret;
					}
				}
			}

			// TokenDefines 를 이용하여 regular expression 으로 token get.
			unsigned int nTDCnt = TKType::TK_MAX;
			for (unsigned int i = 1; i < nTDCnt; ++i)	// 0 -> null
			{
				const TKDefine& td = m_pTokenDefines[i];

				if (td.type == TK_NULL)
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

		TKDefine* m_pTokenDefines;
	};

	//template<typename TKEnum>
	//Token Scanner<TKEnum>::Scan(wchar_t*& input)
}