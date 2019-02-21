#pragma once

#include <string>

static bool ends_with(const std::string& str, const std::string& suffix)
{
	return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool starts_with(const std::string& str, const std::string& prefix)
{
	return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static void replace_ext(string& s, const string& new_ext) {

	string::size_type i = s.rfind('.', s.length());

	if (i != string::npos) {
		s.replace(i + 1, new_ext.length(), new_ext);
	}
}