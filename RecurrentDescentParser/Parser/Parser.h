#pragma once

// STL headers
#include <list>
#include <functional>

// my project's headers
#include "../Scanner/Scanner.h"
#include "../Scanner/FuncScanner.h"

#define MATCH(x) if (!match(x)) return false

using namespace AcorossScanner;

namespace AcorossParser
{
	SCANNERDEF_BEGIN(FuncScannerDefine)
	{
		/*add_token(TK_NULL, nullptr), \
		*/
#define token_list  \
	add_token(COMMENT, L"//(.*)"),	\
	add_token(SEMICOLON, L";"), \
	add_token(COMMA, L","),	\
	add_token(LPAREN, L"\\("),	\
	add_token(RPAREN, L"\\)"),	\
	add_token(STR, L"[a-zA-Z]([a-zA-Z0-9_]*)"),	\
	add_token(ID, L"([0-9]+)([a-zA-z]+)"),	\
	add_token(DIGIT, L"(-)?[1-9]([0-9]*)"), \
	add_token(ANYWORD, L"([^ \t\n\\(\\),])+"), \
	add_token(TK_MAX, nullptr)

#include "../Scanner/ScannerImplMacro.inc"

#undef token_list
	}
	SCANNERDEF_END

	typedef Scanner<FuncScannerDefine> FuncScanner;




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

	///////////////////////////////////////////
	// Tran
	class Tran
	{
		typedef std::function<void()> TranFunc;
	public:
		Tran(const TranFunc& f)
			: m_func(f)
		{
		}
		~Tran()
		{
			m_func();
		}
		void Commit()
		{
			m_func = []() {};
		}

	private:
		TranFunc m_func;
	};

	///////////////////////////////////////////
	// Parser
	class Parser
	{
	public:
		typedef FuncScanner::TokenType TKType;
		typedef FuncScanner::Token Token;

		class TranPr
		{
		public:
			TranPr(Parser* p)
				: m_Tr([_this = p, ret = p->m_ret, input = p->m_input]()
						{
							_this->m_ret = ret;
							_this->m_input = input;
						})
			{}

			~TranPr()
			{
			}

			void Commit()
			{
				m_Tr.Commit();
			}

		private:
			Tran m_Tr;
		};

		enum ExprType
		{
			STMT,
			FUNC
		};
		enum ExprTokenType
		{
			TK,
			EXPR
		};
		struct ExprToken
		{
			ExprTokenType type;
			union{
				int raw;
				TKType token;
				ExprType expr;
			}data;
		};

	public:
		Parser()
		{
			;
		}

		// run this parser to load "filename"
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