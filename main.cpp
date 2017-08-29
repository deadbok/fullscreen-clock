#include <FL/Fl.h>
#include <FL/Fl_Box.h>
#include <FL/Fl_Window.h>
#include <Fl/Fl_PNG_Image.h>
#include <curl/curl.h>
#include <json/json.h>
#include <microhttpd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "config.h"

#define PORT 8888

using namespace std;

string prog_name = "Clock";
string prog_version = "0.0.2";

char time_str[7];
char weather_str[100];
map< string, vector< unsigned char > > icons;

void update_time(void *ui_element)
{
    time_t current_time;
    tm time_buffer;
    tm *timeinfo;

    time(&current_time);
    timeinfo = localtime_r(&current_time, &time_buffer);

    Fl_Box *box = static_cast< Fl_Box * >(ui_element);

    snprintf(time_str, 6, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    time_str[5] = '\0';

    printf("%d:%d\n", timeinfo->tm_hour, timeinfo->tm_min);
    printf("%s\n", time_str);

    box->label(time_str);

    Fl::repeat_timeout(10.0, update_time, ui_element);
}

size_t weather_icon_cb(char *in, uint size, uint nmemb,
                       vector< unsigned char > &icon_buffer)
{
    size_t ret = 0;

    for (unsigned int i = 0; i < nmemb; i++)
    {
        for (unsigned int j = 0; j < size; j++)
        {
            icon_buffer.push_back(in[i * nmemb + j]);
            ret++;
        }
    }

    return ret;
}

int weather_get_icons(void)
{
    CURL *curl;
    CURLcode res;
    string icons_ids[] = {"01d", "01n", "02d", "02n", "03d", "03n",
                          "04d", "04n", "09d", "09n", "10d", "10n",
                          "11d", "11n", "13d", "13n", "50d", "50n"};

    cout << "Downloading weather icons";

    for (unsigned int i; i < 10; i++)
    {
        cout << ".";

        string icon_url = "http://openweathermap.org/img/w/xxx.png";
        icon_url.replace(32, 3, icons_ids[i]);

        vector< unsigned char > icon_buffer;

        curl = curl_easy_init();
        if (curl)
        {
            curl_easy_setopt(curl, CURLOPT_URL, icon_url.c_str());
            // curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, weather_icon_cb);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, icon_buffer);

            res = curl_easy_perform(curl);
            if (res != CURLE_OK)
            {
                cerr << "Open Weather Map GET request failed: "
                     << curl_easy_strerror(res) << endl;
                return 1;
            }

            curl_easy_cleanup(curl);
        }

        icons.insert(std::pair< string, vector< unsigned char > >(icons_ids[i],
                                                                  icon_buffer));

        cout << endl;
    }
    return 0;
}

void weather_parse_icon(json_object *weather)
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
                printf("Icon URL: %s\n", icon_url);
            }
        }
    }
}

void weather_parse_temperature(json_object *temp)
{
    if (temp != NULL)
    {
        json_object_object_foreach(temp, key, val)
        {
            printf("key: %s, ", key);
            printf("value: %s\n", json_object_get_string(val));
            if (strncmp(key, "temp", 4) == 0)
            {
                printf("Temperature\n");
            }
        }
    }
}

void weather_json_parse(const char *json_str)
{
    json_object *root = json_tokener_parse(json_str);
    json_object_object_foreach(root, key, val)
    {
        if (strncmp(key, "weather", 7) == 0)
        {
            weather_parse_icon(json_object_array_get_idx(val, 0));
        }

        if (strncmp(key, "main", 4) == 0)
        {
            weather_parse_temperature(val);
        }
    }
}

size_t weather_cb(char *in, uint size, uint nmemb)
{
    size_t ret = size * nmemb;
    weather_json_parse(in);
    return ret;
}

char *get_weather(string city_id, string api_key)
{
    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        string url = "http://api.openweathermap.org/data/2.5/weather?id=";
        url += city_id + "?id=2615876&APPID=" + api_key;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, weather_cb);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "cur openweathermap get request failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
    return (NULL);
}

void update_weather(void *ui_element)
{
    get_weather("", "");
    Fl::repeat_timeout(1800.0, update_weather, ui_element);
}

int main(int argc, char **argv)
{
    int ret = 0;
    int c;
    bool fullscreen = FALSE;

    cout << prog_name << "version " << prog_version << endl;

    Config config = Config(".clockrc");

    if (config.parse() != 0)
    {
        exit(1);
    }

    while ((c = getopt(argc, argv, "f")) != -1)
    {
        switch (c)
        {
        case 'f':
            cout << "Enabling fulscreen mode" << endl;
            fullscreen = TRUE;
            break;
        default:
            printf("Unknown option: %c\n", c);
        }
    }
    if (optind < argc)
    {

        while (optind < argc)
        {
            printf("Unknown option: %s\n", argv[optind++]);
        }
    }

    curl_global_init(CURL_GLOBAL_ALL);
    weather_get_icons();
    /*Fl_Window *win = new Fl_Window(Fl::w(), Fl::h(), "Clock");
    win->color(fl_rgb_color(0));
    if (fullscreen)
    {
        win->fullscreen();
    }

    Fl_Box *time_disp = new Fl_Box(0, 0, Fl::w(), Fl::h() / 1.5);
    time_disp->color(fl_rgb_color(150));
    time_disp->labelcolor(fl_rgb_color(255));
    time_disp->labelsize(int(Fl::h() / 2));
    time_disp->label("00:00");
    Fl::add_timeout(0.1, update_time, (void *)time_disp);
    Fl::add_timeout(0.1, update_weather, (void *)NULL);
    win->show();
    ret = Fl::run();*/
    curl_global_cleanup();
    return (ret);
}
