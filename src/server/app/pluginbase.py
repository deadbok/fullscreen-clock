import os
import time
import urllib2
import cStringIO
from PIL import Image


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
        self.icon_dir = 'app/static/images/'
        self.icon_size = (64, 64)

    def fetch_icon(self, url):
        image_stream = urllib2.urlopen(url)

        buffer = cStringIO.StringIO(urllib2.urlopen(url).read())

        image = Image.open(buffer)
        icon = image.resize(self.icon_size, Image.LANCZOS)

        filename = self.icon_dir
        filename += os.path.splitext(os.path.basename(url))[0]
        filename += '.png'

        icon.save(filename, 'PNG')

        return (filename)

    def update(self):
        raise UnImplementedUpdateException()

    def run(self):
        if time.time() > (self.last_run_time + self.interval):
            self.last_run_time = time.time()
            self.update()
        return (self.ret)
