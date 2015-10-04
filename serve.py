import flask
import ought_ext
from flask.ext.cors import CORS


app = flask.Flask(__name__)
app.debug = True
CORS(app)

database = {}


@app.route("/")
def describe():
    return database.get('network').to_json()


@app.route("/create/<int:x>,<int:y>")
def create(x, y):
    database['network'] = ought_ext.Network(x, y)
    return 'Network created successfully'


@app.route("/iterate")
def iterate():
    database['network'].iterate()
    return 'Network iterated successfully'

if __name__ == "__main__":
    app.run()
