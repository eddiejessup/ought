import flask
import ought_ext
from flask.ext.cors import CORS
import redis


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
    network = ought_ext.Network(x, y)
    _set_network(network)
    return 'Network created successfully'


@app.route("/iterate")
def iterate():
    _get_network().iterate()
    return 'Network iterated successfully'


if __name__ == "__main__":
    app.run()
