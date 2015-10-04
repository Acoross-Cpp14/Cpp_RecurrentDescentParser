#pragma once

#include <iostream>
//temp
#include <fstream>
#include <locale>
#include <codecvt>
#include <list>
#include <unordered_map>
//temp

#include "../Scanner/Scanner.h"

using namespace AcorossScanner;

//template <typename ScanT>
class CParserBase
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
	typedef Scanner<FuncScannerDefine> ScanT;
	//using MyToken = ScanT::Token;
	using MyToken = FuncScanner::Token;
	//using MyTokenT = ScanT::TokenType;
	using MyTokenT = FuncScanner::TokenType;
	
	enum ParserSymbolType
	{
		PS_TOKEN,	// terminal
		PS_FUNC,
		PS_ARGS,
		PS_ARG,		// terminal
		PS_ARGTAIL,
		PS_EMPTY	// terminal
	};

	class PSData
	{
	public:
		PSData(ParserSymbolType eT)
			: eType(eT)
		{}

		PSData(ParserSymbolType eT,
			MyTokenT tkT)
			: eType(eT), tkType(tkT)
		{}

		ParserSymbolType eType;
		MyTokenT tkType;
		std::wstring str;
	};

	typedef list<PSData> SymbolList;
	typedef list<SymbolList> SymbolListList;

	// base type of PS
	class ParserSymbol
	{
	public:
		ParserSymbol() {}
		virtual ~ParserSymbol() {}

		virtual bool MatchAndConsume(MyToken& input_token, wchar_t*& input) const = 0;
	};

	class PSTerminal : public ParserSymbol
	{
	public:
		PSTerminal() {}
		virtual ~PSTerminal() {}

		virtual bool MatchAndConsume(MyToken& input_token, wchar_t*& input) const
		{
			if (match(input_token, input))
			{
				input_token = ScanT::Scan(input);	// consume
				return true;
			}
			return false;
		}

		virtual bool match(MyToken& input_token, wchar_t*& input) const = 0;
	};

	class PSToken : public PSTerminal
	{
	public:
		explicit PSToken(MyTokenT eTKtype)
			: eTokenType(eTKtype)
		{
		}
		virtual ~PSToken() {}

		virtual bool match(MyToken& input_token, wchar_t*& input) const
		{
			return eTokenType == input_token.type;
		}

	protected:
		MyTokenT eTokenType;
	};

	class PSNonTerminal : public ParserSymbol
	{
	public:
		PSNonTerminal() {}
		virtual ~PSNonTerminal() {}

		virtual bool MatchAndConsume(MyToken& input_token, wchar_t*& input) const
		{
			// or 조건은 한 함수로 표현하지 않는다.			
			for (auto it_symbol_list = m_SymbolLists.begin(); it_symbol_list != m_SymbolLists.end(); ++it_symbol_list)
			{
				auto symbol_list = *it_symbol_list;
				if (DeriveAndConsume(symbol_list, input_token, input))	// drive 에서 consume 도 된다.
				{
					OnFound();
					return true;
				}
			}

			return false;
		}
	protected:
		virtual void OnFound() const {}

		// 이 ParserSymbol 의 여러 or 로 연결된 production 중 하나를 try
		static bool DeriveAndConsume(const SymbolList& symbolList, MyToken& input_token, wchar_t*& input)
		{
			// back-up
			auto tmp_input_token = input_token;
			auto tmp_input = input;

			// or 조건은 한 함수로 표현하지 않는다.			
			for (auto it_symbol = symbolList.begin(); it_symbol != symbolList.end(); ++it_symbol)
			{
				auto symbol = *it_symbol;
				if (symbol.eType == ParserSymbolType::PS_TOKEN)
				{
					auto symbol_define = PSToken(symbol.tkType);
					if (!symbol_define.MatchAndConsume(tmp_input_token, tmp_input))
						return false;
				}
				else
				{
					auto p_symbol_define = ParserSymbolDefines::Inst()->GetDefine(symbol.eType);
					if (nullptr == p_symbol_define)
						return false;

					if (!p_symbol_define->MatchAndConsume(tmp_input_token, tmp_input))
						return false;
				}
			}

			//commit
			input_token = tmp_input_token;
			input = tmp_input;

			return true;
		}

		SymbolListList m_SymbolLists;
	};
	class PSString : public PSTerminal
	{
	public:
		PSString() {}
		virtual ~PSString() {}

		virtual bool match(MyToken& input_token, wchar_t*& input) const
		{
			return (m_string.compare(input_token.data) == 0);
		}

	protected:
		std::wstring m_string;
	};

	class PSEmpty : public PSTerminal
	{
	public:
		PSEmpty() {}
		virtual ~PSEmpty() {}

		virtual bool MatchAndConsume(MyToken& input_token, wchar_t*& input) const
		{
			return true;	// no consume
		}
		virtual bool match(MyToken& input_token, wchar_t*& input) const
		{
			return true;
		}
	};

	/////////////
	// non-terminals
	class PSFunc : public PSNonTerminal
	{
	public:
		PSFunc()
		{
			// func -> str ( args )
			SymbolList tmpList;
			tmpList.push_back(PSData(PS_TOKEN, FuncScanner::STR));
			tmpList.push_back(PSData(PS_TOKEN, FuncScanner::LPAREN));
			tmpList.push_back(PSData(PS_ARGS));
			tmpList.push_back(PSData(PS_TOKEN, FuncScanner::RPAREN));

			m_SymbolLists.push_back(tmpList);
		}
		virtual ~PSFunc() {}

	protected:
		virtual void OnFound() const
		{
			printf("function parsed\n");
		}
	};

	class PSArgs : public PSNonTerminal
	{
	public:
		PSArgs()
		{
			// args -> arg argtail | e
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_ARG));
				tmpList.push_back(PSData(PS_ARGTAIL));
				m_SymbolLists.push_back(tmpList);
			}
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_EMPTY));
				m_SymbolLists.push_back(tmpList);
			}
		}
		virtual ~PSArgs() {}
	};

	class PSArg : public PSNonTerminal
	{
	public:
		PSArg()
		{
			// arg -> func | str | digit
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_FUNC));
				m_SymbolLists.push_back(tmpList);
			}
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_TOKEN, FuncScanner::STR));
				m_SymbolLists.push_back(tmpList);
			}
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_TOKEN, FuncScanner::DIGIT));
				m_SymbolLists.push_back(tmpList);
			}
		}
		virtual ~PSArg() {}
	};

	class PSArgTail : public PSNonTerminal
	{
	public:
		PSArgTail()
		{
			// argtail -> , args | e
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_TOKEN, FuncScanner::COMMA));
				tmpList.push_back(PSData(PS_ARGS));
				m_SymbolLists.push_back(tmpList);
			}
			{
				SymbolList tmpList;
				tmpList.push_back(PSData(PS_EMPTY));
				m_SymbolLists.push_back(tmpList);
			}
		}
		virtual ~PSArgTail() {}
	};

	class ParserSymbolDefines
	{
	public:
		virtual ~ParserSymbolDefines() {}

		// singleton
		static ParserSymbolDefines* Inst()
		{
			if (nullptr == m_inst)
			{
				m_inst = new ParserSymbolDefines();
				m_inst->Init();
			}

			return m_inst;
		}

	public:
		// public function
		ParserSymbol* GetDefine(ParserSymbolType eType)
		{
			static PSToken tk(FuncScanner::TK_MAX);
			static PSFunc fc;
			static PSArgs args;
			static PSArg arg;
			static PSArgTail at;
			static PSEmpty em;

			switch (eType)
			{
			case PS_TOKEN:
				return &tk;
			case PS_FUNC:
				return &fc;
			case PS_ARGS:
				return &args;
			case PS_ARG:
				return &arg;
			case PS_ARGTAIL:
				return &at;
			case PS_EMPTY:
				return &em;
				break;
			default:
				return nullptr;
			}
		}

	private:
		// singleton
		ParserSymbolDefines()
		{}
		ParserSymbolDefines(const ParserSymbolDefines& rhs) {}
		const ParserSymbolDefines& operator=(const ParserSymbolDefines& rhs) {}

		static ParserSymbolDefines* m_inst;

		// parser symbol defines
		void Init()
		{
		}
	};

	public:
		void Run()
		{
			FuncScanner scanner;
			setlocale(LC_ALL, "");

			std::wifstream wis(L"monster.csv", std::ifstream::binary);
			if (false == wis.is_open())
				return;

			// apply BOM-sensitive UTF-16 facet
			wis.imbue(std::locale(wis.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

			int nScriptLine = 0;
			std::wstring wline;

			bool ret = true;
			if (std::getline(wis, wline))	// 한 줄 읽어들인다.
			{
				memset(m_buf, 0, sizeof(m_buf));
				wline._Copy_s(m_buf, 2000, wline.size(), 0);
				wchar_t* input = m_buf;
				input = m_buf;

				auto input_token = scanner.Scan(input);

				PSFunc StartSymbol;
				bool ret = StartSymbol.MatchAndConsume(input_token, input);

				std::cout << "ret=" << ret << std::endl;

			}
		}

	private:
		wchar_t m_buf[2000];
};
