#ifndef VECTOR_FORMAT_HPP
#define VECTOR_FORMAT_HPP

#include <vector>
#include <string>
#include "strings_format.hpp"

// template<typename T>
// std::string pointers_to_string(const std::vector<T>& v);

// template<typename T>
// std::string objects_to_string(const std::vector<T>& v);

// std::string to_string(const std::vector<std::string>& strs);


template <typename T> std::string pointers_to_string(const std::vector<T>& v) {
    std::vector<std::string> strs;
    for (auto e: v) {
        strs.push_back(e->to_string());
    }
    return to_string(strs);
}

template <typename T> std::string objects_to_string(const std::vector<T>& v) {
    std::vector<std::string> strs;
    for (auto e: v) {
        strs.push_back(e.to_string());
    }
    return to_string(strs);
}

#endif /* VECTOR_FORMAT_HPP */
