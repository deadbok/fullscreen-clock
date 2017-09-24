#include <cmath>
#include <cstring>
#include <fstream>
#include <iostream>

#include <curl/curl.h>

#include "owm.h"

/*
 * TODO: Add temperature unit handling.
 */

std::string OWM::icons_ids[] = {"01d", "01n", "02d", "02n", "03d", "03n",
                                "04d", "04n", "09d", "09n", "10d", "10n",
                                "11d", "11n", "13d", "13n", "50d", "50n"};

OWM::OWM(std::string &city_id, std::string &api_key, bool celsius)
{
    this->city_id = city_id;
    this->api_key = api_key;
    this->get_icons();
    this->celsius = celsius;
}

size_t OWM::icon_cb(char *in, uint size, uint nmemb, void *png_file)
{
    size_t ret = size * nmemb;

    reinterpret_cast< std::ofstream * >(png_file)->write(in, ret);
    if (reinterpret_cast< std::ofstream * >(png_file)->fail())
    {
        std::cerr << std::endl
                  << "Error saving temporary icon file" << std::endl;
        return 0;
    }

    return ret;
}

int OWM::get_icons(void)
{
    CURL *curl;
    CURLcode res;

    std::cout << "Downloading weather icons";

    for (unsigned int i = 0; i < 18; i++)
    {
        std::cout << ".";

        std::string icon_url = "http://openweathermap.org/img/w/xxx.png";
        icon_url.replace(32, 3, icons_ids[i]);

        std::ofstream png_file;

        png_file.open(std::string("/var/tmp/" + icons_ids[i] + ".png").c_str(),
                      std::ofstream::binary);
        if (png_file.fail())
        {
            std::cerr << std::endl
                      << "Error opening temporary icon file" << std::endl;
            return 2;
        }

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, icon_url.c_str());
            // curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->icon_cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &png_file);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                std::cerr << "Open Weather Map GET request failed: "
                          << curl_easy_strerror(res) << std::endl;
                return 1;
            }

            curl_easy_cleanup(curl);
        }
        png_file.close();
    }
    std::cout << std::endl;
    return 0;
}

void OWM::parse_icon(json_object *weather)
{
    if (weather != NULL)
    {
        json_object_object_foreach(weather, key, val)
        {
            if (strncmp(key, "icon", 4) == 0)
            {
                char *icon_url = (char *)calloc(40, sizeof(char));
                memset(icon_url, '\0', 40);
                strncpy(icon_url, "http://openweathermap.org/img/w/xxx.png",
                        39);
                const char *icon = json_object_get_string(val);
                strncpy(icon_url + 32, icon, 3);
                // this->icon_file_name.clear();
                this->icon_file_name = icon;
                this->icon_file_name += ".png";
            }
        }
    }
}

void OWM::parse_temperature(json_object *temp)
{
    if (temp != NULL)
    {
        json_object_object_foreach(temp, key,
                                   val) if (strncmp(key, "temp\0", 5) == 0)
        {
            float kelvin;
            kelvin = std::stof(std::string(json_object_get_string(val)), NULL);

            if (this->celsius)
            {
                this->temperature = round(kelvin - 273.15);
            }
            else
            {
                this->temperature = round((9.0 / 5) * (kelvin - 273.15) + 32);
            }
        }
    }
}

void OWM::json_parse(const char *json_str)
{
    json_object *root = json_tokener_parse(json_str);
    json_object_object_foreach(root, key, val)
    {
        if (strncmp(key, "weather", 7) == 0)
        {
            parse_icon(json_object_array_get_idx(val, 0));
        }

        if (strncmp(key, "main", 4) == 0)
        {
            parse_temperature(val);
        }
    }
}

size_t OWM::weather_cb(char *in, uint size, uint nmemb, void *instance)
{
    size_t ret = size * nmemb;
    reinterpret_cast< OWM * >(instance)->json_parse(in);
    return ret;
}

void OWM::update_weather()
{
    if (this->city_id.length() == 0)
    {
        std::cerr << "No city ID when getting current weather data"
                  << std::endl;
        return;
    }
    if (this->api_key.length() == 0)
    {
        std::cerr << "No city API key when getting current weather data"
                  << std::endl;
        return;
    }

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        std::string url = "http://api.openweathermap.org/data/2.5/weather?id=";
        url += this->city_id + "&APPID=" + this->api_key;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, weather_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "cur openweathermap get request failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

int OWM::get_temperature() { return this->temperature; }

std::string OWM::get_weather_icon() { return this->icon_file_name; }

void OWM::set_celsius(bool celsius) { this->celsius = celsius; }
