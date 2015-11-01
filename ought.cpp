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

auto State::transition_func(int self_label, vector<int> labels) {
    const auto num_alive_for_birth = 3;
    const auto min_num_alive = 2;
    const auto max_num_alive = 3;

    const auto label_alive = 1;
    const auto label_dead = 0;

    auto num_alive = 0;
    for (auto label : labels) {
        num_alive += label == label_alive ? 1 : 0;
    }

    if (self_label == label_alive) {
        if ((num_alive < min_num_alive) || (num_alive > max_num_alive)) {
            return label_dead;
        } else {
            return label_alive;
        }
    } else {
        if (num_alive == num_alive_for_birth) {
            return label_alive;
        } else {
            return label_dead;
        }
    }
}

// auto State::transition_func(int self_label, vector<int> labels) {
//    if (labels.size() == 0) {
//        throw std::length_error("Node has no friends, cannot apply this transition function");
//    } else {
//        return labels[0];
//    }
// }

// auto State::transition_func(int self_label, vector<int> labels) {
//    return (self_label + 1) % 10;
// }

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
    rapidjson::Document json_doc;
    json_doc.Parse(json.c_str());
    const rapidjson::Value& nodes_json {json_doc["nodes"]};
    nodes.resize(nodes_json.Size());

    // Rapidjson does not support range expressions
    for (rapidjson::SizeType i = 0; i < nodes_json.Size(); i++) {
        string name = nodes_json[i]["name"].GetString();
        int state_label = nodes_json[i]["state"].GetInt();
        int index = nodes_json[i]["index"].GetInt();
        State state {state_label};
        shared_ptr<Node> node {new Node {name, state}};
        nodes[index] = node;
    }
    const rapidjson::Value& links_json {json_doc["links"]};
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
    rapidjson::Document json_doc;
    json_doc.SetObject();
    rapidjson::Document::AllocatorType& allocator = json_doc.GetAllocator();

    rapidjson::Value nodes_array {rapidjson::kArrayType};

    for (int node_index = 0; node_index != this->nodes.size(); ++node_index) {
        rapidjson::Value obj_value;
        obj_value.SetObject();
        auto node = this->nodes[node_index];

        rapidjson::Value name_key {"name", json_doc.GetAllocator()};
        rapidjson::Value name_val {node->get_name().c_str(), json_doc.GetAllocator()};
        obj_value.AddMember(name_key, name_val, json_doc.GetAllocator());

        rapidjson::Value id_key {"id", json_doc.GetAllocator()};
        rapidjson::Value id_val {node->get_name().c_str(), json_doc.GetAllocator()};
        obj_value.AddMember(id_key, id_val, json_doc.GetAllocator());

        rapidjson::Value index_key {"index", json_doc.GetAllocator()};
        rapidjson::Value node_index_val {node_index};
        obj_value.AddMember(index_key, node_index_val, json_doc.GetAllocator());

        rapidjson::Value state_key {"state", json_doc.GetAllocator()};
        rapidjson::Value state_val {node->get_label()};
        obj_value.AddMember(state_key, state_val, json_doc.GetAllocator());

        nodes_array.PushBack(obj_value, allocator);
    }

    json_doc.AddMember("nodes", nodes_array, allocator);

    rapidjson::Value links_array {rapidjson::kArrayType};

    auto names = this->get_names();
    for (int node_index = 0; node_index != this->nodes.size(); ++node_index) {
        auto node = this->nodes[node_index];
        for (auto friend_cur : node->get_friends()) {
            rapidjson::Value obj_value;
            obj_value.SetObject();

            int friend_index = index_of(friend_cur.get_name(), names);

            obj_value.AddMember("source", node_index, allocator);
            obj_value.AddMember("target", friend_index, allocator);

            links_array.PushBack(obj_value, allocator);
        }
    }

    json_doc.AddMember("links", links_array, allocator);

    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer {str_buf};
    json_doc.Accept(writer);

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

void Network::iterate(int n) {
    for (int i=0; i<n; ++i) {
        this->iterate();
    }
}
