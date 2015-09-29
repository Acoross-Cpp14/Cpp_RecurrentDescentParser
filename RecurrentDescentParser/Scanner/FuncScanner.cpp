#include "FuncScanner.h"

// ��� ������ Token�� regex �� ����.
extern AcorossScanner::TokenDefine<FuncTokenType> _TokenDefines[] = {
#define add_token(type, rex) {FuncTokenType::type, rex}
	func_token_list
#undef add_token
};

const wchar_t* TokenToName(FuncTokenType type)
{
#define add_token(type, rex) L#type
	const wchar_t* nameMap[] = {
		func_token_list
	};
	return nameMap[type];
#undef add_token
}