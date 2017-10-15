#ifndef CONFIG_H_
#define CONFIG_H_

#include <map>
#include <string>

class Config
{
  private:
    std::string file_name;
    std::map< std::string, std::string > options;

    unsigned int error(std::string msg, std::string line,
                       unsigned int line_number);
    std::string trim(const std::string &str,
                     const std::string &whitespace = " \t");

  public:
    Config();
    unsigned char load(const std::string &file_name);
    unsigned char parse(std::string lines);
    unsigned char parse_line(std::string line, const unsigned int line_no);
    std::string get_value(const std::string &key);
    size_t size();
    std::map< std::string, std::string > values();
    void clear();
};

#endif
