#include <exception>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>

#include "config.h"

class ConfigException : public std::exception
{
  public:
    virtual const char *what() const throw()
    {
        return "Error loading configuration file";
    }
};

class App;

struct s_callback_data
{
    App *instance;
    Fl_Widget *ui_element;
    void *data;
};

class App
{
  private:
    char time_str[7];
    char weather_str[100];
    char icon_file_name[64];

    Config *config;
    std::string server_url;
    bool fullscreen;

    Fl_Box *time_box;
    Fl_Box *weather_box;
    Fl_Double_Window *window;

    int last_minute;
    struct s_callback_data time_cb_data;

    struct s_callback_data weather_cb_data;

    // Callback for updating time.
    static void static_time_callback(void *cb_data)
    {
        (reinterpret_cast< struct s_callback_data * >(cb_data)
             ->instance->update_time(
                 reinterpret_cast< struct s_callback_data * >(cb_data)
                     ->ui_element));
    }
    void update_time(Fl_Widget *ui_element);

    void json_parse(const char *json_str);
    static size_t msg_cb(char *in, uint size, uint nmemb, void *instance);
    void get_msg(unsigned char lineno);
    static void static_msgs_callback(void *cb_data)
    {
        (reinterpret_cast< struct s_callback_data * >(cb_data)
             ->instance->update_msgs(
                 reinterpret_cast< struct s_callback_data * >(cb_data)
                     ->ui_element));
    }
    void update_msgs(Fl_Widget *ui_element);

  public:
    std::string name = "Clock";
    std::string version = "0.0.3";

    App(std::string config_file_name);
    int run();
    virtual ~App();
};
