#ifndef __TAEK_XML_PARSER_H__
#define __TAEK_XML_PARSER_H__

#include "define.h"

BEGIN_UTILITY_NAMESPACE(XMLParser)

class XML
{
private:
	class Root
	{
	public:
		Root();
		~Root();

	private:

	};

	class Child
	{
	public:
		Child();
		~Child();

	private:

	};

public:
	XML(const str& path);
	~XML();

	bool TryRead(const str& path);

private:

};

END_UTILITY_NAMESPACE

#endif // !__TAEK_XML_PARSER_H__