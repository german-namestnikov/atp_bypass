#include "./harvester.h"

harvester::harvester()
{
}

system_profile harvester::get_system_profile()
{
	system_profile profile;

	profile.m_file_hashes = gather_file_hashes("C:\\*");
	profile.m_os_locale = gather_os_locale();
	profile.m_executable_path = gather_executable_path();

	return profile;
}

set<unsigned int> harvester::gather_file_hashes(string start_folder)
{
	set<unsigned int> files;

	wstring w_start_folder = wstring(start_folder.begin(), start_folder.end());

	WIN32_FIND_DATA file_data;
	HANDLE first_file = FindFirstFile(string_to_wstring(start_folder).c_str(), &file_data);

	while (FindNextFile(first_file, &file_data))
	{
		if (isalpha(file_data.cFileName[0]))
		{
			string file_name = wstring_to_string(file_data.cFileName);
			files.insert(hash(file_name.c_str()));

			if (file_data.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{
				string next_folder = start_folder.substr(0, start_folder.length() - 1) + file_name;
				next_folder = next_folder + "\\*";

				set<unsigned int> temp = gather_file_hashes(next_folder);
				files.insert(temp.begin(), temp.end());
			}
		}
	}
	return files;
}

string harvester::gather_os_locale()
{
	int max_locale_name_length = 80;

	LPSTR locale = new CHAR[max_locale_name_length];
	GetLocaleInfoA(LOCALE_SYSTEM_DEFAULT, LOCALE_SENGLISHLANGUAGENAME, locale, max_locale_name_length * sizeof(TCHAR));

	return string(locale);
}

string harvester::gather_executable_path()
{
	TCHAR szFileName[MAX_PATH + 1];
	GetModuleFileName(NULL, szFileName, MAX_PATH + 1);

	char path[MAX_PATH + 1];
	wcstombs(path, szFileName, MAX_PATH);

	string path_string = string(path);
	return path_string;
}

unsigned int harvester::hash(const char* s, unsigned int seed)
{
	unsigned int hash_value = seed;
	while (*s)
	{
		hash_value = hash_value * 101 + *s++;
	}
	return hash_value;
}

wstring harvester::string_to_wstring(string str)
{
	return wstring(str.begin(), str.end());
}

string harvester::wstring_to_string(wstring w_str)
{
	return string(w_str.begin(), w_str.end());
}
