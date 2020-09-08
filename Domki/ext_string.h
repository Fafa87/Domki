#pragma once 
#include <cstdio>
#include <cstdarg>
#include <string>
#include <vector>

static bool ends_with(const std::string& str, const std::string& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}

static bool starts_with(const std::string& str, const std::string& prefix)
{
    return str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix);
}

static void replace_ext(std::string& s, const std::string& new_ext) {

    std::string::size_type i = s.rfind('.', s.length());

    if (i != std::string::npos) {
        s.replace(i + 1, new_ext.length(), new_ext);
    }
}

static std::string string_format(const char *fmt, ...)
{
    // written by Toby Speight
    // from: https://codereview.stackexchange.com/questions/187183/create-a-c-string-using-printf-style-formatting
    char buf[256];

    va_list args;
    va_start(args, fmt);
    const auto r = std::vsnprintf(buf, sizeof buf, fmt, args);
    va_end(args);

    if (r < 0)
        // conversion failed
        return {};

    const size_t len = r;
    if (len < sizeof buf)
        // we fit in the buffer
        return { buf, len };

    auto vbuf = std::unique_ptr<char[]>(new char[len + 1]);
    va_start(args, fmt);
    std::vsnprintf(vbuf.get(), len + 1, fmt, args);
    va_end(args);

    return { vbuf.get(), len };
}


template<typename Out>
void split(const std::string &s, char delim, Out result) {
    std::stringstream ss;
    ss.str(s);
    std::string item;
    while (std::getline(ss, item, delim)) {
        *(result++) = item;
    }
}

std::vector<std::string> split(const std::string &s, char delim);
