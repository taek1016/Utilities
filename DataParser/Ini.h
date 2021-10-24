#ifndef __TAEK_INI_PARSER_H__
#define __TAEK_INI_PARSER_H__

#include <fstream>
#include <list>
#include <map>
#include <vector>

#include "define.h"

BEGIN_UTILITY_NAMESPACE(IniParser)

class Ini
{
private:
	typedef std::map<str, str> KeyValueMap;

	// Nested Class
	class Section
	{
	public:
		Section(const str& name);
		~Section();

		const str& GetName(void) const;
		bool AddData(str key, str data);
		bool ChangeData(str key, str data);
		bool Get(str key, str& result);
		void WriteData(std::fstream& fileStream);

	private:
		str m_Name;
		KeyValueMap m_Map;
	};

	typedef std::list<Section*> SectionList;

public:
	Ini(const str path);
	~Ini();

	bool TryAddData(str sectionName, str key, str data);
	bool TryChangeData(str sectionName, str key, str data);

	bool TryGetString(str sectionName, str key, str& data);
	bool TryGetInt32(str sectionName, str key, int32_t& data);
	bool TryGetInt64(str sectionName, str key, int64_t& data);
	bool TryGetFloat(str sectionName, str key, float_t& data);
	bool TryGetDouble(str sectionName, str key, double_t& data);

	bool TrySave(const str path);
	bool TryRead(const str path);

	static str TrimStr(const str& from);

private:
	const bool getSection(const str& name, Ini::Section* outSection) const;

	bool isDigit(bool (*determine)(char), const str& data, const size_t startPos, const size_t endPos);

	static bool isInteger(char character);
	static bool isFloating(char character);

private:
	SectionList m_Sections;
};

END_UTILITY_NAMESPACE

#endif // !__TAEK_INI_PARSER_H__