// ScannerDefine 을 정의하기 위해 사용하는 macro
// 구현 파일에서는 #define token_list 매크로를 정의한다.
// 구현하려는 ScannerDefine struct 의 내부에 이 파일을 include 한다.
	// #include "..\ScannerImplMacro.inc"

#define add_token(type, rex) type
	enum TokenType
	{
		token_list
	};
#undef add_token
	typedef TokenDefine<TokenType> _TkDef;

	// 사용 가능한 Token의 regex 를 정의.
#define add_token(type, rex) {type, rex}
	static const _TkDef& GetTokenDefine(TokenType type) 
	{
		static const _TkDef TokenDefines[] = 
		{ 
			token_list
		};
		return TokenDefines[type];
	}
#undef add_token

#define add_token(type, rex) L#type
	static const wchar_t* TokenToName(TokenType type) {
		static const wchar_t* nameMap[] = 
		{ 
			token_list 
		};
		return nameMap[type];
	}
#undef add_token