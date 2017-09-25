from flask import Flask
from flask_restplus import Resource, Api
from Queue import Queue, Empty

app = Flask(__name__)
api = Api(app)
config = app.config.from_pyfile('config.py')

L1_DATASOURCES = Queue(50)
L1_DATASOURCES = Queue(50)


@api.route('/line/<lineno>')
class Lines(Resource):
    lines = [
        'Line1',
        'Line2'
    ]

    def get(self, lineno):
        lineno = int(lineno)
        if lineno > 1:
            return {'status': 'fail', 'test': 'Line does not exist'}

        try:
            if lineno == 0:
                self.lines[0] = ''
                while True:
                    ds = L1_DATASOURCES.get(False)
                    self.lines[0] += ds()
                    L1_DATASOURCES.task_done()
            if lineno == 1:
                self.lines[1] = ''
                while True:
                    ds = L2_DATASOURCES.get(False)
                    self.lines[1] += ds()
                    L2_DATASOURCES.task_done()
        except Empty as e:
            print('Exception: ' + str(e))

        return {'text': self.lines[lineno]}


@api.route('/message/<message>')
class Messages(Resource):
    def put(self, message):
        pass


def test_ds():
    return "Test"


if __name__ == '__main__':
    L1_DATASOURCES.put(test_ds)
    app.run(debug=True)
