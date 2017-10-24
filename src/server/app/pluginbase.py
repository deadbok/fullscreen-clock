import os
import time
import urllib2
import cStringIO
import copy
from PIL import Image


class UnImplementedUpdateException(Exception):
    def __init___(self):
        super(UnImplementedUpdateException, self).__init__(
            "Update must be implemented in child class.}")


class PluginBase(object):
    def __init__(self, config, static_path):
        self.display_sec = 60
        self.interval_sec = 60
        self.repeat = 0
        self.last_run_time = 0
        self.line = 0
        self.icon_dir = static_path + '/images/'
        self.icon_url = ''
        self.icon_size = (64, 64)
        self.ret = {'text': '', 'icon': '', 'icon_url': '', 'seconds': self.display_sec}

    def fetch_icon(self, url):
        ret = '';
        if url is not '':
            buffer = cStringIO.StringIO(urllib2.urlopen(url).read())

            image = Image.open(buffer)
            icon = image.resize(self.icon_size, Image.LANCZOS)

            #ret = self.icon_dir
            ret += os.path.splitext(os.path.basename(url))[0]
            ret += '.png'

            icon.save(self.icon_dir + ret, 'PNG')

        return (ret)

    def update(self):
        raise UnImplementedUpdateException()

    def run(self):
        if time.time() > (self.last_run_time + self.interval_sec):
            self.last_run_time = time.time()
            self.update()
            if self.repeat > 0:
                self.repeat -= 1
        return (copy.deepcopy(self.ret))
