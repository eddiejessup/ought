class Network(object):

    def __init__(self, nodes):
        self.nodes = nodes

    def iterate(self):
        for node in self.nodes:
            # Make a node save its state at this point
            node.cache()
        for node in self.nodes:
            # Set a node's state based on the old state of its friends
            node.iterate()

    @property
    def states(self):
        return [node.state for node in self.nodes]


class Node(object):

    def __init__(self, initial_state, transition_func):
        self.friends = []
        self.state = initial_state
        self.transition_func = transition_func

    def cache(self):
        self.old_state = self.state

    def iterate(self):
        states = (node.old_state for node in self.friends)
        self.state = self.transition_func(self.state, states)


def transition_func(self_state, friend_states):
    num_state_1_for_birth = 3
    min_num_state_1_for_sufficiency = 2
    max_num_state_1_for_food = 3

    num_state_1 = 0
    for state in friend_states:
        num_state_1 += state == 1

    if self_state == 1:
        if num_state_1 < min_num_state_1_for_sufficiency:
            return 0
        if num_state_1 > max_num_state_1_for_food:
            return 0
        else:
            return 1
    else:
        return num_state_1 == num_state_1_for_birth

initial_state = 1
node_1 = Node(initial_state=0, transition_func=transition_func)
node_2 = Node(initial_state=1, transition_func=transition_func)
node_1.friends.append(node_2)
node_2.friends.append(node_1)
nodes = set((node_1, node_2))
network = Network(nodes)

while True:
    network.iterate()
    print(network.states)
