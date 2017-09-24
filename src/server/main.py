from flask import Flask
from flask_restplus import Resource, Api
from Queue import Queue

app = Flask(__name__)
api = Api(app)

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
                line[0] = ''
                while True:
                    ds = L1_DATASOURCES.get(False)
                    line[0] += ds()
                    do_work(ds)
                    L1_DATASOURCES.task_done()
            if lineno == 1:
                line[1] = ''
                while True:
                    ds = L2_DATASOURCES.get(False)
                    line[1] += ds()
                    do_work(ds)
                    L2_DATASOURCES.task_done()
        except:
            print('Exception')

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
