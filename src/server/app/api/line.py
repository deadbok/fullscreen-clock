from Queue import Empty

from flask import current_app, url_for
from flask_restplus import Resource, fields

from app.api import api
from app.queue import L1_DATASOURCES
from app.queue import L2_DATASOURCES

ns = api.namespace('line',
                   description='Operations to get lines displayed on the clock')


@ns.route('/<int(min=0, max=1):lineno>')
@api.response(404, 'Line not found.')
class LineEP(Resource):
    """
    Endpoint to get the lines to be displayed on the clock.
    """

    def get(self, lineno):
        """
        Get the messages queued for a line.
        """
        lineno = int(lineno)
        ret = {'text': '', 'icon': '', 'icon_url': '', 'seconds': 0}
        plugin_queue = None

        if lineno == 0:
            plugin_queue = L1_DATASOURCES
        elif lineno == 1:
            plugin_queue = L2_DATASOURCES

        if plugin_queue is not None:
            try:
                plugin = plugin_queue.get(False)
                ret = plugin.run()
                if ret['icon_url'] is not '':
                    ret['icon_url'] = url_for('static', filename=ret['icon_url'])
                plugin_queue.task_done()
                if plugin.repeat != 0:
                    plugin_queue.put(plugin)
            except Empty as e:
                pass

        return ret
