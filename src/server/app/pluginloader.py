import os
import importlib


class PluginLoader(object):
    def __init__(self, config, static_path):
        self.plugins = list()
        self.config = config

        print('Loading plugins:')

        plugin_files = [pfile for pfile in os.listdir(config['PLUGIN_DIR']) if
                        pfile.endswith('.py')]

        plugins = map(self.module_name, plugin_files)
        # import parent module / namespace
        importlib.import_module('plugins')
        for plugin in plugins:
            if not plugin.startswith('_'):
                print(' ' + plugin)
                module = importlib.import_module('plugins.' + plugin,
                                                 package="plugins")
                instance = getattr(module, plugin)(config, static_path)
                self.plugins.append(instance)

    def module_name(self, filename):
        return os.path.splitext(filename)[0]
