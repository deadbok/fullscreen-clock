class UnImplementedUpdateException(Exception):
    def __init___(self):
        super(UnImplementedUpdateException, self).__init__("Update must be implemented in child class.}")


class PluginBase(object):
    def __init__(self, name):
        self.name = name
        self.ret = [{'text': '', 'icon': ''}]

    def update(self):
        raise UnImplementedUpdateException()
