#ifndef OPENMPRDB_CLI_GPGTOOL_H
#define OPENMPRDB_CLI_GPGTOOL_H

#include <string>

class GPGTool
{
public:
    explicit GPGTool(const char * key_id);

    virtual ~GPGTool();
    //get public key
    std::string getKey();
    //return sign message by private key
    std::string sign(std::string message);

private:
    std::string key_id_;

};


#endif //OPENMPRDB_CLI_GPGTOOL_H
