from __future__ import print_function
import urllib2
from pluginbase import PluginBase


class OWM(PluginBase):
    icon_ids = ["01d", "01n", "02d", "02n", "03d", "03n",
                "04d", "04n", "09d", "09n", "10d", "10n",
                "11d", "11n", "13d", "13n", "50d", "50n"]

    def __init__(self, api_key='', city_id=''):
        self.api_key = config['OWM_API_KEY']
        self.city_id = config['OWM_CITY_ID']
        super(OWM, self).__init__('owm')
        self.get_icons()

    def get_icons(self):
        print('Downloading weather icons')
        for icon_id in self.icons_ids:
            url = "http://openweathermap.org/img/w/" + icon_id + ".png"

            image_stream = urllib2.urlopen(url)
            image_file = open(icon_id + ".png", 'wb')

            block_sz = 8192
            while True:
                buffer = image_stream.read(block_sz)
                if not buffer:
                    break

                print('.', end='')
                image_file.write(buffer)

            image_file.close()
            print('')

    def update(self):
        return self.ret
