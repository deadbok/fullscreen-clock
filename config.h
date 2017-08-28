#include <map>
#include <string>

class Config
{
  private:
    std::string file_name;
    std::map< std::string, std::string > options;

    void error(std::string line, unsigned int line_number);

  public:
    Config(std::string file_name);
    unsigned char parse();
};
