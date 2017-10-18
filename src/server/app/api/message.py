from flask import request
from flask_restplus import Resource, fields

from app.message import Message
from app.queue import L2_DATASOURCES
from app.api import api

ns = api.namespace('message',
                   description='Operations to add messages to be displayed on the clock')

message = api.model('Message', {
    'text': fields.String(description='The text of the messae'),
    'icon': fields.String(description='Optional URL to an icon to include'),
    'repeat': fields.Integer(description='Number times to display the message'),
    'interval_sec': fields.Integer(description='Minimum number of seconds between message repeats'),
    'display_sec': fields.Integer(description='Minimum number of seconds the messaage is visible')
})


@ns.route('/')
@api.expect(message)
@api.response(404, 'Message was not added.')
class MessageEP(Resource):
    """
    Enqueue a message in the botton line of the clock display.
    """

    @api.response(201, 'Message successfully added.')
    def put(self):
        """
        Add message to the bottom line of the clock display.
        """
        data = request.json
        msg = Message(data['text'], data['icon'], data['interval_sec'], data['display_sec'], data['repeat'])
        L2_DATASOURCES.put(msg)
        return None, 201
