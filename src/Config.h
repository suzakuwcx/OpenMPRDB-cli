#ifndef OPENMPRDB_CLI_CONFIG_H
#define OPENMPRDB_CLI_CONFIG_H

#include <string>
#include <map>

class Config
{
public:
    typedef std::map<std::string,std::map<std::string,std::string>> Conf;
public:
    Config();
    Config(std::string file_path);
    ~Config() = default;
    std::map<std::string,std::string> &operator[](const std::string &key);
    void save();
    bool isFileExit();
    void setFilePath(std::string new_path);
    std::string getFilePath();

private:
    void analyse();

private:
    std::string kFilePath;
    Conf conf;
    bool isExit;


};


#endif //OPENMPRDB_CLI_CONFIG_H
