#include <map>
#include <string>
#include <vector>

#include <json/json.h>

class OWM
{
  private:
    std::string city_id;
    std::string api_key;
    bool celsius;
    static std::string icons_ids[18];

    int temperature;
    std::string icon_file_name;

    static size_t icon_cb(char *in, uint size, uint nmemb, void *png_file);
    void parse_icon(json_object *weather);
    void parse_temperature(json_object *temp);
    void json_parse(const char *json_str);
    static size_t weather_cb(char *in, uint size, uint nmemb, void *instance);

  public:
    OWM(std::string &city_id, std::string &api_key, bool celsius);
    int get_icons();
    void update_weather();
    int get_temperature();
    std::string get_weather_icon();
    void set_celsius(bool celsius);
};
