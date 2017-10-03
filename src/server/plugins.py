class Plugins:
    def __init__(self, config):
        self.plugins = list()
        self.config = config

        plugin_files = os.listdir(config['PLUGIN_DIR'])

        # Function to create plugin names from file names.
        def form_module(fp): '.' + os.path.splitext(fp)[0]
        plugins = map(form_module, plugin_files)
        # import parent module / namespace
        importlib.import_module('plugins')
        for plugin in plugins:
            if not plugin.startswith('__'):
                module = importlib.import_module(plugin, package="plugins")

                self.plugins.append(module.Plugin(config))
