import json
from flask import Flask
from flask_restplus import Resource, Api
from Queue import Queue, Empty
from pluginloader import PluginLoader

app = Flask(__name__)
api = Api(app)
app.config.from_pyfile('config.py')
plugins = PluginLoader(app.config)

L1_DATASOURCES = Queue(50)
L2_DATASOURCES = Queue(50)


@api.route('/line/<lineno>')
class Lines(Resource):
    lines = {
        'status': 'success',
        'data': []
    }

    def get(self, lineno):
        lineno = int(lineno)

        if lineno > 1:
            return {'status': 'fail', 'text': 'Line does not exist'}

        try:
            if lineno == 0:
                while True:
                    ds = L1_DATASOURCES.get(False)
                    self.lines['data'].append(ds.run())
                    L1_DATASOURCES.task_done()
            if lineno == 1:
                self.lines[1] = ''
                while True:
                    ds = L2_DATASOURCES.get(False)
                    self.lines['data'].append(ds.run())
                    L2_DATASOURCES.task_done()
        except Empty as e:
            pass

        return json.dumps(self.lines)


@api.route('/message/<message>')
class Messages(Resource):
    def put(self, message):
        pass


def test_ds():
    return "Test"


if __name__ == '__main__':
    for plugin in plugins.plugins:
        L1_DATASOURCES.put(plugin)

    app.run(debug=True)
