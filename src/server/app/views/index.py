from flask import render_template
from app.views import bp

@bp.route('/')
def index():
    return render_template('index.html')