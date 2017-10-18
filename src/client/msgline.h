#ifndef MSGLINE_H_
#define MSGLINE_H_

#include <FL/Fl_Box.H>

#include "json.hpp"

class MsgLine;

// Callback data for curl responses to line requests.
struct s_curl_callback_data
{
    MsgLine *instance;
    unsigned char lineno;
};

class MsgLine : public Fl_Box
{
  private:
    struct s_curl_callback_data msg_cb_data;
    // URL of the server.
    std::string server_url;

    std::string json_str;

    std::string text;
    std::string icon;

    static size_t msg_cb(char *in, uint size, uint nmemb, void *data);

  public:
    MsgLine(int x, int y, int w, int h, int font_size, std::string url);
    int update(unsigned char lineno);
};

#endif
