from flask_restful import Resource
from queue import Queue

MESSAGE_QUEUE = Queue(20)


class Message(Resource):
    def get(self):
        result = {'status': 'OK'}
        print('Message')
        return jsonify(result)
