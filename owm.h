#include <map>
#include <string>
#include <vector>

#include <json/json.h>

class OWM
{
  private:
    std::string city_id;
    std::string api_key;
    static std::string icons_ids[18];

    static size_t icon_cb(char *in, uint size, uint nmemb, void *png_file);
    void parse_icon(json_object *weather);
    void parse_temperature(json_object *temp);
    void json_parse(const char *json_str);
    static size_t weather_cb(char *in, uint size, uint nmemb, void *instance);

  public:
    OWM(std::string &city_id, std::string &api_key);
    int get_icons();
    void get_weather();
};
