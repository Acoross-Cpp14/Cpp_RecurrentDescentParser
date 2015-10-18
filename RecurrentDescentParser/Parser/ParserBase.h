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

/////////////////////////////////////////
//general
template <typename ScanT, typename PsTy>
class PSData
{
	typedef PsTy MySymT;
	typedef typename ScanT::TokenType MyTokenT;
public:
	PSData(PsTy eT)
		: eType(eT)
	{}

	PSData(PsTy eT,
		MyTokenT tkT)
		: eType(eT), tkType(tkT)
	{}

	PsTy eType;
	MyTokenT tkType;
	std::wstring str;
};

template <typename ScanT, typename PsTy>
class CProductionEngine
{
public:
	typedef PsTy MyPsType;
	typedef PSData<ScanT, PsTy> MyPSData;

	typedef list<MyPSData> SymbolList;
	typedef list<SymbolList> SymbolListList;

	typedef typename ScanT::TokenType MyTokenT;
	typedef typename ScanT::Token MyToken;

public:
	CProductionEngine() {}
	~CProductionEngine() {}

	static bool MatchAndConsume(const SymbolListList& listlist, MyToken& input_token, wchar_t*& input)
	{
		// or 조건은 한 함수로 표현하지 않는다.			
		for (auto it_symbol_list = listlist.begin(); it_symbol_list != listlist.end(); ++it_symbol_list)
		{
			auto symbol_list = *it_symbol_list;
			if (DeriveAndConsume(symbol_list, input_token, input))	// drive 에서 consume 도 된다.
			{
				//OnFound();
				return true;
			}
		}

		return false;
	}
	static bool MatchAndConsume(MyTokenT eTokenType, MyToken& input_token, wchar_t*& input)
	{
		if (match(eTokenType, input_token, input))
		{
			input_token = ScanT::Scan(input);	// consume
			return true;
		}
		return false;
	}
protected:
	static bool match(MyTokenT eTokenType, MyToken& input_token, wchar_t*& input)
	{
		return eTokenType == input_token.type;
	}

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
			if (symbol.eType == MyPsType::PS_EMPTY)
				return true;

			if (symbol.eType == MyPsType::PS_TOKEN)
			{
				if (!MatchAndConsume(symbol.tkType, tmp_input_token, tmp_input))
					return false;
			}

			auto new_ListList = ParserSymbolDefines<ScanT, PsTy>::Inst()->GetDefine2(symbol.eType);
			if (!MatchAndConsume(new_ListList, tmp_input_token, tmp_input))
				return false;
		}

		//commit
		input_token = tmp_input_token;
		input = tmp_input;

		return true;
	}
};

template <typename ScanT, typename PsTy>
class ParserSymbolDefines
{
public:
	typedef ScanT MyScanner;
	typedef PSData<ScanT, PsTy> MyPSData;
	typedef list<MyPSData> SymbolList;
	typedef list<SymbolList> SymbolListList;
	typedef std::unordered_map<PsTy, SymbolListList> MyMap;
	typedef PsTy MySymT;

	//typedef std::function<SymbolListList(MySymT)> MyDefineFunc;

public:
	virtual ~ParserSymbolDefines() {}

	// singleton
	static ParserSymbolDefines* Inst()
	{
		if (nullptr == m_inst)
		{
			m_inst = new ParserSymbolDefines();
			//m_inst->Init();
		}

		return m_inst;
	}

public:
	// public function
	/*void SetDefine(MyDefineFunc& func)
	{
		m_DefineFunc = func;
	}
	SymbolListList& GetDefine2(PsTy eType)
	{
		return m_DefineFunc();
	}*/

	void SetDefine(MyMap& map)
	{
		m_mapProductions = map;
	}

	SymbolListList& GetDefine2(PsTy eType)
	{
		auto it = m_mapProductions.find(eType);
		if (it != m_mapProductions.end())
		{
			return it->second;
		}

		static SymbolListList nullList;
		return nullList;
	}

private:
	// singleton
	ParserSymbolDefines()
	{}
	ParserSymbolDefines(const ParserSymbolDefines& rhs) {}
	const ParserSymbolDefines& operator=(const ParserSymbolDefines& rhs) {}

	MyMap m_mapProductions;
	//MyDefineFunc m_DefineFunc;

	static ParserSymbolDefines* m_inst;
};

template <typename ScanT, typename PsTy>
ParserSymbolDefines<ScanT, PsTy>* ParserSymbolDefines<ScanT, PsTy>::m_inst = nullptr;







class CFunctionParser
{
public:
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
	SCANNERDEF_END

	typedef Scanner<FuncScannerDefine> FuncScanner;

	enum ParserSymbolType
	{
		PS_TOKEN,	// terminal
		PS_FUNC,
		PS_ARGS,
		PS_ARG,		// terminal
		PS_ARGTAIL,
		PS_EMPTY	// terminal
	};
	///////////////////////////////
public:
	/*typedef PSData<FuncScanner, ParserSymbolType> MyPSData;
	typedef CProductionEngine<FuncScanner, ParserSymbolType> MyProductionEngine;
	typedef ParserSymbolDefines<FuncScanner, ParserSymbolType> MySymbolDefines;*/

	CFunctionParser()
	{}

	void Run()
	{
		FuncScanner scanner;

		//MySymbolDefines::Inst()->SetDefine(
		//	[](ParserSymbolType eType)->MySymbolDefines::SymbolListList
		//{
		//	MySymbolDefines::SymbolListList ret;
		//	typedef MySymbolDefines::SymbolList SymbolList;
		//	typedef MySymbolDefines::MyScanner ScanT;

		//	switch (eType)
		//	{
		//	case PS_FUNC:
		//	{
		//		// func -> str ( args )
		//		SymbolList tmpList;
		//		tmpList.push_back(MyPSData(PS_TOKEN, ScanT::STR));
		//		tmpList.push_back(MyPSData(PS_TOKEN, ScanT::LPAREN));
		//		tmpList.push_back(MyPSData(PS_ARGS));
		//		tmpList.push_back(MyPSData(PS_TOKEN, ScanT::RPAREN));
		//		ret.push_back(tmpList);
		//	}
		//		break;
		//	case PS_ARGS:
		//	{
		//		// args -> arg argtail | e
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_ARG));
		//			tmpList.push_back(MyPSData(PS_ARGTAIL));
		//			ret.push_back(tmpList);
		//		}
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_EMPTY));
		//			ret.push_back(tmpList);
		//		}
		//	}
		//		break;
		//	case PS_ARGTAIL:
		//	{
		//		// argtail -> , args | e
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_TOKEN, ScanT::COMMA));
		//			tmpList.push_back(MyPSData(PS_ARGS));
		//			ret.push_back(tmpList);
		//		}
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_EMPTY));
		//			ret.push_back(tmpList);
		//		}
		//	}
		//		break;
		//	case PS_ARG:
		//	{
		//		// arg -> func | str | digit
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_FUNC));
		//			ret.push_back(tmpList);
		//		}
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_TOKEN, ScanT::STR));
		//			ret.push_back(tmpList);
		//		}
		//		{
		//			SymbolList tmpList;
		//			tmpList.push_back(MyPSData(PS_TOKEN, ScanT::DIGIT));
		//			ret.push_back(tmpList);
		//		}
		//	}
		//		break;
		//	default:
		//		// tokens, strings, empty
		//		break;
		//	}

		//	return ret;
		//}
		//);
		
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
		if (std::getline(wis, wline))	// 한 줄 읽어들인다.
		{
			memset(buf, 0, sizeof(buf));
			wline._Copy_s(buf, 2000, wline.size(), 0);
			wchar_t* input = buf;
			input = buf;

			auto input_token = scanner.Scan(input);
			

			auto StartSymbolProduction = 
				ParserSymbolDefines<FuncScanner, ParserSymbolType>::Inst()->GetDefine2(PS_FUNC);
			
			bool ret = 
				CProductionEngine<FuncScanner, ParserSymbolType>::MatchAndConsume(StartSymbolProduction, input_token, input);



			std::cout << "ret=" << ret << std::endl;
		}
	}

private:
	NO_COPY(CFunctionParser);
};


