#pragma once

#include <set>
#include <string>
#include <windows.h>

#include "./system_profile.h"

using namespace std;

class harvester
{
public:
	harvester();

	system_profile get_system_profile();

private:
	set<unsigned int> gather_file_hashes(string start_folder);
	string gather_os_locale();
	string gather_executable_path();

	unsigned int hash(const char* s, unsigned int seed = 0);

	wstring string_to_wstring(string str);
	string wstring_to_string(wstring str);
};