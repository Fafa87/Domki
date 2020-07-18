#include "os.h"

#include <experimental/filesystem>
#include<clocale>

pair<string, string> split_parent(const string& path)
{
	size_t found;
	found = path.find_last_of("/\\");
	return { path.substr(0, found), path.substr(found + 1) };
}

string join(const string& path, const string& subpath)
{
	if (path.size())
		return path + "\\" + subpath;
	else
		return subpath;
}

vector<string> get_all_names_within_folder(string folder, bool folder_mode, bool get_full_paths)
{
    vector<string> names;
    // std::setlocale(LC_ALL, ".1250"); nie dziala
    // std::locale::global(std::locale(".1250"));
    for (const auto & entry : std::experimental::filesystem::directory_iterator(folder))
    {
        auto path = entry.path();
        if (folder_mode ^ std::experimental::filesystem::is_directory(path))
            continue;
        if (!get_full_paths)
            path = path.filename();
        names.push_back(path.generic_string());
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
