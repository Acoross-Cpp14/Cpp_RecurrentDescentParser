#pragma once

#include "ParserBase.h"
#include <list>

namespace AcorossParser
{
	enum ArgType
	{
		AT_NULL,
		AT_DIGIT,
		AT_STR,
		AT_FUNC
	};

	typedef void(*ARG_FUNC)();

	class ARG
	{
	public:
		ArgType type;
		union
		{
			int digit;
			ARG_FUNC func;
		} data;
		wstring str;
	};

	typedef std::list<ARG> ARGLIST;

	class Parser
	{
	public:
		typedef FuncScanner::TokenType TKType;
		typedef FuncScanner::Token Token;
	public:
		Parser()
		{
			;
		}

		bool Load(const wchar_t* const filename);

	private:
		bool procFile();
		bool procStmt();
		bool procArgs(ARGLIST& arglist);
		ARG procArg();
		bool procArgTail(ARGLIST& arglist);
		bool procFunc(ARG_FUNC& func);
		
		bool match(TKType expected);

		Token m_parsedTk;
		Token m_ret;

		wchar_t m_buf[2000];
		wchar_t* m_input;

		FuncScanner m_scanner;
	};
}