#ifndef OPENMPRDB_CLI_DATABASE_H
#define OPENMPRDB_CLI_DATABASE_H

#include "value.h"

class DataBase
{
public:
    static DataBase& getInstance();

private:
    DataBase();
    ~DataBase();

public:

    DataBase(const DataBase&) = delete;
    DataBase& operator=(const DataBase&) = delete;
};


#endif //OPENMPRDB_CLI_DATABASE_H
