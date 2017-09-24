from flask_restful import Resource

class Lines(Resource):
    def get(self, number):
        result = {'status': 'OK'}
        print('Lines')
        return jsonify(result)
