#include "ought.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <stdexcept>
#include <sstream>
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "vector_format.hpp"

using std::vector;
using std::string;
using std::shared_ptr;

template<typename T>
auto index_of(const T& item, vector<T> v) {
    return std::distance(v.begin(), std::find(v.begin(), v.end(), item));
}

State::State() {
    }

State::State(int label) {
        this->label = label;
    }

auto State::is_updated() const {
        return this->cached_label == -1 && this->label != -1;
    }

auto State::is_cached() const {
        return this->cached_label != -1 && this->label == -1;
    }

auto State::is_dirty() const {
        return this->cached_label != -1 && this->label != -1;
    }

auto State::get_label() const {
        if (this->is_cached()) {
            return this->cached_label;
        } else {
            return this->label;
        }
    }

string State::to_string() const {
    std::stringstream str;
    str << "State(";
    if (this->is_updated()) {
        str << "updated, label=" << this->label;
    } else if (this->is_cached()) {
        str << "cached, cached_label=" << this->cached_label;
    } else if (this->is_dirty()) {
        str << "dirty, label=" << this->label << ", cached_label=" << this->cached_label;
    } else {
        throw std::logic_error("State is in undefined state");
    }
    str << ")";
    return str.str();
}

void State::cache() {
    if (this->label != -1) {
        this->cached_label = this->label;
        this->label = -1;
    }
}

void State::uncache() {
    this->cached_label = -1;
}

// auto State::transition_func(int self_label, vector<int> labels) {
//     const auto num_alive_for_birth = 3;
//     const auto min_num_alive = 2;
//     const auto max_num_alive = 3;

//     const auto label_alive = 1;
//     const auto label_dead = 0;

//     auto num_alive = 0;
//     for (auto label : labels) {
//         num_alive += label == label_alive ? 1 : 0;
//     }

//     if (self_label == label_alive) {
//         if ((num_alive < min_num_alive) || (num_alive > max_num_alive)) {
//             return label_dead;
//         } else {
//             return label_alive;
//         }
//     } else {
//         if (num_alive == num_alive_for_birth) {
//             return label_alive;
//         } else {
//             return label_dead;
//         }
//     }
// }

// auto State::transition_func(int self_label, vector<int> labels) {
//    if (labels.size() == 0) {
//        throw std::length_error("Node has no friends, cannot apply this transition function");
//    } else {
//        return labels[0];
//    }
// }

auto State::transition_func(int self_label, vector<int> labels) {
   return (self_label + 1) % 10;
}

void State::iterate(const vector<State>& friend_states) {
    vector<int> cached_labels;
    for (State state : friend_states) {
        cached_labels.push_back(state.cached_label);
    }
    this->label = this->transition_func(this->cached_label, cached_labels);
}

Node::Node(string name, State state) {
    this->name = name;
    this->state = state;
}

auto Node::get_label() const {
    return this->state.get_label();
}

auto Node::get_state() const {
    return this->state;
}

auto Node::get_name() const {
    return this->name;
}

vector<State> Node::get_friend_states() const {
    vector<State> friend_states;
    for (auto friend_ref : this->friends) {
        friend_states.push_back(friend_ref->get_state());
    }
    return friend_states;
}

vector<Node> Node::get_friends() const {
    vector<Node> friends;
    for (auto friend_ref : this->friends) {
        friends.push_back(*friend_ref);
    }
    return friends;
}

auto Node::to_string() const {
    vector<string> friend_names;
    for (auto friend_ref : this->friends) {
        friend_names.push_back(friend_ref->name);
    }
    std::stringstream str;
    str << "Node(name=" << this->name << ", state=" << this->state.to_string() << ", friends=" << ::to_string(friend_names) << ")";
    return str.str();
}

void Node::cache() {
    this->state.cache();
}

void Node::uncache() {
    this->state.uncache();
}

void Node::add_friend(shared_ptr<Node> node) {
    this->friends.push_back(node);
}

void Node::iterate() {
    this->state.iterate(this->get_friend_states());
}

Network::Network(string json) {
    rapidjson::Document jsonDoc;
    jsonDoc.Parse(json.c_str());
    // Rapidjson does not support range expressions
    const rapidjson::Value& nodes_json = jsonDoc["nodes"];
    for (rapidjson::SizeType i = 0; i < nodes_json.Size(); i++) {
        string name = nodes_json[i]["name"].GetString();
        int state_label = nodes_json[i]["state"].GetInt();
        State state { state_label };
        shared_ptr<Node> node {new Node {name, state}};
        nodes.push_back(node);
    }
    const rapidjson::Value& links_json = jsonDoc["links"];
    for (rapidjson::SizeType i = 0; i < links_json.Size(); i++) {
        int source = links_json[i]["source"].GetInt();
        int target = links_json[i]["target"].GetInt();
        nodes[source]->add_friend(nodes[target]);
    }
}

Network::Network(vector<shared_ptr<Node>> nodes) {
    this->nodes = nodes;
}

auto Network::get_names() const {
    vector<string> names;
    for (auto node : this->nodes) {
        names.push_back(node->get_name());
    }
    return names;
}

string Network::to_json() const {
    rapidjson::Document jsonDoc;
    jsonDoc.SetObject();
    rapidjson::Document::AllocatorType& allocator = jsonDoc.GetAllocator();

    rapidjson::Value nodes_array(rapidjson::kArrayType);

    for (int node_index = 0; node_index != this->nodes.size(); ++node_index) {
        rapidjson::Value objValue;
        objValue.SetObject();
        auto node = this->nodes[node_index];

        rapidjson::Value name_key("name", jsonDoc.GetAllocator());
        rapidjson::Value name_val(node->get_name().c_str(), jsonDoc.GetAllocator());
        objValue.AddMember(name_key, name_val, jsonDoc.GetAllocator());

        rapidjson::Value id_key("id", jsonDoc.GetAllocator());
        objValue.AddMember(id_key, name_val, jsonDoc.GetAllocator());

        rapidjson::Value index_key("index", jsonDoc.GetAllocator());
        rapidjson::Value node_index_val(node_index);
        objValue.AddMember(index_key, node_index_val, jsonDoc.GetAllocator());

        rapidjson::Value state_key("state", jsonDoc.GetAllocator());
        rapidjson::Value state_val(node->get_label());
        objValue.AddMember(state_key, state_val, jsonDoc.GetAllocator());

        nodes_array.PushBack(objValue, allocator);
    }

    jsonDoc.AddMember("nodes", nodes_array, allocator);

    rapidjson::Value links_array(rapidjson::kArrayType);

    auto names = this->get_names();
    for (int node_index = 0; node_index != this->nodes.size(); ++node_index) {
        auto node = this->nodes[node_index];
        for (auto friend_cur : node->get_friends()) {
            rapidjson::Value objValue;
            objValue.SetObject();

            int friend_index = index_of(friend_cur.get_name(), names);

            objValue.AddMember("source", node_index, allocator);
            objValue.AddMember("target", friend_index, allocator);

            links_array.PushBack(objValue, allocator);
        }
    }

    jsonDoc.AddMember("links", links_array, allocator);

    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
    jsonDoc.Accept(writer);

    return str_buf.GetString();
}

string Network::to_string() const {
    std::stringstream str;
    str << "Network(nodes=" << ::pointers_to_string(this->nodes) << ")";
    return str.str();
}

void Network::iterate() {
    for (auto node : this->nodes) {
        node->cache();
    }
    for (auto node : this->nodes) {
        node->iterate();
    }
    for (auto node : this->nodes) {
        node->uncache();
    }
}
