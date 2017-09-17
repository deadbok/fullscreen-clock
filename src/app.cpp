#include <Fl/Fl_PNG_Image.H>
#include <curl/curl.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <time.h>

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "app.h"

App::App(std::string config_file_name)
{
    std::cout << this->name << " version " << this->version << std::endl;

    // Load configuration.
    this->config = new Config();
    if (config->load(config_file_name) != 0)
    {
        std::cerr << "Error loading configuration file" << std::endl;
        return;
    }

    // Init curl.
    curl_global_init(CURL_GLOBAL_ALL);

    std::string city_id = this->config->get_value("owm_city_id");
    std::string api_key = this->config->get_value("owm_api_key");
    std::string celsius = this->config->get_value("owm_celsius");
    this->celsius = false;
    std::string fullscreen = this->config->get_value("fullscreen");
    this->fullscreen = false;

    this->weather = new OWM(city_id, api_key, false);

    if (celsius == "yes")
    {
        this->celsius = true;
        weather->set_celsius(true);
    }

    // Creat the FLTK window.
    this->window = new Fl_Double_Window(Fl::w(), Fl::h(), "Clock");
    this->window->color(fl_rgb_color(50));

    std::cout << "Resolution: " << Fl::w() << "x" << Fl::h() << std::endl;

    // Set to fullscreen if told to.
    if (fullscreen != "")
    {
        if ((fullscreen == "yes") || (fullscreen == "yes"))
        {
            this->fullscreen = true;
            this->window->fullscreen();
            std::cout << "Fulscreen mode enabled" << std::endl;
        }
    }

    this->time_box = new Fl_Box(0, 0, Fl::w(), Fl::h() / 2);
    this->time_box->labelcolor(fl_rgb_color(255));
    this->time_box->labelsize(int(Fl::h() / 1.75));
    this->time_box->label("00:00");

    this->weather_box = new Fl_Box(0, Fl::h() / 2, Fl::w(), Fl::h() / 4);
    weather_box->labelcolor(fl_rgb_color(255));
    weather_box->labelsize(int(Fl::h() / 8));
    weather_box->align(FL_ALIGN_INSIDE);
    weather_box->label("20 \u00B0C");
}

void App::update_time(Fl_Widget *ui_element)
{
    time_t current_time;
    tm time_buffer;
    tm *timeinfo;

    time(&current_time);
    timeinfo = localtime_r(&current_time, &time_buffer);

    if (this->last_minute != timeinfo->tm_min)
    {
        snprintf(time_str, 6, "%02d:%02d", timeinfo->tm_hour, timeinfo->tm_min);
        time_str[5] = '\0';

        ui_element->label(time_str);
    }

    this->last_minute = timeinfo->tm_min;

    Fl::repeat_timeout(1.0, this->static_time_callback,
                       static_cast< void * >(&this->time_cb_data));
}

void App::update_weather(Fl_Widget *ui_element, OWM *weather)
{
    weather->update_weather();
    sprintf(weather_str, "%2d \u00B0C", weather->get_temperature());
    ui_element->label(weather_str);

    sprintf(icon_file_name, "/var/tmp/%s", weather->get_weather_icon().c_str());
    Fl_PNG_Image *icon = new Fl_PNG_Image(icon_file_name);

    Fl_Image *scaled_icon = icon->copy(Fl::h() / 8, Fl::h() / 8);

    ui_element->image(scaled_icon);

    Fl::repeat_timeout(900.0, static_weather_callback,
                       static_cast< void * >(&this->weather_cb_data));
}

int App::run()
{
    int ret;

    this->time_cb_data.instance = this;
    this->time_cb_data.ui_element = this->time_box;
    this->time_cb_data.data = NULL;
    Fl::add_timeout(0.1, this->static_time_callback,
                    static_cast< void * >(&this->time_cb_data));

    this->weather_cb_data.instance = this;
    this->weather_cb_data.ui_element = this->weather_box;
    this->weather_cb_data.data = this->weather;
    Fl::add_timeout(0.1, this->static_weather_callback,
                    static_cast< void * >(&this->weather_cb_data));

    this->window->show();

    ret = Fl::run();

    return (ret);
}

App::~App() { curl_global_cleanup(); }
