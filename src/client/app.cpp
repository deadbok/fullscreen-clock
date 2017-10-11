#include <FL/Fl_PNG_Image.H>
#include <FL/fl_draw.H>

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

#include "json.hpp"

#include "app.h"

App::App(std::string config_file_name)
{
    int font_size = 1;

    std::cout << this->name << " version " << this->version << std::endl;
    std::cout << "Configuration file: " << config_file_name << std::endl;

    // Load configuration.
    this->config = new Config();
    if (config->load(config_file_name) != 0)
    {
        throw ConfigException();
    }

    // Init curl.
    curl_global_init(CURL_GLOBAL_ALL);

    this->server_url = this->config->get_value("server");
    std::string fullscreen = this->config->get_value("fullscreen");
    this->fullscreen = false;

    // Creat the FLTK window.
    this->window = new Fl_Double_Window(Fl::w(), Fl::h(), "Clock");
    this->window->color(fl_rgb_color(50));

    std::cout << "Resolution: " << this->window->w() << "x" << this->window->h()
              << std::endl;

    int max_width = this->window->w() - this->window->w() / 20;
    int max_height = this->window->h() - this->window->h() / 20;
    int width = 0, height = 0;
    while ((width < max_width) && (height < max_height))
    {
        fl_font(FL_HELVETICA, font_size);
        fl_measure("00:00", width, height);
        font_size++;
    }
    font_size--;
    std::cout << "Font size: " << font_size << std::endl;

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

    this->time_box = new Fl_Box(0, 0, this->window->w(), this->window->h() / 2);
    this->time_box->labelcolor(fl_rgb_color(255));
    this->time_box->labelsize(font_size);
    this->time_box->label("");

    this->weather_box = new Fl_Box(0, this->window->h() / 2, this->window->w(),
                                   this->window->h() / 4);
    weather_box->labelcolor(fl_rgb_color(255));
    weather_box->labelsize(int(font_size / 4));
    weather_box->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
    weather_box->label("");
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

void App::json_parse(const char *json_str, unsigned char lineno)
{
    nlohmann::json msg_json = nlohmann::json::parse(json_str);
    std::cout << "Line: ";
    std::cout << int(lineno) << std::endl;

    for (nlohmann::json::iterator root_iter = msg_json.begin();
         root_iter != msg_json.end(); ++root_iter)
    {
        auto entry = root_iter.value();
        std::cout << entry << "\n";
        for (nlohmann::json::iterator entry_iter = entry.begin();
             entry_iter != entry.end(); ++entry_iter)
        {
            std::cout << entry_iter.key() << ": " << entry_iter.value() << "\n";

            if (entry_iter.key() == "text")
            {
                this->lines[lineno + 1] = entry_iter.value();
            }
        }
    }
}

size_t App::msg_cb(char *in, uint size, uint nmemb, void *data)
{
    size_t ret = size * nmemb;
    struct s_curl_callback_data *s_data =
        reinterpret_cast< struct s_curl_callback_data * >(data);

    s_data->instance->json_parse(in, s_data->lineno);
    return ret;
}

void App::get_msg(unsigned char lineno)
{
    if (lineno < 0)
    {
        std::cerr << "Invalid message line: " << lineno << std::endl;
        return;
    }
    if (lineno > 1)
    {
        std::cerr << "Invalid message line: " << lineno << std::endl;
        return;
    }

    CURL *curl;
    CURLcode res;
    curl = curl_easy_init();
    this->msg_cb_data.instance = this;
    this->msg_cb_data.lineno = lineno;
    if (curl)
    {
        std::string url = this->server_url;
        url += "/api/line/";
        url += std::to_string(lineno);

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, this->msg_cb);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &this->msg_cb_data);
        res = curl_easy_perform(curl);
        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl openweathermap get request failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}

void App::update_msgs(Fl_Widget *ui_element)
{
    // weather->update_weather();
    // sprintf(weather_str, "%2d \u00B0C", weather->get_temperature());
    // ui_element->label(weather_str);
    //
    // sprintf(icon_file_name, "/var/tmp/%s",
    // weather->get_weather_icon().c_str());
    // Fl_PNG_Image *icon = new Fl_PNG_Image(icon_file_name);
    //
    // Fl_Image *scaled_icon = icon->copy(Fl::h() / 8, Fl::h() / 8);
    //
    // ui_element->image(scaled_icon);
    this->get_msg(0);
    this->get_msg(1);

    ui_element->label(this->lines[1].c_str());

    Fl::repeat_timeout(5.0, this->static_msgs_callback,
                       static_cast< void * >(&this->weather_cb_data));
}

int App::run()
{
    int ret;

    this->time_cb_data.instance = this;
    this->time_cb_data.ui_element = this->time_box;
    this->time_cb_data.data = NULL;
    Fl::add_timeout(0.01, this->static_time_callback,
                    static_cast< void * >(&this->time_cb_data));

    this->weather_cb_data.instance = this;
    this->weather_cb_data.ui_element = this->weather_box;
    this->weather_cb_data.data = NULL;
    Fl::add_timeout(0.1, this->static_msgs_callback,
                    static_cast< void * >(&this->weather_cb_data));

    this->window->show();

    ret = Fl::run();

    return (ret);
}

App::~App() { curl_global_cleanup(); }
