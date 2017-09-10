#include "os.h"

pair<string, string> split_parent(const string& path)
{
	size_t found;
	found = path.find_last_of("/\\");
	return { path.substr(0, found), path.substr(found + 1) };
}

vector<string> get_all_names_within_folder(string folder, bool folder_mode)
{
	vector<string> names;
	string search_path = folder + "/*.*";
	WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile(search_path.c_str(), &fd);
	if (hFind != INVALID_HANDLE_VALUE) {
		do {
			// read all (real) files in current folder
			// , delete '!' read other 2 default folder . and ..
			string file_name = fd.cFileName;
			if (!folder_mode && !(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				|| folder_mode && (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && file_name != "." && file_name != "..")
			{
				names.push_back(file_name);
			}
		} while (::FindNextFile(hFind, &fd));
		::FindClose(hFind);
	}
	return names;
}

string next_folder_within_folder(string path)
{
	auto dir_name = split_parent(path);
	auto all_files = get_all_names_within_folder(dir_name.first, true);
	auto pos = find(all_files.begin(), all_files.end(), dir_name.second);
	pos++;
	if (pos == all_files.end())
		pos = all_files.begin();
	return *pos;
}
