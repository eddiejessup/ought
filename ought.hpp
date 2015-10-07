#ifndef OUGHT_HPP
#define OUGHT_HPP

#include <vector>
#include <string>

template<typename T>
auto index_of(const T& item, std::vector<T> v);

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
        std::string to_string() const;
        void cache();
        void uncache();
        auto transition_func(int self_label, std::vector<int> labels);
        void iterate(const std::vector<State>& friend_states);
};

class Node {
    private:
        std::string name;
        State state;
        std::vector<std::shared_ptr<Node>> friends;

    public:
        Node(std::string name, State state);
        auto get_label() const;
        auto get_state() const;
        auto get_name() const;
        std::vector<State> get_friend_states() const;
        std::vector<Node> get_friends() const;
        auto to_string() const;
        void cache();
        void uncache();
        void add_friend(std::shared_ptr<Node> node);
        void iterate();
};

class Network {
    private:
        std::vector<std::shared_ptr<Node>> nodes;

    public:
        Network(std::string json);
        Network(std::vector<std::shared_ptr<Node>> nodes);
        Network(int n_x, int n_y);
        auto get_names() const;
        std::string to_json() const;
        std::string to_string() const;
        void iterate();
};

vector<vector<int>> get_bit_array(int nX, int nY);
auto get_i_x_y(const int i_x, const int i_y, const int n_y);
vector<shared_ptr<Node>> get_periodic_two_dim_lattice(const vector<vector<int>> initial_state);
#endif /* OUGHT_HPP */
