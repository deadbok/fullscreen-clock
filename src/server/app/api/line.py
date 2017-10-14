from Queue import Empty

from flask import current_app
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
        plugins = [[], []]
        ret = []

        try:
            while True:
                if lineno == 0:
                    ds = L1_DATASOURCES.get(False)
                if lineno == 1:
                    ds = L2_DATASOURCES.get(False)
                data = ds.run()
                data['icon'] = current_app.static_folder + data['icon']
                ret.append(data)
                if lineno == 0:
                    plugins[0].append(ds)
                    L1_DATASOURCES.task_done()
                if lineno == 1:
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

        return ret
