#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <Fl/Fl_Button.h>
#include <Fl/Fl_PNG_Image.H>
#include <curl/curl.h>
#include <json/json.h>
#include <microhttpd.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "config.h"
#include "owm.h"

#define PORT 8888

std::string prog_name = "Clock";
std::string prog_version = "0.0.3";

char time_str[7];
char weather_str[100];
char icon_file_name[64];

struct weather_cb_data
{
    OWM *weather;
    Fl_Box *ui_element;
};

void update_time(void *ui_element)
{
    time_t current_time;
    tm time_buffer;
    tm *timeinfo;

    time(&current_time);
    timeinfo = localtime_r(&current_time, &time_buffer);

    Fl_Box *box = reinterpret_cast< Fl_Box * >(ui_element);

    snprintf(time_str, 6, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
    time_str[5] = '\0';

    box->label(time_str);

    Fl::repeat_timeout(10.0, update_time, ui_element);
}

void update_weather(void *weather_cb_data)
{
    OWM *weather =
        reinterpret_cast< struct weather_cb_data * >(weather_cb_data)->weather;
    Fl_Box *box = reinterpret_cast< struct weather_cb_data * >(weather_cb_data)
                      ->ui_element;

    weather->update_weather();
    sprintf(weather_str, "%2d \u00B0C", weather->get_temperature());
    box->label(weather_str);

    sprintf(icon_file_name, "/var/tmp/%s", weather->get_weather_icon().c_str());
    Fl_PNG_Image *icon = new Fl_PNG_Image(icon_file_name);

    Fl_Image *scaled_icon = icon->copy(Fl::h() / 8, Fl::h() / 8);

    box->image(scaled_icon);

    Fl::repeat_timeout(1800.0, update_weather);
};

int main(int argc, char **argv)
{
    int ret = 0;
    std::string fullscreen = "no";
    struct weather_cb_data weather_data;

    std::cout << prog_name << " version " << prog_version << std::endl;

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
    std::string celsius = config.get_value("owm_celsius");

    OWM weather = OWM(city_id, api_key, false);
    if (celsius == "yes")
    {
        weather.set_celsius(true);
    }

    // Creat the FLTK window.
    Fl_Window *win = new Fl_Window(Fl::w(), Fl::h(), "Clock");
    win->color(fl_rgb_color(50));

    std::cout << "Resolution: " << Fl::w() << "x" << Fl::h() << std::endl;

    // Set to fullscreen if told to.
    fullscreen = config.get_value("fullscreen");
    if (fullscreen != "")
    {
        if ((fullscreen == "yes") || (fullscreen == "yes"))
        {
            win->fullscreen();
            std::cout << "Fulscreen mode enabled" << std::endl;
        }
    }

    Fl_Box *time_disp = new Fl_Box(0, 0, Fl::w(), Fl::h() / 2);
    time_disp->labelcolor(fl_rgb_color(255));
    time_disp->labelsize(int(Fl::h() / 1.75));
    time_disp->label("00:00");

    Fl_Box *temp_disp = new Fl_Box(0, Fl::h() / 2, Fl::w(), Fl::h() / 4);
    temp_disp->labelcolor(fl_rgb_color(255));
    temp_disp->labelsize(int(Fl::h() / 8));
    temp_disp->align(FL_ALIGN_INSIDE);
    temp_disp->label("20 \u00B0C");

    Fl::add_timeout(0.1, update_time, (void *)time_disp);

    weather_data.weather = &weather;
    weather_data.ui_element = temp_disp;

    Fl::add_timeout(0.1, update_weather, static_cast< void * >(&weather_data));

    win->show();

    ret = Fl::run();

    curl_global_cleanup();

    return (ret);
}
