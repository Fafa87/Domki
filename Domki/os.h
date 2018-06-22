#pragma once
#include <Windows.h>
#include <vector>
#include <string>

#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Widgets.hpp>
using namespace std;

namespace GUI
{
	static sfg::SFGUI sfgui;
	static sfg::Desktop pulpit;
}

pair<string, string> split_parent(const string& path);

vector<string> get_all_names_within_folder(string folder, bool folder_mode = false);

string next_folder_within_folder(string path);