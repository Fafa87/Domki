#pragma once
#include <Windows.h>
#include <vector>
#include <string>

using namespace std;

pair<string, string> split_parent(const string& path);

string join(const string & path, const string & subpath);

vector<string> get_all_names_within_folder(string folder, bool folder_mode = false, bool get_full_paths = false);

string next_folder_within_folder(string path);

string open_file_from_folder(string folder);

string open_folder_from_folder(string folder);