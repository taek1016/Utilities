#include "Ini.h"

USING_TAEK_UTILITY_NAMESPACE(IniParser);
USING_NAMESPACE(std);

Ini::Section::Section(const str& name)
	: m_Name(name)
{
}

Ini::Section::~Section()
{
	m_Map.clear();
}
const str& Ini::Section::GetName(void) const
{
	return m_Name;
}

bool Ini::Section::AddData(str key, str data)
{
	auto findIterator = m_Map.find(key);

	if (m_Map.end() != findIterator)
	{
		return false;
	}

	m_Map.insert(make_pair(key, data));

	return true;
}

bool Ini::Section::ChangeData(str key, str data)
{
	auto findIterator = m_Map.find(key);

	if (m_Map.end() == findIterator)
	{
		return false;
	}

	findIterator->second = data;

	return true;
}

bool Ini::Section::Get(str key, str& result)
{
	auto findIterator = m_Map.find(key);

	if (m_Map.end() == findIterator)
	{
		return false;
	}

	result = m_Map[key];

	return true;
}

void Ini::Section::WriteData(fstream& fileStream)
{
	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	for (auto iter : m_Map)
	{
		memset(buffer, 0, sizeof(char) * BUFFER_SIZE);

		str key = TrimStr(iter.first);
		str data = TrimStr(iter.second);

		snprintf(buffer, BUFFER_SIZE, "%s=%s\n", key.c_str(), data.c_str());

		fileStream.write(buffer, strlen(buffer));
	}
}

Ini::Ini(const str path)
{
	TryRead(path);
}

Ini::~Ini()
{
	for (auto iter = m_Sections.begin(); iter != m_Sections.end(); ++iter)
	{
		delete (*iter);
		(*iter) = nullptr;
	}
	m_Sections.clear();
}

bool Ini::TryAddData(str sectionName, str key, str data)
{
	Section* section = nullptr;
	if (!getSection(sectionName, section))
	{
		section = new Section(sectionName);

		m_Sections.push_back(section);
	}

	if (!section->AddData(key, data))
	{
		return false;
	}

	return true;
}

bool Ini::TryChangeData(str sectionName, str key, str data)
{
	Section* section = nullptr;
	if (!getSection(sectionName, section))
	{
		return false;
	}

	if (section->ChangeData(key, data))
	{
		return false;
	}

	return true;
}

bool Ini::TryGetString(str sectionName, str key, str& data)
{
	Section* section = nullptr;
	if (!getSection(sectionName, section))
	{
		return false;
	}

	if (!section->Get(key, data))
	{
		return false;
	}

	return true;
}

bool Ini::TryGetInt32(str sectionName, str key, int32_t& data)
{
	str strData;
	if (!TryGetString(sectionName, key, strData))
	{
		return false;
	}

	if (!isDigit(isInteger, strData, 0, strData.length()))
	{
		return false;
	}

	data = (int32_t)stoi(strData);

	return true;
}

bool Ini::TryGetInt64(str sectionName, str key, int64_t& data)
{
	str strData;
	if (!TryGetString(sectionName, key, strData))
	{
		return false;
	}

	if (!isDigit(isInteger, strData, 0, strData.length()))
	{
		return false;
	}

	data = (int64_t)stoi(strData);

	return true;
}

bool Ini::TryGetFloat(str sectionName, str key, float_t& data)
{
	str strData;
	if (!TryGetString(sectionName, key, strData))
	{
		return false;
	}

	if (!isDigit(isFloating, strData, 0, strData.length()))
	{
		return false;
	}

	data = (float_t)stof(strData);

	return true;
}

bool Ini::TryGetDouble(str sectionName, str key, double_t& data)
{
	str strData;
	if (!TryGetString(sectionName, key, strData))
	{
		return false;
	}

	if (!isDigit(isFloating, strData, 0, strData.length()))
	{
		return false;
	}

	data = (float_t)stod(strData);

	return true;
}

bool Ini::TrySave(const str path)
{
	fstream fileStream;
	fileStream.open(path, ios_base::out);

	if (!fileStream.is_open())
	{
		return false;
	}

	const int BUFFER_SIZE = 256;
	char buffer[BUFFER_SIZE] = "";

	for (auto iter = m_Sections.begin(); iter != m_Sections.end(); ++iter)
	{
		memset(buffer, 0, sizeof(char) * BUFFER_SIZE);

		snprintf(buffer, BUFFER_SIZE, "[%s]\n", (*iter)->GetName().c_str());

		fileStream.write(buffer, strlen(buffer));

		(*iter)->WriteData(fileStream);
	}

	return true;
}

bool Ini::TryRead(const str path)
{
	fstream fileStream;

	fileStream.open(path.c_str(), ios_base::in);

	if (!fileStream.is_open())
	{
		return false;
	}

	const int BUFFER_SIZE = 1024;
	char buffer[BUFFER_SIZE];

	Section* section = nullptr;

	while (!fileStream.eof())
	{
		fileStream.getline(buffer, BUFFER_SIZE);

		str content = buffer;

		if (content.empty())
		{
			continue;
		}

		size_t parenthesisStart = content.find('[');

		if (std::string::npos != parenthesisStart)
		{
			size_t parenthesisEnd = content.find_last_of(']');
			str name = content.substr(parenthesisStart + 1, parenthesisEnd - 1);

			if (!getSection(name, section))
			{
				section = new Section(name);

				m_Sections.push_back(section);
			}
		}
		else
		{
			size_t equalPos = content.find('=');
			
			if (str::npos == equalPos)
			{
				continue;
			}

			str key = content.substr(0, equalPos);
			key = TrimStr(key);

			str data = content.substr(equalPos + 1);
			data = TrimStr(data);

			section->AddData(key, data);
		}
	}

	return true;
}

str Ini::TrimStr(const str& from)
{
	const str TRIM_VALUES = " \t\v\r\n";

	size_t leftSide = from.find_first_not_of(TRIM_VALUES.c_str());
	size_t rightSide = from.find_last_not_of(TRIM_VALUES.c_str()) + 1;

	if (str::npos == leftSide)
	{
		leftSide = 0;
	}

	if (str::npos == rightSide)
	{
		rightSide = from.length() - 1;
	}

	return from.substr(leftSide, rightSide);
}

const bool Ini::getSection(const str& name, Ini::Section* outSection) const
{
	for (auto iter : m_Sections)
	{
		if (name == iter->GetName())
		{
			outSection = iter;
			return true;
		}
	}

	return false;
}

bool Ini::isDigit(bool (*determine)(const char), const str& data, const size_t startPos, const size_t endPos)
{
	for (size_t i = startPos; i < endPos; ++i)
	{
		if (!determine(data[i]))
		{
			return false;
		}
	}

	return true;
}

bool Ini::isInteger(char character)
{
	if (!isdigit(character))
	{
		return false;
	}

	return true;
}

bool Ini::isFloating(char character)
{
	if (!isdigit(character) && character != '.')
	{
		return false;
	}

	return true;
}