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
        json = db.get('network')
        network = ought_ext.Network(json)
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


@app.route("/create/<int:x>,<int:y>")
def create(x, y):
    initial_state = np.random.randint(0, 2, size=[x, y])
    nx_graph = graph_algs.get_periodic_two_dim_lattice(initial_state)
    data = graph_algs.nx_to_d3(nx_graph)
    data_json = json.dumps(data)
    network = ought_ext.Network(data_json)
    _set_network(network)
    return 'Network created successfully'


@app.route("/iterate")
def iterate():
    _get_network().iterate()
    return 'Network iterated successfully'


if __name__ == "__main__":
    app.run()
