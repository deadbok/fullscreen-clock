#include <FL/fl_draw.H>

#include <curl/curl.h>

#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>

#include <string>

#include "app.h"

App::App(std::string config_file_name)
{
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

    // Create the FLTK window with a dark backround.
    this->window = new Fl_Double_Window(Fl::w(), Fl::h(), "Clock");
    this->window->color(fl_rgb_color(50));

    std::cout << "Resolution: " << this->window->w() << "x" << this->window->h()
              << std::endl;

    // Find optimal font size.
    int font_size = this->get_max_font_size(
        this->window->w() - this->window->w() / 16,
        this->window->h() - this->window->h() / 16, "00:00");
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

    // Create time display.
    this->time_line =
        new TimeLine(this->window->w(), this->window->h(), font_size);
    // Create the top message display.
    this->top_msg_line =
        new MsgLine(0, this->window->h() / 2, this->window->w(),
                    this->window->h(), font_size, this->server_url);
    // Create the bottom message display.
    this->bottom_msg_line =
        new MsgLine(0, (this->window->h() / 4) * 3, this->window->w(),
                    this->window->h(), font_size, this->server_url);
}

int App::get_max_font_size(int w, int h, std::string text)
{
    // Coarse calculation.
    int width = 0, height = 0, font_size = 1;
    while ((width < w) && (height < h))
    {
        fl_font(FL_HELVETICA, font_size);
        fl_measure(text.c_str(), width, height);
        font_size += 50;
    }
    // Fine calculation.
    while ((width > w) && (height > h))
    {
        fl_font(FL_HELVETICA, font_size);
        fl_measure(text.c_str(), width, height);
        font_size--;
    }
    font_size--;
    return (font_size);
}

void App::update_time()
{
    this->time_line->update();

    Fl::repeat_timeout(1.0, this->static_time_callback, this);
}

void App::update_msgs(unsigned char lineno)
{
    // Top line always updates every 5 minutes,
    if (lineno == 0)
    {
        this->top_msg_line->update(lineno);
        Fl::repeat_timeout(300.0, this->static_top_msgs_callback, this);
    }
    // Bottom line updates are controlled by the server,
    else if (lineno == 1)
    {
        int seconds = this->bottom_msg_line->update(lineno);
        if (seconds == 0)
        {
            seconds = 60;
        }
        Fl::repeat_timeout(seconds, this->static_bottom_msgs_callback, this);
    }
}

int App::run()
{
    int ret;

    // Add timers to update the GUI elements.
    Fl::add_timeout(0.1, this->static_time_callback, this);
    Fl::add_timeout(1.1, this->static_top_msgs_callback, this);
    Fl::add_timeout(2.1, this->static_bottom_msgs_callback, this);

    // Run FLTK main loop.
    this->window->show();
    ret = Fl::run();

    return (ret);
}

App::~App()
{
    // Clean up curl.
    curl_global_cleanup();
}
