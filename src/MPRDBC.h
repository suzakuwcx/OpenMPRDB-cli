#ifndef OPENMPRDB_CLI_MPRDBC_H
#define OPENMPRDB_CLI_MPRDBC_H

#include <string>
#include <utility>

class MPRDBC
{
private:
public:
    MPRDBC();
    void printHelp();
    bool registerServer();
    bool newSubmit();
    bool DeleteServer();
    bool deleteSubmit();
    bool getAllServers();
    bool getSubmitDetailsByUUID(std::string submit_uuid);
    bool getSubmitDetailsByServerUUID(std::string server_uuid);
    bool getSubmitDetailsByServerKey(std::string key);




};


#endif //OPENMPRDB_CLI_MPRDBC_H
