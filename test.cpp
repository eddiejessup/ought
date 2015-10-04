#include <iostream>
#include <fstream>
#include <string>
#include "ought.hpp"

int main () {
    std::ifstream json_file { "test.json" };
    std::string maker_json { std::istreambuf_iterator<char> {json_file}, std::istreambuf_iterator<char> {} };
    Network network { maker_json };
    std::string from_json = network.to_json();
    Network network_clone { from_json };
    std::ofstream json_clone_file { "test_clone.json" };
    json_clone_file << network_clone.to_json();
    return 0;
}
