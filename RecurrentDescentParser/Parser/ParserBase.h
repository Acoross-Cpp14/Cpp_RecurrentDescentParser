#pragma once

#include "..\Scanner\Scanner.h"
#include "..\Scanner\\FuncScanner.h"

namespace NParserBase
{
	using namespace AcorossScanner;

	class ParserBase
	{
	public:
		virtual ~ParserBase() {};
		virtual bool Load(const wchar_t* const filename) = 0;

	protected:		
		
	};
}
