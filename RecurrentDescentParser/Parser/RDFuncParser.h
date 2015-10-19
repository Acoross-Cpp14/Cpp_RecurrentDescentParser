#pragma once

#include <iostream>
//temp
#include <fstream>
#include <locale>
#include <codecvt>
#include <list>
#include <unordered_map>
//temp

#include "..\default.h"

#include "../Scanner/Scanner.h"

// handwritten parser - for func

// func -> STR ( args )
// args -> arg argtail | e
// argtail -> , args | e
// arg -> func | STR | DIGIT

using namespace AcorossScanner;

class CRDFuncParser
{
	///////////////////////////////
	// IMPL 
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
	SCANNERDEF_END;

	typedef Scanner<FuncScannerDefine> FuncScanner;

	using TokenType = FuncScanner::TokenType;
	using TokenStruct = FuncScanner::Token;

public:
	CRDFuncParser() = default;
	~CRDFuncParser() = default;

#define new_MATCH(eType) if (!parser.match(eType)) return false;
#define DERIVE(symbol) if (!parser.Derive(symbol)) return false;
#define EXPECT(eType) if (!parser.expect(eType)) return false;


	TokenStruct input_token;
	wchar_t* input;



	bool match(const TokenType eType)
	{
		if (input_token.type == eType)
		{
			input_token = FuncScanner::Scan(input);	// consume input_token

			return true;
		}

		return false;
	}

	bool expect(const TokenType eType)
	{
		return input_token.type == eType;
	}


	class IParserSymbolBase
	{
	public:
		IParserSymbolBase() = default;
		virtual ~IParserSymbolBase() = default;

		bool DoDerive(CRDFuncParser& parser)
		{
			if (Derive(parser))
			{
				OnDerived();
				return true;
			}
			return false;
		}

		virtual bool Derive(CRDFuncParser& parser) = 0;
		virtual void OnDerived() = 0;
	};

	enum ParserSymbolEnums
	{
		func,
		args,
		argtail,
		arg
	};


	bool Derive(const ParserSymbolEnums eSymbol)
	{
		switch (eSymbol)
		{
		case func:
		{
			CFunc sym;
			return sym.DoDerive(*this);
		}
		case args:
		{
			CArgs sym;
			return sym.DoDerive(*this);
		}
		case argtail:
		{
			CArgtail sym;
			return sym.DoDerive(*this);
		}
		case arg:
		{
			CArg sym;
			return sym.DoDerive(*this);
		}
		default:
			return false;
		}
	}

	class CFunc : public IParserSymbolBase
	{
	public: 
		DEF_CON(CFunc);

	public:
		bool Derive(CRDFuncParser& parser) final
		{
			// func -> funcname ( args )

			EXPECT(TokenType::STR);
			m_FuncName = parser.input_token.data;
			new_MATCH(TokenType::STR);
			new_MATCH(TokenType::LPAREN);
			DERIVE(args);
			new_MATCH(TokenType::RPAREN);

			return true;
		}
		void OnDerived() final
		{
			std::wcout << "func parsed: " << m_FuncName.c_str() << std::endl;
		}

	private:
		wstring m_FuncName;
	};
	
	class CArgs : public IParserSymbolBase
	{
	public:
		DEF_CON(CArgs);
	public:
		bool Derive(CRDFuncParser& parser) final
		{
			// args -> arg argtail | e
			if (parser.expect(TokenType::RPAREN))
			{
				// epsilon production
				return true;
			}
			else
			{
				DERIVE(arg);
				DERIVE(argtail);
				return true;
			}

			return false;
		}
		void OnDerived() final
		{

		}
	};

	class CArgtail : public IParserSymbolBase
	{
	public:
		DEF_CON(CArgtail);
		bool Derive(CRDFuncParser& parser) final
		{
			// argtail -> , args | e
			if (parser.expect(TokenType::RPAREN))
			{
				// epsilon production
				return true;
			}
			else
			{
				new_MATCH(TokenType::COMMA);
				DERIVE(args);
				return true;
			}
			return false;
		}
		void OnDerived() final
		{

		}
	};

	class CArg : public IParserSymbolBase
	{
	public:
		DEF_CON(CArg);
		bool Derive(CRDFuncParser& parser) final
		{
			// arg -> func | STR | DIGIT
			if (parser.expect(TokenType::DIGIT))
			{
				new_MATCH(TokenType::DIGIT);
				return true;
			}
			else if (parser.expect(TokenType::STR))
			{
				wchar_t* bak_input = parser.input;
				TokenStruct bak_input_TK = parser.input_token;

				if (parser.Derive(func))
				{
					return true;
				}
				else
				{
					parser.input = bak_input;
					parser.input_token = bak_input_TK;
					new_MATCH(TokenType::STR);
					return true;
				}
			}

			return false;
		}
		void OnDerived() final
		{

		}
	};

public:
	void RunTestProgram()
	{
		setlocale(LC_ALL, "");

		std::wifstream wis(L"monster.csv", std::ifstream::binary);
		if (false == wis.is_open())
			return;

		// apply BOM-sensitive UTF-16 facet
		wis.imbue(std::locale(wis.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::consume_header>));

		int nScriptLine = 0;
		std::wstring wline;

		wchar_t buf[2000];

		bool ret = true;
		while (std::getline(wis, wline))	// 한 줄 읽어들인다.
		{
			memset(buf, 0, sizeof(buf));
			wline._Copy_s(buf, 2000, wline.size(), 0);
			//wchar_t* input = buf;
			input = buf;

			input_token = FuncScanner::Scan(input);
			bool ret = Derive(func);

			std::cout << "ret=" << ret << std::endl;
		}
	}

private:
	NO_COPY(CRDFuncParser);
};