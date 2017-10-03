from pluginbase import PluginBase


class Plugin(PluginBase):
    def __init__(self, api_key='', city_id=''):
        super(Plugin, self).__init__('test')

    def update(self):
        return self.ret
