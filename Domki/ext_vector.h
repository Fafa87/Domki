#pragma once

#include <vector>

template<typename T>
void remove_item(std::vector<T>& vector, T& elem)
{
    vector.erase(std::remove(vector.begin(), vector.end(), elem), vector.end());
}

