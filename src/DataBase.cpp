#include <fstream>
#include "DataBase.h"

const std::string &DataBase::getFilePath() const
{
    return file_path_;
}

void DataBase::setFilePath(const std::string &kFilePath)
{
    DataBase::file_path_ = kFilePath;

    std::ifstream ifstream(file_path_);
    std::string temp;

    if (!ifstream)
        return;

    while (!ifstream.eof())
    {
        getline(ifstream,temp);
    }
    Value value = Value::CreateValueByJson(temp);
    data_ = value["record"].getArray();
}

std::string DataBase::getAllRecord()
{
    std::string reply;

    for (Value value:data_)
    {
        reply += "uuid: " + value["uuid"].transform() + '\n';
        reply += "timestamp: " + value["timestamp"].transform() + '\n';
        reply += "player_uuid: " + value["player_uuid"].transform() + '\n';
        reply += "points: " + value["points"].transform() + '\n';
        reply += "comment: " + value["comment"].transform() + '\n';
        reply += '\n';
    }

    return reply;
}

void DataBase::save()
{
    Value value;
    value["record"] = data_;

    std::ofstream ofstream(file_path_);
    if (!ofstream)
        throw std::runtime_error("cannot save record");

    ofstream << value;
}

void DataBase::addRecord(Value value)
{
    data_.push_back(value);
    save();
}

void DataBase::deleteRecord(std::string uuid)
{
    uuid = '\"' + uuid + '\"';
    auto iterator = data_.begin();
    if (iterator->transform() == uuid)
        data_.erase(iterator);
    save();
}
