from app import app
from app import plugins
from app import L1_DATASOURCES


if __name__ == '__main__':
    for plugin in plugins.plugins:
        L1_DATASOURCES.put(plugin)

    app.run(debug=app.config['DEBUG'], host=app.config['HOST'])
