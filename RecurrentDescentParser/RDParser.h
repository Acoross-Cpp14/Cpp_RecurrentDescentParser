#pragma once

#include <string>
#include <unordered_map>

using namespace std;

#define token_list \
	add_token(TK_NULL, nullptr),	\
	add_token(DIGIT, L"[1-9]([0-9]*)"),	\
	add_token(STR, L"[a-zA-Z]([a-zA-Z0-9_]*)"),	\
	add_token(COMMA, L","),	\
	add_token(LPAREN, L"("),	\
	add_token(RPAREN, L")")

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

const wchar_t* TokenToName(TKType type);

struct Token
{
	TKType type;
	wstring data;
};

Token Scan(wchar_t*& input);