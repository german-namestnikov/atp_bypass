#pragma once
#include <set>
#include <string>

using namespace std;

struct system_profile
{
	set<unsigned int> m_file_hashes;
	string m_os_locale;
	string m_executable_path;
};