#pragma once
#include <Windows.h>
#include <vector>
#include <string>
using namespace std;

pair<string, string> split_parent(const string& path);

vector<string> get_all_names_within_folder(string folder, bool folder_mode = false);

string next_folder_within_folder(string path);