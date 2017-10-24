import json
import os

import requests

from app.pluginbase import PluginBase


class OWM(PluginBase):
    '''
    Class for getting city weather information from Open Weather Map.
    '''
    # Weather icon file names part.
    icon_ids = ["01d", "01n", "02d", "02n", "03d", "03n",
                "04d", "04n", "09d", "09n", "10d", "10n",
                "11d", "11n", "13d", "13n", "50d", "50n"]

    def __init__(self, config, static_path):
        super(OWM, self).__init__(config, static_path)

        self.api_key = config['OWM_API_KEY']
        self.city_id = config['OWM_CITY_ID']
        self.icon_dir += config['OWM_ICON_DIR']
        self.icon_url = 'images/' + config['OWM_ICON_DIR']
        self.celsius = config['OWM_CELSIUS']
        self.icon_size = config['ICON_SIZE']
        self.interval_sec = 600
        self.display_sec = 600
        self.repeat = -1

        if self.icon_dir[-1] is not '/':
            self.icon_dir += '/'
        if not os.path.isdir(self.icon_dir):
            os.makedirs(self.icon_dir)

        self.get_icons()

    def get_icons(self):
        print('  Downloading weather icons')
        i = 1
        for icon_id in OWM.icon_ids:
            url = "http://openweathermap.org/img/w/" + icon_id + ".png"

            self.fetch_icon(url)

            print('   ' + str(i) + '/' + str(len(OWM.icon_ids)))
            i += 1

    def update(self):
        resp = requests.get('http://api.openweathermap.org/data/2.5/weather',
                            params={'id': self.city_id, 'APPID': self.api_key})
        weather_data = json.loads(resp.text)

        kelvin = weather_data['main']['temp']
        if self.celsius:
            self.ret['text'] = unicode(str((int(kelvin - 273.15))) + u'\xB0C')
        else:
            self.ret['text'] = unicode(str(
                int((9.0 / 5) * (kelvin - 273.15)) + 32) + u'\xB0F')

        if len(weather_data['weather']) > 0:
            self.ret['icon'] = self.icon_dir + \
                               weather_data['weather'][0]['icon'] + '.png'
            self.ret['icon_url'] = self.icon_url + '/' + \
                               weather_data['weather'][0]['icon'] + '.png'

        self.ret['seconds'] = self.display_sec



def owm(config, static_path):
    return OWM(config, static_path)
