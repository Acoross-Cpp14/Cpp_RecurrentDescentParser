#pragma once

// related headers
#include "Scanner.h"

using namespace AcorossScanner;

/*add_token(TK_NULL, nullptr), \
add_token(COMMENT, L"//(.*)"),	\*/

/////////////////////////////////////////////////////////
// code for scanner generation
SCANNERDEF_BEGIN(FuncScannerDefine)
{
#define token_list  \
	add_token(SEMICOLON, L";"), \
	add_token(COMMA, L","),	\
	add_token(LPAREN, L"\\("),	\
	add_token(RPAREN, L"\\)"),	\
	add_token(STR, L"[a-zA-Z]([a-zA-Z0-9_]*)"),	\
	add_token(ID, L"([0-9]+)([a-zA-z]+)"),	\
	add_token(DIGIT, L"(-)?[1-9]([0-9]*)"), \
	add_token(ANYWORD, L"([^ \t\n\\(\\),])+"), \
	add_token(TK_MAX, nullptr)

#include "./ScannerImplMacro.inc"

#undef token_list
}
SCANNERDEF_END

typedef Scanner<FuncScannerDefine> FuncScanner;