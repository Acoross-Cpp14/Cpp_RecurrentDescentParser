// related headers
#include "Parser.h"

// STL headers
#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <functional>

namespace AcorossParser
{
	using namespace AcorossScanner;

	int ParseInt(FuncScanner::Token& tk)
	{
		return std::wcstol(tk.data.c_str(), nullptr, 10);
	}

	bool Parser::match(Parser::TKType expected)
	{
		if (m_ret.type == expected)
		{
			m_parsedTk = m_ret;
			m_ret = m_scanner.Scan(m_input);
			return true;
		}
		else
		{
			return false;
		}
	}

	ARG Parser::procArg()
	{
		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;

		ARG arg;
		arg.type = AT_NULL;

		if (match(TKType::DIGIT))
		{
			arg.type = AT_DIGIT;
			arg.data.digit = ParseInt(m_parsedTk);
			return arg;
		}
		else if (procFunc(arg.data.func))
		{
			arg.type = AT_FUNC;
			 
			return arg;
		}
		else if (match(TKType::STR))
		{
			arg.type = AT_STR;
			arg.str = m_parsedTk.data;
			return arg;
		}
		
		m_ret = tmpTk;
		m_input = tmpInput;
		return arg;
	}

	bool Parser::procArgTail(ARGLIST& arglist)
	{
		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;

		if (match(TKType::RPAREN))
			return true;

		ARGLIST arglist2;
		if (match(TKType::COMMA))
		{
			ARG arg = procArg();
			if (arg.type != AT_NULL)
			{
				arglist2.push_back(arg);

				if (procArgTail(arglist2))
				{
					arglist.insert(arglist.end(), arglist2.begin(), arglist2.end());
					return true;
				}
			}
		}

		m_ret = tmpTk;
		m_input = tmpInput;
		return false;
	}

	bool Parser::procArgs(ARGLIST& arglist)
	{
		TranPr tr(this);

		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;

		ARGLIST arglist2;

		if (match(TKType::RPAREN))
			return true;

		ARG arg = procArg();
		if (arg.type != AT_NULL)
		{
			arglist2.push_back(arg);
			if (procArgTail(arglist2))
			{
				arglist.insert(arglist.end(), arglist2.begin(), arglist2.end());
				return true;
			}
		}

		m_ret = tmpTk;
		m_input = tmpInput;
		return false;
	}

	void TempFunc()
	{
		wprintf_s(L"func parsed");
	}

	bool Parser::procFunc(ARG_FUNC& func)
	{
		TranPr tr(this);

		if (m_ret.type == TKType::SEMICOLON)
		{
			func = nullptr;
			tr.Commit();
			return true;
		}

		ARGLIST arglist;
		wstring funcName;

		MATCH(TKType::STR);
		funcName = m_parsedTk.data;
		MATCH(TKType::LPAREN);
		if (false == procArgs(arglist))
		{
			return false;
		}

		func = TempFunc;
		tr.Commit();
		return true;
	}
		
	bool Parser::procStmt()
	{
		const std::vector<ExprToken> grammar =
		{
			{ TK, TKType::DIGIT },
			{ TK, TKType::COMMA },
			{ TK, TKType::STR },
			{ TK, TKType::COMMA },
			{ TK, TKType::ANYWORD },
			{ TK, TKType::COMMA },
			{ EXPR, ExprType::FUNC },
			{ TK, TKType::SEMICOLON },
		};

		// return 될 때 rollback 됨.
			// 명시적으로 commit 호출하면 rollback 안 됨.
		TranPr tr(this);

		for (auto it = grammar.begin(); it != grammar.end(); ++it)
		{
			switch (it->type)
			{
			case ExprTokenType::TK:
				MATCH(it->data.token);
				break;
			case ExprTokenType::EXPR:
				ARG_FUNC tmpFunc;
				if (false == procFunc(tmpFunc))
				{
					return false;
				}
				(*tmpFunc)();
				break;
			default:
				return false;
				break;
			}
		}

		tr.Commit();
		return true;
	}
	
	bool Parser::procFile()
	{
		TranPr tr(this);

		if (false == procStmt())
			return false;

		MATCH(TKType::TK_MAX);

		tr.Commit();
		return true;
	}



	bool Parser::Load(const wchar_t* const filename)
	{
		setlocale(LC_ALL, "");
		
		std::wifstream wis(filename, std::ifstream::binary);
		if (false == wis.is_open())
		{
			return false;
		}
		// apply BOM-sensitive UTF-16 facet
		wis.imbue(std::locale(wis.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

		int nScriptLine = 0;
		std::wstring wline;
		
		bool ret = true;
		while (std::getline(wis, wline))	// 한 줄 읽어들인다.
		{	
			memset(m_buf, 0, sizeof(m_buf));
			wline._Copy_s(m_buf, 2000, wline.size(), 0);
			m_input = m_buf;

			m_ret = m_scanner.Scan(m_input);
			if (m_ret.type == TKType::COMMENT)
			{
				m_ret = m_scanner.Scan(m_input);
			}

			ret = procStmt();

			if (ret)
			{
				std::cout << "true" << std::endl;
			}
			else
			{
				std::cout << "false" << std::endl;
				break;
			}
		}

		return true;
	}
}
