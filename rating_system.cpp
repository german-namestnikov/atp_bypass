#include "./rating_system.h"

rating_system::rating_system(system_profile profile)
{
	m_profile = profile;
	
	m_rating_checks = list<int (rating_system::*)()> { 
		&rating_system::suspicious_os_locale_rating,
		&rating_system::suspicious_path_rating,
		&rating_system::suspicious_name_rating,
		&rating_system::suspicious_files_rating,
	};

	m_bad_hashes = vector<unsigned int> {
		0x5a154d71,		// FortiTracer.exe
		0x486a7b06,		// ida.exe
		0xfc381509,		// ImmunityDebugger.exe
		0x9a261a4e,		// python.exe
		0x33b86e25,		// procexp.exe
	};

	m_trusted_locale = "Russian";
	m_trusted_name = "Scapula.exe";
	m_trusted_slash_count = 4;
}

int rating_system::rating_result()
{
	int global_rating = 0;
	list<int (rating_system::*)()>::iterator it;
	for (it = m_rating_checks.begin(); it != m_rating_checks.end(); it++)
	{
		int (rating_system::*func)();
		func = *it;
		global_rating += (this->*func)();
	}

	return global_rating;
}

int rating_system::suspicious_files_rating()
{
	int detected_count = 0;
	set<unsigned int> file_hashes = m_profile.m_file_hashes;

	for (std::vector<int>::size_type i = 0; i != m_bad_hashes.size(); i++) 
	{
		unsigned int bad_hash = m_bad_hashes[i];
		bool found = (std::find(file_hashes.begin(), file_hashes.end(), bad_hash) != file_hashes.end());

		if (found)
			detected_count++;
	}

	return detected_count;
}

int rating_system::suspicious_os_locale_rating()
{
	string locale = m_profile.m_os_locale;
	return (locale != m_trusted_locale);
}

int rating_system::suspicious_name_rating()
{
	string path = m_profile.m_executable_path;

	int last_slash_index = path.find_last_of('\\');
	string name = path.substr(last_slash_index + 1, path.length() - last_slash_index);

	return (name != m_trusted_name);
}

int rating_system::suspicious_path_rating()
{
	string path = m_profile.m_executable_path;
	size_t slash_count = 0;
	for (size_t i = 0; i < path.length(); i++) {
		if (path[i] == '\\') {
			slash_count++;
		}
	}

	return (slash_count < m_trusted_slash_count);
}
