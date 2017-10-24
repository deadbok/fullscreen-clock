

from flask import Blueprint, Flask, url_for

from pluginloader import PluginLoader

from app.api import api
from app.api.message import ns as message_namespace
from app.api.line import ns as line_namespace

from app.views import bp as root_blueprint

app = Flask(__name__)
app.config.from_object('config')
plugins = PluginLoader(app.config, app.static_folder)


def init_app(flask_app):
    rest_blueprint = Blueprint('api', __name__, url_prefix='/api')
    api.init_app(rest_blueprint)
    api.add_namespace(line_namespace)
    api.add_namespace(message_namespace)
    flask_app.register_blueprint(rest_blueprint)
    flask_app.register_blueprint(root_blueprint)
