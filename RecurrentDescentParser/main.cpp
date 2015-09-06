#include <iostream>
#include <iterator>
#include <string>
#include <regex>

#include "RDParser.h"

int main()
{
	wchar_t* input = L"haha 12322 12dd id3d .83k  ( dfd dddd)";
	std::wstring buf = input;

	std::wcout << L"input: " << input << std::endl;

	Token ret;

	ret = Scan(input);
	std::wcout << TokenToName(ret.type) << L":" << ret.data << std::endl;

	while (ret.type != TK_NULL)
	{
		ret = Scan(input);
		std::wcout << TokenToName(ret.type) << L":" << ret.data << std::endl;
	}

	system("pause");
}