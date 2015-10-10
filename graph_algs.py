import networkx as nx


def get_i_x_y(i_x, i_y, n_y):
    return i_y + i_x * n_y


def get_periodic_two_dim_lattice(initial_state):
    n_x, n_y = initial_state.shape

    g = nx.DiGraph()

    for i_x in range(n_x):
        for i_y in range(n_y):
            name = "({}, {})".format(i_x, i_y)
            state = initial_state[i_x, i_y]
            i_x_y = get_i_x_y(i_x, i_y, n_y)
            g.add_node(i_x_y, state=state, name=name)
    for i_x in range(n_x):
        i_x_up = i_x + 1 if (i_x < n_x - 1) else 0
        i_x_down = i_x - 1 if (i_x > 0) else n_x - 1
        for i_y in range(n_y):
            i_y_up = i_y + 1 if (i_y < n_y - 1) else 0
            i_y_down = i_y - 1 if (i_y > 0) else n_y - 1
            i_source_node = get_i_x_y(i_x, i_y, n_y)
            g.add_edge(i_source_node, get_i_x_y(i_x_up, i_y, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x_down, i_y, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x, i_y_up, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x, i_y_down, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x_up, i_y_up, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x_up, i_y_down, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x_down, i_y_up, n_y))
            g.add_edge(i_source_node, get_i_x_y(i_x_down, i_y_down, n_y))
    return g


def augment_nx(g):
    for node, data in g.nodes(data=True):
        data['name'] = node
        data['state'] = 1


def nx_to_d3(g):
    keys_and_data = g.nodes(data=True)
    keys, nx_data = zip(*keys_and_data)
    key_to_index = {key: index for index, key in enumerate(keys)}
    d3_data = {'nodes': [], 'links': []}
    for key_source, nx_data_source in keys_and_data:
        index_source = key_to_index[key_source]
        d3_data['nodes'].append({"name": str(key_source),
                                 "index": index_source,
                                 "state": int(nx_data_source['state'])})
        for key_target in g.neighbors(key_source):
            index_target = key_to_index[key_target]
            d3_data['links'].append({'source': index_source,
                                     'target': index_target})
    return d3_data
