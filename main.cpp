#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <Fl/Fl_PNG_Image.H>
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
#include "owm.h"

#define PORT 8888

std::string prog_name = "Clock";
std::string prog_version = "0.0.2";

char time_str[7];
char weather_str[100];

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

void update_weather(void *weather)
{
    reinterpret_cast< OWM * >(weather)->get_weather();
    Fl::repeat_timeout(1800.0, update_weather);
};

int main(int argc, char **argv)
{
    int ret = 0;
    std::string fullscreen = "no";

    std::cout << prog_name << "version " << prog_version << std::endl;

    // Load configuration.
    Config config = Config();
    if (config.load(".clockrc") != 0)
    {
        std::cerr << "Error loading configuration file" << std::endl;
        exit(1);
    }

    // Init curl.
    curl_global_init(CURL_GLOBAL_ALL);

    std::string city_id = config.get_value("owm_city_id");
    std::string api_key = config.get_value("owm_api_key");

    OWM weather = OWM(city_id, api_key);

    // Creat the FLTK window.
    Fl_Window *win = new Fl_Window(Fl::w(), Fl::h(), "Clock");
    win->color(fl_rgb_color(0));

    // Set to fullscreen if told to.
    fullscreen = config.get_value("fullscreen");
    if (fullscreen != "")
    {
        if ((fullscreen == "yes") || (fullscreen == "yes"))
        {
            win->fullscreen();
        }
    }

    Fl_Box *time_disp = new Fl_Box(0, 0, Fl::w(), Fl::h() / 1.5);
    time_disp->color(fl_rgb_color(150));
    time_disp->labelcolor(fl_rgb_color(255));
    time_disp->labelsize(int(Fl::h() / 2));
    time_disp->label("00:00");

    Fl::add_timeout(0.1, update_time, (void *)time_disp);

    Fl::add_timeout(0.1, update_weather, (void *)&weather);

    win->show();

    ret = Fl::run();

    curl_global_cleanup();

    return (ret);
}
