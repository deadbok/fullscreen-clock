#include "config.h"
#include <fstream>
#include <iostream>

Config::Config() {}

unsigned char Config::load(std::string file_name)
{
    this->file_name = file_name;

    std::ifstream config_file;

    config_file.open(this->file_name.c_str());

    std::string line;
    unsigned int line_no = 0;
    unsigned char ret;

    while (!config_file.eof())
    {
        {
            // Keep track of the line number for error reporting.
            line_no++;

            // Get a line from the configuration file.
            std::getline(config_file, line);

            ret = this->parse(line, line_no);
            if (ret != 0)
            {
                return ret;
            }
        }
    }
    return 0;
}

unsigned char Config::parse(std::string line, unsigned int line_no)
{

    // Get first none white space character.
    size_t key_begin = line.find_first_not_of("\t ");

    // Make sure an option name comes first.
    if (line[key_begin] == '=')
    {
        return this->error("No configuration option name", line, line_no);
    }

    // Skip line on comment at the beginning.
    if (line[key_begin] != '#')
    {
        // Test for reaching the end of the line.
        if (key_begin != std::string::npos)
        {
            size_t key_end = line.find_first_of("\t ");

            std::string key = line.substr(key_begin, key_end - key_begin);

            if (key.find("#", 0) != std::string::npos)
            {
                return this->error("Comment starts in configuration key", line,
                                   line_no);
            }

            std::cout << "Key: " << key << std::endl;
        }
    }

    return 0;
}

unsigned int Config::error(std::string msg, std::string line,
                           unsigned int line_number)
{
    std::cerr << std::endl << msg << std::endl;
    std::cerr << "Error in line " << line_number << ": " << line << std::endl;
    return 1;
}
