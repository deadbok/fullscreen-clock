import json
from Queue import Queue, Empty

from flask import Flask
from flask_restplus import Resource, Api

from message import Message
from pluginloader import PluginLoader

app = Flask(__name__)
api = Api(app)
app.config.from_object('config')
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
        plugins = [[], []]

        if lineno > 1:
            return {'status': 'fail',
                    'data': [
                        {
                            'text': 'Line does not exist',
                            'icon': ''
                        }
                    ]
                    }

        try:
            if lineno == 0:
                while True:
                    ds = L1_DATASOURCES.get(False)
                    self.lines['data'].append(ds.run())
                    plugins[0].append(ds)
                    L1_DATASOURCES.task_done()
            if lineno == 1:
                self.lines[1] = ''
                while True:
                    ds = L2_DATASOURCES.get(False)
                    self.lines['data'].append(ds.run())
                    plugins[1].append(ds)
                    L2_DATASOURCES.task_done()
        except Empty as e:
            pass

        for task in plugins[0]:
            if task.interval > 0:
                L1_DATASOURCES.put(task)
        for task in plugins[1]:
            if task.interval > 0:
                L2_DATASOURCES.put(task)

        return json.dumps(self.lines)


@api.route('/message/<message>/<icon>')
class Messages(Resource):
    def put(self, message, icon):
        msg = Message(message, icon)
        L2_DATASOURCES.put(msg)
