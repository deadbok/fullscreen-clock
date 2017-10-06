from flask import  Blueprint

bp = Blueprint('root', __name__,
                        template_folder='templates')

import index