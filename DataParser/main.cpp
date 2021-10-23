#include "Ini.h"

int main(void)
{
	Taek::Utility::IniParser::Ini ini("test.ini");
	
	double data = 0;

	if (!ini.TryGetDouble("Test", "data4", data))
	{
		
	}

	int result = 0;

	if (!ini.TryGetInt32("Test", "data2", result))
	{

	}

	str name = "Name";
	str key = "test";

	ini.TryAddData(name, key, "1234");
	ini.TryChangeData(name, key, "5034.34934");

	ini.TrySave("test.ini");
	return 0;
}