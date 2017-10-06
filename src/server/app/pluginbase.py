import time


class UnImplementedUpdateException(Exception):
    def __init___(self):
        super(UnImplementedUpdateException, self).__init__(
            "Update must be implemented in child class.}")


class PluginBase(object):
    def __init__(self):
        self.ret = {'text': '', 'icon': ''}
        self.interval = 60
        self.last_run_time = 0
        self.line = 0

    def update(self):
        raise UnImplementedUpdateException()

    def run(self):
        if time.time() > (self.last_run_time + self.interval):
            self.last_run_time = time.time()
            self.update()
        return (self.ret)
