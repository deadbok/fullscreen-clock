#include "config.h"
#include <fstream>
#include <iostream>

Config::Config(std::string file_name) { this->file_name = file_name; }

unsigned char Config::parse()
{
    std::ifstream config_file;

    config_file.open(this->file_name.c_str());

    std::string line;
    unsigned int line_no = 0;

    while (!config_file.eof())
    {
      //Keep track of the line number for error reporting.
        line_no++;

        // Get a line from the configuration file.
        std::getline(config_file, line);

        //Get first none white space character.
        size_t key_begin = line.find_first_not_of("\t ");

        //Make sure an option name comes first.
        if (line[key_begin] == '=')
        {
          std::cerr << std::endl << "No configuration option name" << std::endl;
          this->error(line, line_no);
          return 1;
        }

        //Test for reaching the end of the line.
        if (key_begin != std::string::npos)
        {
          size_t key_end = line.find_first_of("\t ");

          std::string key = line.substr(key_begin, key_end - key_begin);

          std::cout << "Key: " << key << std::endl;
        }
    }

    config_file.close();

    return 0;
}

void Config::error(std::string line, unsigned int line_number)
{
    std::cerr << "Error in line " << line_number << ": " << line << std::endl;
}
