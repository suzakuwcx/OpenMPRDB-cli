#include "Config.h"
#include <fstream>
#include <iostream>
#include <utility>

std::map<std::string, std::string> &Config::operator[](const std::string &key)
{
    return conf[key];
}

void Config::save()
{
    std::ofstream ofstream(file_path_, std::ofstream::out);

    if (!ofstream)
        throw std::runtime_error("cannot save configuration file at :" + file_path_);

    //save non_section setting
    std::map<std::string, std::string> non_section_setting = conf[""];

    for (const auto& temp:non_section_setting)
    {
        ofstream << temp.first;
        ofstream << '=';
        ofstream << temp.second;
        ofstream << '\n';
    }
    ofstream << '\n';

    //save common setting
    for (const auto& section_conf:conf)
    {
        //skip non_section setting
        if (section_conf.first.empty())
            continue;
        else
            ofstream << '[' << section_conf.first << ']' << '\n';

        for (const auto& temp : section_conf.second)
            ofstream << temp.first << '=' << temp.second << '\n';

        ofstream << '\n';
    }

}

Config::Config(std::string file_path) : isExit(true)
{
    this->file_path_ = std::move(file_path);
    analyse();
}

bool Config::isFileExit()
{
    return this->isExit;
}

std::string Config::getFilePath()
{
    return file_path_;
}

Config::Config() : isExit(false)
{
    analyse();
}

void Config::setFilePath(std::string new_path)
{
    this->file_path_ = std::move(new_path);
    analyse();
}

void Config::analyse()
{
    std::ifstream ifstream(file_path_);

    if (!ifstream)
    {
        isExit = false;
        return;
    }
    else
        isExit = true;

    std::string string_temp;
    std::string key_temp;
    std::string section_temp;

    char s = ifstream.get();
    while (ifstream)
    {
        switch (s)
        {
            case ' ':
            case '\t':
                    break;
            case '\n':
                if (!key_temp.empty())
                {
                    conf[section_temp][key_temp] = string_temp;
                    string_temp.clear();
                    key_temp.clear();
                }
                break;
            case '[':
                s = ifstream.get();
                while (ifstream && s != ']')
                {
                    string_temp.push_back(s);
                    s = ifstream.get();
                }
                if (s == ']')
                {
                    section_temp = string_temp;
                    string_temp.clear();
                }
                break;
            case '#':
                s = ifstream.get();
                while (ifstream && s != '\n')
                {
                    s = ifstream.get();
                }
                break;
            case '=':
                key_temp = string_temp;
                string_temp.clear();
                break;
            case ']':
                throw std::runtime_error("invalid conf");
            default:
                string_temp.push_back(s);
        }
        s = ifstream.get();
    }
    if (!key_temp.empty())
    {
        conf[section_temp][key_temp] = string_temp;
        string_temp.clear();
        key_temp.clear();
    }
}
