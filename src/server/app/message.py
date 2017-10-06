from pluginbase import PluginBase


class Message(PluginBase):
    message = ''
    icon = ''

    def __init__(self, message, icon):
        Message.message = message
        Message.icon = icon
        super(Message, self).__init__()

        # Do not repeat.
        self.interval = -1

    def update(self):
        self.ret['text'] = Message.message
        self.ret['icon'] = Message.icon
