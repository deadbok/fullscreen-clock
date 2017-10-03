from pluginbase import PluginBase


class Test(PluginBase):
    def __init__(self):
        super(Test, self).__init__('test')

    def update(self):
        return self.ret

def test(config):
    return Test()