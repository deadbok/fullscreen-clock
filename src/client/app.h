#ifndef APP_H_
#define APP_H_

#include <exception>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Widget.H>

#include "json.hpp"

#include "config.h"
#include "msgline.h"
#include "timeline.h"

// Configuration file error exception.
class ConfigException : public std::exception
{
  public:
    virtual const char *what() const throw()
    {
        return "Error loading configuration file";
    }
};

// Forward decalration of the application class.
class App;

class App
{
  private:
    // Configuration data.
    Config *config;
    // Use fullscreen?
    bool fullscreen;
    // URL of the server.
    std::string server_url;

    TimeLine *time_line;
    MsgLine *top_msg_line;
    MsgLine *bottom_msg_line;

    Fl_Double_Window *window;

    int get_max_font_size(int w, int h, std::string text);

    // Callback for updating time.
    static void static_time_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_time();
    }
    void update_time();

    void json_parse(const char *json_str, unsigned char lineno);
    static size_t msg_cb(char *in, uint size, uint nmemb, void *instance);
    void get_msg(unsigned char lineno);
    static void static_top_msgs_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_msgs(0);
    }
    static void static_bottom_msgs_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_msgs(1);
    }
    void update_msgs(unsigned char lineno);

  public:
    std::string name = "Clock";
    std::string version = "0.0.3";

    App(std::string config_file_name);
    int run();
    virtual ~App();
};

#endif
