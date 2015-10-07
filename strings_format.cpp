#include <vector>
#include <string>
#include <sstream>

std::string to_string(const std::vector<std::string>& strs) {
    std::stringstream stream;
    stream << "[ ";
    for (auto i = 0; i != strs.size(); ++i) {
        if (i > 0) {
            stream << ", ";
        }
        stream << strs[i];
    }
    stream << " ]";
    return stream.str();
}
