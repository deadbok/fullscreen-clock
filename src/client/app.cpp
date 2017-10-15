#include <FL/fl_draw.H>

#include <curl/curl.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

#include "app.h"

App::App(std::string config_file_name)
{
    int font_size = 1;

    // Header
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

    // Get values from configuration file
    this->server_url = this->config->get_value("server");
    std::string fullscreen = this->config->get_value("fullscreen");
    this->fullscreen = false;

    // Creat the FLTK window.
    this->window = new Fl_Double_Window(Fl::w(), Fl::h(), "Clock");
    this->window->color(fl_rgb_color(50));

    std::cout << "Resolution: " << this->window->w() << "x" << this->window->h()
              << std::endl;

    // Calculate the maximum font size for the current resolution.
    int max_width = this->window->w() - this->window->w() / 20;
    int max_height = this->window->h() - this->window->h() / 20;
    int width = 0, height = 0;
    while ((width < max_width) && (height < max_height))
    {
        fl_font(FL_HELVETICA, font_size);
        fl_measure("00:00", width, height);
        font_size += 50;
    }
    while ((width > max_width) && (height > max_height))
    {
        fl_font(FL_HELVETICA, font_size);
        fl_measure("00:00", width, height);
        font_size--;
    }
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

    // Create a box for the time display.
    this->time_line =
        new TimeLine(this->window->w(), this->window->h(), font_size);

    this->top_msg_line =
        new MsgLine(0, this->window->h() / 2, this->window->w(),
                    this->window->h(), font_size, this->server_url);

    this->bottom_msg_line =
        new MsgLine(0, (this->window->h() / 4) * 3, this->window->w(),
                    this->window->h(), font_size, this->server_url);
}

void App::update_time()
{
    this->time_line->update();

    Fl::repeat_timeout(1.0, this->static_time_callback, NULL);
}

void App::update_msgs(unsigned char lineno)
{
    this->top_msg_line->update(lineno);

    Fl::repeat_timeout(60.0, this->static_top_msgs_callback, this);
}

int App::run()
{
    int ret;

    Fl::add_timeout(0.1, this->static_time_callback, this);

    Fl::add_timeout(0.1, this->static_top_msgs_callback, this);

    this->window->show();

    ret = Fl::run();

    return (ret);
}

App::~App() { curl_global_cleanup(); }
