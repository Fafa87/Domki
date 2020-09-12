#include "serwery.h"

std::vector<std::string> split(const std::string &s, char delim) {
    std::vector<std::string> elems;
    split(s, delim, std::back_inserter(elems));
    return elems;
}


std::string join(const std::vector<std::string>& vec, const std::string& delim)
{
    std::stringstream res;
    copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(res, delim.c_str()));
    return res.str();
}
