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
        auto get_names() const;
        std::string to_json() const;
        std::string to_string() const;
        void iterate();
};

#endif /* OUGHT_HPP */
