from fltk import *
import sys
from datetime import datetime

__version__ = '0.0.1'


def close_cb(ptr):
    sys.exit(0)


def update_time():
    global timebox
    timebox.label('{}'.format(datetime.now().strftime('%H:%M')))
    Fl.repeat_timeout(5.0, update_time)


window = Fl_Window(Fl.w(), Fl.h())
window.fullscreen()
window.color(fl_rgb_color(0))
window.label('Full screen clock V{}'.format(__version__))

timebox = Fl_Box(0, 50, Fl.w(), int(Fl.h() / 1.5) - 50)
timebox.label('{}'.format(datetime.now().strftime('%H:%M')))
timebox.labelcolor(fl_rgb_color(255))
timebox.labelsize(int(Fl.h() / 2))

button = Fl_Button(Fl.w() - 20, 0, Fl.w(), 20)
button.label("X")
button.color(fl_rgb_color(0))
button.down_box(FL_NO_BOX)
button.down_color(fl_rgb_color(0))

button.callback(close_cb)

window.end()
window.show(len(sys.argv), sys.argv)

Fl.add_timeout(5.0, update_time)

Fl.run()
