#include <map>
#include <string>

class Config
{
  private:
    std::string file_name;
    std::map< std::string, std::string > options;

    unsigned int error(std::string msg, std::string line,
                       unsigned int line_number);

  public:
    Config();
    unsigned char load(std::string file_name);
    unsigned char parse(std::string line, unsigned int line_no);
};
