#include "Parser.h"

#include <string>
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>

#include "..\Scanner\Scanner.h"
#include "..\Scanner\FuncScanner.h"

namespace AcorossParser
{

#define MATCHTRUE(x) if (!match(x))return false; else
#define MATCH(x) if (!match(x)) return false

	using namespace AcorossScanner;


	int ParseInt(FuncScanner::Token& tk)
	{
		return std::wcstol(tk.data.c_str(), nullptr, 10);
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
		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;

		if (m_ret.type == TKType::SEMICOLON)
		{
			func = nullptr;
			return true;
		}

		ARGLIST arglist;
		wstring funcName;

		if (match(TKType::STR))
		{
			funcName = m_parsedTk.data;

			if (match(TKType::LPAREN))
				if (procArgs(arglist))
				{
					func = TempFunc;
					return true;
				}
		}

		m_ret = tmpTk;
		m_input = tmpInput;
		return false;
	}
	
	bool Parser::procStmt()
	{
		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;
				
		if (match(TKType::COMMENT))
			return true;

		MATCHTRUE(TKType::DIGIT)
		{
			wprintf_s(L"%d,", ParseInt(m_parsedTk));
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::STR)
		{
			wprintf_s(L"%s,", m_parsedTk.data.c_str());
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::ANYWORD)
		{
			wprintf_s(L"%s,", m_parsedTk.data.c_str());
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::DIGIT)
		{
			wprintf_s(L"%d,", ParseInt(m_parsedTk));
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::DIGIT)
		{
			wprintf_s(L"%d,", ParseInt(m_parsedTk));
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::DIGIT)
		{
			wprintf_s(L"%d,", ParseInt(m_parsedTk));
		}
		MATCH(TKType::COMMA);
		MATCHTRUE(TKType::DIGIT)
		{
			wprintf_s(L"%d,", ParseInt(m_parsedTk));
		}
		MATCH(TKType::COMMA);
		
		ARG_FUNC tmpFunc;
		if (procFunc(tmpFunc))
		{
			if (tmpFunc)
			{
				(*tmpFunc)();
			}
			
			if (match(TKType::SEMICOLON))
			{
				wprintf_s(L";");
				return true;
			}
		}

		m_ret = tmpTk;
		m_input = tmpInput;
		return false;

	}
	
	bool Parser::procFile()
	{
		FuncScanner::Token tmpTk = m_ret;
		wchar_t* tmpInput = m_input;

		if (procStmt())
			if (match(TKType::TK_NULL))
				return true;

		m_ret = tmpTk;
		m_input = tmpInput;
		return false;
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
		
		//m_scanner = FuncScanner(_TokenDefines);

		bool ret = true;
		while (std::getline(wis, wline))	// 한 줄 읽어들인다.
		{	
			memset(m_buf, 0, sizeof(m_buf));
			wline._Copy_s(m_buf, 2000, wline.size(), 0);
			m_input = m_buf;

			m_ret = m_scanner.Scan(m_input);
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

	bool Parser::match(FuncScanner::TKType expected)
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
}
