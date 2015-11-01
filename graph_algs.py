from __future__ import print_function, division
import networkx as nx
from networkx.readwrite import json_graph
import json


def get_i_x_y(i_x, i_y, n_y):
    return i_y + i_x * n_y


def get_periodic_two_dim_square_lattice(initial_state):
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


def get_periodic_two_dim_hex_lattice(initial_state):
    n_x, n_y = initial_state.shape

    disps = (
        (0, 2),
        (0, 1),
        (1, 1),
        (1, 0),
        (1, -1),
        (0, -1),
        (0, -2),
        (-1, -2),
        (-1, -1),
        (-1, 0),
        (-1, 1),
        (-1, 2),
    )

    flip = False
    g = nx.DiGraph()
    for i_x in range(n_x):
        for i_y in range(n_y):
            name = "({}, {})".format(i_x, i_y)
            state = initial_state[i_x, i_y]
            g.add_node((i_x, i_y), state=state, name=name)

    for i_x in range(n_x):
        for i_y in range(n_y):
            flip = not flip
            for di, dj in disps:
                if flip:
                    di *= -1
                i_x_d = i_x + di
                i_y_d = i_y + dj
                if i_x_d > n_x - 1:
                    i_x_d -= n_x
                    # continue
                elif i_x_d < 0:
                    i_x_d += n_x
                    # continue
                if i_y_d > n_y - 1:
                    i_y_d -= n_y
                    # continue
                elif i_y_d < 0:
                    i_y_d += n_y
                    # continue
                g.add_edge((i_x, i_y), (i_x_d, i_y_d))
    return g


def augment_nx(g):
    for node, data in g.nodes(data=True):
        data['name'] = node
        data['state'] = 1


def nx_to_d3(g):
    json_dict = json_graph.node_link_data(g)
    for i, node in enumerate(json_dict['nodes']):
        node['index'] = i
    return json_dict


def d3_to_nx(json_dict):
    return json_graph.node_link_graph(json_dict)


def get_layout(g):
    return nx.spring_layout(g, iterations=100)
