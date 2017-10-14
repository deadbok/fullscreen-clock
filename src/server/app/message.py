import os

from app.pluginbase import PluginBase
from flask import current_app


class Message(PluginBase):
    def __init__(self, message, icon):
        super(Message, self).__init__()

        self.icon_dir = current_app.static_folder + "/" + current_app.config[
            'MSG_ICON_DIR']
        self.icon_size = current_app.config['ICON_SIZE']

        self.message = message
        self.icon = ''

        if self.icon_dir[-1] is not '/':
            self.icon_dir += '/'
        if not os.path.isdir(self.icon_dir):
            os.makedirs(self.icon_dir)

        if icon is not '':
            self.icon = self.fetch_icon(icon)

        # Do not repeat.
        self.interval = -1

    def update(self):
        self.ret['text'] = self.message
        self.ret['icon'] = self.icon
