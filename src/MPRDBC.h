#ifndef OPENMPRDB_CLI_MPRDBC_H
#define OPENMPRDB_CLI_MPRDBC_H

#include <string>
#include <utility>
#include "Config.h"
#include "HttpConnection.h"

struct Arguments
{
    bool showHelp = false;
    std::string opera;
    std::pair<bool,std::string> key = {false,""};
    std::pair<bool,std::string> name = {false,""};
    std::pair<bool,std::string> player = {false,""};
    std::pair<bool,std::string> remote = {false,""};
    std::pair<bool,std::string> comment = {false,""};
    std::pair<bool,std::string> banList = {false,""};
    std::pair<bool,std::string> submit = {false,""};
};

class MPRDBC
{
private:
public:
    MPRDBC();
    void printHelp();
    void GenerateDefaultGlobalConfig();
    void printMissingConfig();
    void CheckKey();

    void init(int argc, char *argv[]);
    void launch();
private:
    const char * kVersion = "1.0.0";
    HttpConnection connection;
    Arguments arguments_;
    std::string home_dir_;
    Config global_config_;
    Config user_config_;
};


#endif //OPENMPRDB_CLI_MPRDBC_H
