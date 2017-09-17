#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_Widget.H>

#include "config.h"
#include "owm.h"

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
    bool celsius;
    bool fullscreen;

    Fl_Box *time_box;
    Fl_Box *weather_box;
    Fl_Double_Window *window;

    OWM *weather;

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

    static void static_weather_callback(void *cb_data)
    {
        (reinterpret_cast< struct s_callback_data * >(cb_data)
             ->instance->update_weather(
                 reinterpret_cast< struct s_callback_data * >(cb_data)
                     ->ui_element,
                 reinterpret_cast< OWM * >(
                     reinterpret_cast< struct s_callback_data * >(cb_data)
                         ->data)));
    }
    void update_weather(Fl_Widget *ui_element, OWM *weather);

  public:
    std::string name = "Clock";
    std::string version = "0.0.3";

    App(std::string config_file_name);
    int run();
    virtual ~App();
};
