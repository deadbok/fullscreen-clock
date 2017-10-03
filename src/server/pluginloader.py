import os
import importlib


class PluginLoader(object):
    def __init__(self, config):
        self.plugins = list()
        self.config = config

        plugin_files = [pfile for pfile in os.listdir(config['PLUGIN_DIR']) if pfile.endswith('.py')]

        plugins = map(self.module_name, plugin_files)
        # import parent module / namespace
        importlib.import_module('plugins')
        for plugin in plugins:
            if not plugin.startswith('_'):
                module = importlib.import_module('plugins.' + plugin, package="plugins")

                self.plugins.append(getattr(module, plugin)(config))

    def module_name(self, filename):
        return os.path.splitext(filename)[0]
