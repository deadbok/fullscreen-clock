import os

from app.pluginbase import PluginBase
from flask import current_app, url_for


class Message(PluginBase):
    def __init__(self, message, icon, interval_sec = 60, display_sec = 60, repeat = 0):
        super(Message, self).__init__(None, current_app.static_folder)

        self.icon_dir += current_app.config['MSG_ICON_DIR']
        self.icon_url = 'images/' + current_app.config['MSG_ICON_DIR']
        self.icon_size = current_app.config['ICON_SIZE']

        self.message = message
        self.interval_sec = interval_sec
        self.display_sec = display_sec
        self.repeat = repeat
        self.icon = ''

        if icon is not u'':
            if self.icon_dir[-1] is not '/':
                self.icon_dir += '/'
            if not os.path.isdir(self.icon_dir):
                os.makedirs(self.icon_dir)

            self.icon = self.fetch_icon(icon)

        # Do not repeat.
        self.interval = -1

    def update(self):
        self.ret['text'] = self.message
        if self.icon is not '':
            self.ret['icon'] = self.icon_dir + self.icon
            self.ret['icon_url'] = self.icon_url + self.icon
        else:
            self.ret['icon'] = self.ret['icon_url'] = ''
        self.ret['seconds'] = self.display_sec
