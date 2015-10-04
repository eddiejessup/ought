#ifndef OUGHT_HPP
#define OUGHT_HPP

#include <vector>
#include <sstream>
#include <stdexcept>
#include <iostream>
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

using std::vector;
using std::string;
using std::shared_ptr;

template<typename T>
auto index_of(const T& item, vector<T> v);

template<typename T>
string object_vector_to_string(const vector<T>& v);

template<typename T>
string string_vector_to_string(const vector<T>& v);

template<typename T>
string pointer_vector_to_string(const vector<T>& v);

template<typename T>
string primitive_vector_to_string(const vector<T>& v);

class State {
    private:
        int label = -1;
        int cached_label = -1;

    public:
        State();
        State(int label);
        auto is_updated() const;
        auto is_cached() const;
        auto is_dirty() const;
        auto get_label() const;
        string to_string() const;
        void cache();
        void uncache();
        auto transition_func(int self_label, vector<int> labels);
        void iterate(const vector<State>& friend_states);
};

class Node {
    private:
        string name;
        State state;
        vector<shared_ptr<Node>> friends;

    public:
        Node(string name, State state);
        auto get_label() const;
        auto get_state() const;
        auto get_name() const;
        vector<State> get_friend_states() const;
        vector<Node> get_friends() const;
        auto to_string() const;
        void cache();
        void uncache();
        void add_friend(shared_ptr<Node> node);
        void iterate();
};

class Network {
    private:
        vector<shared_ptr<Node>> nodes;

    public:
        Network(string json);
        Network(vector<shared_ptr<Node>> nodes);
        Network(int n_x, int n_y);
        auto get_names() const;
        string to_json() const;
        string to_string() const;
        void iterate();
};

vector<vector<int>> get_bit_array(int nX, int nY);
auto get_i_x_y(const int i_x, const int i_y, const int n_y);
vector<shared_ptr<Node>> get_periodic_two_dim_lattice(const vector<vector<int>> initial_state);
#endif /* OUGHT_HPP */
