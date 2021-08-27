#include "DataBase.h"

DataBase::DataBase()
{

}

DataBase::~DataBase()
{

}

DataBase &DataBase::getInstance()
{
    static DataBase instance;
    return instance;
}
