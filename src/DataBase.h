#ifndef OPENMPRDB_CLI_DATABASE_H
#define OPENMPRDB_CLI_DATABASE_H

#include "value.h"

class DataBase
{
public:
    DataBase() = default;
    ~DataBase() = default;

    const std::string &getFilePath() const;

    void setFilePath(const std::string &kFilePath);

    std::string getAllRecord();
    void addRecord(Value value);
    void deleteRecord(std::string uuid);
    void save();

private:
    std::string file_path_;
    Value::Array data_;

};


#endif //OPENMPRDB_CLI_DATABASE_H
