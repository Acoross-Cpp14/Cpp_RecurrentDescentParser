#pragma once

#include "Scanner.h"

#define func_token_list  \
	add_token(TK_NULL, nullptr),	\
	add_token(COMMENT, L"//(.*)"),	\
	add_token(SEMICOLON, L";"),	\
	add_token(COMMA, L","),	\
	add_token(LPAREN, L"\\("),	\
	add_token(RPAREN, L"\\)"),	\
	add_token(STR, L"[a-zA-Z]([a-zA-Z0-9_]*)"),	\
	add_token(ID, L"([0-9]+)([a-zA-z]+)"),	\
	add_token(DIGIT, L"(-)?[1-9]([0-9]*)"), \
	add_token(ANYWORD, L"([^ \t\n\\(\\),])+"), \
	add_token(TK_MAX, nullptr)

#define add_token(type, rex) type
enum FuncTokenType
{
	func_token_list
};
#undef add_token

// 사용 가능한 Token의 regex 를 정의.
extern AcorossScanner::TokenDefine<FuncTokenType> _TokenDefines[];

const wchar_t* TokenToName(FuncTokenType type);

typedef AcorossScanner::Scanner<FuncTokenType> FuncScanner;