#include <FL/Fl_PNG_Image.H>

#include <curl/curl.h>

#include "msgline.h"

MsgLine::MsgLine(int x, int y, int w, int h, int font_size, std::string url)
    : Fl_Box(x, y, w, h / 4)
{
    this->labelcolor(fl_rgb_color(255));
    this->labelsize(font_size / 4);
    this->align(FL_ALIGN_IMAGE_NEXT_TO_TEXT);
    this->label("");
    this->text = "";
    this->icon = "";
    this->server_url = url;
}

size_t MsgLine::msg_cb(char *in, uint size, uint nmemb, void *data)
{
    size_t ret = size * nmemb;
    MsgLine *instance =
        reinterpret_cast< struct s_curl_callback_data * >(data)->instance;

    nlohmann::json msg_json = nlohmann::json::parse(in);

    for (nlohmann::json::iterator root_iter = msg_json.begin();
         root_iter != msg_json.end(); ++root_iter)
    {

        auto line = root_iter.value();

        if (line["text"].type() != nlohmann::json::value_t::null)
        {
            instance->text = line["text"];
            instance->label(instance->text.c_str());

            if (line["icon"].type() != nlohmann::json::value_t::null)
            {
                instance->icon = line["icon"];

                Fl_PNG_Image *fl_icon =
                    new Fl_PNG_Image(instance->icon.c_str());

                instance->image(fl_icon);
            }
        }
    }

    return ret;
}

void MsgLine::update(unsigned char lineno)
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
            fprintf(stderr, "Message request failed: %s\n",
                    curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }
}
