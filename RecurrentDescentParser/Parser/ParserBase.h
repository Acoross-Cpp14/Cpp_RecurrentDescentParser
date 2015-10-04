#pragma once

namespace NParserBase
{
	class ParserBase
	{
	public:
		virtual ~ParserBase() {};
		virtual bool Load(const wchar_t* const filename) = 0;

	protected:		
		
	};
}
