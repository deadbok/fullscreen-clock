#include "config.h"
#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

Config::Config() {}

unsigned char Config::load(const std::string &file_name)
{
    this->file_name = file_name;

    std::ifstream config_file;

    config_file.open(this->file_name.c_str());
    if (config_file.fail())
    {
        std::cerr << std::endl
                  << "Error opening configuration file" << std::endl;
        return 2;
    }

    this->options.clear();

    std::string line;
    size_t line_no = 0;
    unsigned char ret;

    while (!config_file.eof())
    {
        {
            // Keep track of the line number for error reporting.
            line_no++;

            // Get a line from the configuration file.
            std::getline(config_file, line);

            ret = this->parse_line(line, line_no);
            if (ret != 0)
            {
                return ret;
            }
        }
    }

    config_file.close();

    return 0;
}

unsigned char Config::parse(std::string lines)
{
    std::istringstream stream(lines);
    std::string line;
    size_t line_no = 0;
    unsigned char ret;

    this->options.clear();

    while (std::getline(stream, line))
    {
        line_no++;

        ret = this->parse_line(line, line_no);
        if (ret != 0)
        {
            return ret;
        }
    }
    return 0;
}

unsigned char Config::parse_line(std::string line, const unsigned int line_no)
{
    std::string key, value;

    line = this->trim(line);

    // Stop parsing at the sign of a comment.
    // Find any quotes.
    size_t quot_begin = line.find_first_of("\"");
    size_t quot_end = 0;
    if (quot_begin != std::string::npos)
    {
        quot_end =
            line.substr(quot_begin + 1).find_first_of("\"") + quot_begin + 1;
        if (quot_end == std::string::npos)
        {
            return this->error("Missing \" at the end of the value", line,
                               line_no);
        }
    }
    // Find comments
    size_t stmt_end = line.find_first_of("#");
    if ((stmt_end != std::string::npos) && (stmt_end >= quot_begin) &&
        (stmt_end <= quot_end))
    {
        // We're inside the value, try after.
        stmt_end = line.substr(quot_end).find_first_of("#");
        if (stmt_end != std::string::npos)
        {
            line = line.substr(0, stmt_end - 1);
        }
        else
        {

            stmt_end = line.length();
        }
    }
    else if (stmt_end != std::string::npos)
    {
        line = line.substr(0, stmt_end - 1);
    }
    else
    {

        stmt_end = line.length();
    }

    // Skip full line comments and empty lines.
    if (stmt_end != 0)
    {
        // Find the assignment operator.
        const size_t equal_pos = line.find_first_of("=");
        if (equal_pos == std::string::npos)
        {
            return this->error("No value assignment", line, line_no);
        }

        if (equal_pos == 0)
        {
            return this->error("No configuration key", line, line_no);
        }

        // Find the end of the configuration key.
        const size_t key_end = line.find_first_of("\t =");
        if (key_end == std::string::npos)
        {
            return this->error(
                "Could not find the end of the configuration key", line,
                line_no);
        }

        // Save the key.
        key = line.substr(0, key_end);

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);

        // Remove the key from the input string.
        std::string value_str =
            this->trim(line.substr(equal_pos + 1, line.length()));

        // Find the beginning of the value.
        size_t val_begin = value_str.find_first_not_of("\t ");
        if (val_begin == std::string::npos)
        {
            return this->error("No configuration value", line, line_no);
        }

        // Find the end of the value.
        size_t val_end = std::string::npos;
        // Check strings.
        if (value_str[val_begin] == '\"')
        {
            val_end = value_str.substr(val_begin + 1).find_first_of("\"") +
                      val_begin + 1;
            if (val_end == std::string::npos)
            {
                return this->error("Missing \" at the end of the value", line,
                                   line_no);
            }
            val_begin++;
            val_end--;
        }
        else
        {
            val_end = value_str.find_first_of("#\n");
            if (val_end == std::string::npos)
            {
                val_end = value_str.length();
            }
        }

        if (val_begin == val_end)
        {
            return this->error("Empty configuration value", value_str, line_no);
        }

        value = value_str.substr(val_begin, val_end);
    }

    if (key.length() == 0)
    {
        return 0;
    }

    if (value.length() == 0)
    {
        return this->error("No configuration value", line, line_no);
    }

    this->options[key] = value;

    return 0;
}

std::string Config::get_value(const std::string &key)
{
    std::map< std::string, std::string >::iterator key_it =
        this->options.find(key);

    if (key_it == this->options.end())
    {
        return ("");
    }
    return key_it->second;
}

size_t Config::size() { return this->options.size(); }

std::map< std::string, std::string > Config::values() { return this->options; }

unsigned int Config::error(std::string msg, std::string line,
                           unsigned int line_number)
{
    std::cerr << std::endl << msg << std::endl;
    std::cerr << "Error in line " << line_number << ": " << line << std::endl;
    return 1;
}

void Config::clear() { this->options.clear(); }

std::string Config::trim(const std::string &str, const std::string &whitespace)
{
    const size_t str_begin = str.find_first_not_of(whitespace);
    if (str_begin == std::string::npos)
    {
        return "";
    }

    const size_t str_end = str.find_last_not_of(whitespace);
    const size_t str_range = str_end - str_begin + 1;

    return str.substr(str_begin, str_range);
}
