#include <exception>

#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Double_Window.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_Widget.H>

#include "config.h"

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

// Callback data for FLTK timeouts.
struct s_fltk_callback_data
{
    App *instance;
    void *ui_element;
    void *data;
};

// Callback data for curl responses to line requests.
struct s_curl_callback_data
{
    App *instance;
    unsigned char lineno;
};

class App
{
  private:
    // Textual content of the lines in the UI.
    std::string lines[3];
    // Icons for the bottom two lines.
    std::string icons[2];

    // Configuration data.
    Config *config;
    // URL of the server.
    std::string server_url;
    // Use fullscreen?
    bool fullscreen;

    Fl_Box *time_box;
    Fl_Box *msg_box[2];
    Fl_Double_Window *window;

    int last_minute;
    struct s_fltk_callback_data ui_time_cb_data;
    struct s_fltk_callback_data ui_msg_cb_data;

    struct s_curl_callback_data curl_msg_cb_data;

    // Callback for updating time.
    static void static_time_callback(void *cb_data)
    {
        (reinterpret_cast< struct s_fltk_callback_data * >(cb_data)
             ->instance->update_time(reinterpret_cast< Fl_Box * >(
                 reinterpret_cast< struct s_fltk_callback_data * >(cb_data)
                     ->ui_element)));
    }
    void update_time(Fl_Box *ui_element);

    void json_parse(const char *json_str, unsigned char lineno);
    static size_t msg_cb(char *in, uint size, uint nmemb, void *instance);
    void get_msg(unsigned char lineno);
    static void static_msgs_callback(void *cb_data)
    {
        (reinterpret_cast< struct s_fltk_callback_data * >(cb_data)
             ->instance->update_msgs(reinterpret_cast< Fl_Box ** >(
                 reinterpret_cast< struct s_fltk_callback_data * >(cb_data)
                     ->ui_element)));
    }
    void update_msgs(Fl_Box *ui_element[]);

  public:
    std::string name = "Clock";
    std::string version = "0.0.3";

    App(std::string config_file_name);
    int run();
    virtual ~App();
};
