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

//! \brief Configuration file error exception.
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

//! \brief Main apllication class.
class App
{
  private:
    //! \brief Configuration data.
    Config *config;
    //! \brief True if the application is running in full screen mode.
    bool fullscreen;
    //! \brief URL of the server.
    std::string server_url;

    //! \brief The UI object for the time display.
    TimeLine *time_line;
    //! \brief The UI object for the top message line.
    MsgLine *top_msg_line;
    //! \brief The UI object for the Bottom mesage line.
    MsgLine *bottom_msg_line;

    //! \brief Apllication root window.
    Fl_Double_Window *window;

    /** \brief Calculate the maximum font size that will fit a given resolution.

        \param w Width of the drawing area.
        \param h Height of teh drawing area.
        \param text Text to fit.
        \return Font size.
    */
    int get_max_font_size(int w, int h, std::string text);

    /** \brief Callback for updating time.

        Call member function update_time() with the calling convention used
        by FLTK.

        \param cb_data Callback data.
    */
    static void static_time_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_time();
    }
    //! \brief Update the time display.
    void update_time();

    /** \brief Callback for updating the top message line.

        Call member function update_msgs() with the calling convention used
        by FLTK.

        \param cb_data Callback data.
    */
    static void static_top_msgs_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_msgs(0);
    }
    /** \brief Callback for updating the bottom message line.

        Call member function update_msgs() with the calling convention used
        by FLTK.

        \param cb_data Callback data.
    */
    static void static_bottom_msgs_callback(void *cb_data)
    {
        App *instance = reinterpret_cast< App * >(cb_data);
        instance->update_msgs(1);
    }
    /** \brief Callback for updating the a message line.

        \param lineno Line to update.
    */
    void update_msgs(unsigned char lineno);

  public:
    //! \brief Application name.
    std::string name = "Clock client";
    //! \brief Application vrsion.
    std::string version = "0.0.5";

    /** \brief Constructor.
        \param config_file_name Path to the client configuration file
    */
    App(std::string config_file_name);
    //! \brief Run the application.
    int run();
    //! \brief Destructor.
    virtual ~App();
};

#endif
