#pragma once

#include <string>
#include <unordered_map>

using namespace std;

#define NOINSTANCE(x) private: x(){}
#define NOCOPY(x) private: x(x&){}
#define NOINSTANCE_AND_NOCOPY(x) NOINSTANCE(x);NOCOPY(x);

class FuncScanner
{
public:

#define token_list  \
	add_token(TK_NULL, nullptr),	\
	add_token(COMMA, L","),	\
	add_token(LPAREN, L"\\("),	\
	add_token(RPAREN, L"\\)"),	\
	add_token(STR, L"[a-zA-Z]([a-zA-Z0-9_]*)"),	\
	add_token(ID, L"[0-9a-zA-z]+"),	\
	add_token(DIGIT, L"[1-9]([0-9]*)")

#define add_token(type, rex) type
	enum TKType
	{
		token_list
	};
#undef add_token

	struct TokenDefine
	{
		TKType type;
		const wchar_t* rx;
	};

	struct Token
	{
		TKType type;
		wstring data;

		static const wchar_t* TokenToName(TKType type);
	};

	Token Scan(wchar_t*& input);

	static const TokenDefine m_TokenDefines[];
};