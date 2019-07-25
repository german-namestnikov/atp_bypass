#pragma once

#include <list>
#include <vector>

#include "./system_profile.h"
#include "./harvester.h"

using namespace std;

class rating_system
{
public:
	rating_system(system_profile profile);
	int rating_result();

private:
	system_profile m_profile;

	list<int (rating_system::*)()> m_rating_checks;
	vector<unsigned int> m_bad_hashes;  
	string m_trusted_locale;
	string m_trusted_name;
	int m_trusted_slash_count;

	int suspicious_files_rating();
	int suspicious_os_locale_rating();
	int suspicious_name_rating();
	int suspicious_path_rating();
};