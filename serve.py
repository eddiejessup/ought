from __future__ import print_function, division
import json
import numpy as np
import flask
from flask.ext.cors import CORS
import redis
import ought_ext
import graph_algs

db = redis.StrictRedis(host='localhost', port=6379, db=0)

app = flask.Flask(__name__)
app.debug = True
CORS(app)

state = {}


def _get_network():
    if 'network' in state:
        return state['network']
    elif 'network' in db:
        network_json = db.get('network')
        network = ought_ext.Network(network_json)
        state['network'] = network
        return network
    else:
        return None


def _set_network(network):
    state['network'] = network
    db.set('network', network.to_json())


@app.route("/")
def describe():
    return _get_network().to_json()


@app.route("/prelaid")
def describe_prelaid():
    d3_json = _get_network().to_json()
    d3_dict = json.loads(d3_json)
    nx_graph = graph_algs.d3_to_nx(d3_dict)
    layout = graph_algs.get_layout(nx_graph)
    for name, position in layout.items():
        # Scale position to lie in [0, 1]
        p = 0.5 * (position + 1.0)
        nx_graph.node[name]['x'] = p[0]
        nx_graph.node[name]['y'] = p[1]
    json_str = json.dumps(graph_algs.nx_to_d3(nx_graph))
    return json_str


@app.route("/create/<int:x>,<int:y>")
def create(x, y):
    initial_state = np.random.randint(0, 2, size=[x, y])
    nx_graph = graph_algs.get_periodic_two_dim_square_lattice(initial_state)
    data = graph_algs.nx_to_d3(nx_graph)
    data_json = json.dumps(data)
    network = ought_ext.Network(data_json)
    _set_network(network)
    return 'Network created successfully'


@app.route("/iterate")
def iterate():
    _get_network().iterate(1)
    return 'Network iterated successfully'


if __name__ == "__main__":
    app.run()
