class UnImplementedUpdateException(Exception):
    def __init___(self):
        super().__init__("Update must be implemented in child class.}")


class PluginBase:
    def __init__(self, name):
        self.name = name
        self.ret = [{'text': '', 'icon': ''}]

    def update(self):
        raise UnImplementedUpdateException()
