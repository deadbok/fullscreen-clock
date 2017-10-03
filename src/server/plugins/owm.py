import urllib2
import requests
import json
from flask import url_for
from pluginbase import PluginBase


class OWM(PluginBase):
    icon_ids = ["01d", "01n", "02d", "02n", "03d", "03n",
                "04d", "04n", "09d", "09n", "10d", "10n",
                "11d", "11n", "13d", "13n", "50d", "50n"]

    def __init__(self, config):
        self.api_key = config['OWM_API_KEY']
        self.city_id = config['OWM_CITY_ID']
        self.icon_dir = config['OWM_ICON_DIR']
        self.celsius = config['OWM_CELSIUS']
        if self.icon_dir[-1] is not '/':
            self.icon_dir += '/'
        self.interval = 1

        super(OWM, self).__init__('owm')
        self.get_icons()

    def get_icons(self):
        print('Downloading weather icons')
        i = 1
        for icon_id in OWM.icon_ids:
            url = "http://openweathermap.org/img/w/" + icon_id + ".png"

            image_stream = urllib2.urlopen(url)
            image_file = open(self.icon_dir + icon_id + ".png", 'wb')

            block_sz = 8192
            while True:
                buffer = image_stream.read(block_sz)
                if not buffer:
                    break

                print(str(i) + '/' + str(len(OWM.icon_ids)))
                image_file.write(buffer)

            image_file.close()
            i += 1

    def update(self):
        resp = requests.get('http://api.openweathermap.org/data/2.5/weather',
                     params={'id': self.city_id, 'APPID': self.api_key})
        weather_data = json.loads(resp.text)

        if len(weather_data['weather']) > 0:
            self.ret['icon'] = url_for('static', filename=self.icon_dir + weather_data['weather'][0]['icon'] + '.png')

        kelvin = weather_data['main']['temp']
        if self.celsius:
            self.ret['text'] = str(kelvin - 273.15) + u'00B0C'
        else:
            self.ret['text'] = str((9.0 / 5) * (kelvin - 273.15) + 32) + u'00B0F'


def owm(config):
    return OWM(config)