from owm import OWM
from flask import current_app


class Plugins:
    def __init__(self):
        self.plugins = list()
        self.plugins.append(OWM(current_app.config['API_KEY'],
                                current_app.config['CITY_ID']))
