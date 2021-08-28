#include "value.h"
#include <string>
#include <sstream>
#include <stack>
#include "shell.hpp"

#include <utility>

Value::Value(const Value::Iterator &begin, const Value::Iterator &end) : type_(kArray), array_val_(begin, end){}

Value::Value() : type_(kNull) {}

Value::~Value()
{
    CheckDestructor();
}

Value::Value(int intVal) : type_(kInt), int_val_(intVal){}

Value::Value(bool boolVal) : type_(kBool), bool_val_(boolVal){}

Value::Value(double doubleVal) : type_(kDouble), double_val_(doubleVal){}

Value::Value(const char *stringVal) : type_(kString), string_val_(stringVal){}

Value::Value(std::string stringVal) : type_(kString), string_val_(std::move(stringVal)){}

Value::Value(const Value & old_val)
{
    type_ = old_val.type_;
    switch (type_)
    {
        case kNull:
            break;
        case kBool:
            bool_val_ = old_val.bool_val_;
            break;
        case kInt:
            int_val_ = old_val.int_val_;
            break;
        case kString:
            new(&string_val_) std::string(old_val.string_val_);
            break;
        case kDouble:
            double_val_ = old_val.double_val_;
            break;
        case kArray:
            new(&array_val_) Array(old_val.array_val_);
            break;
        case kMap:
            new(&map_val_) Map();
            for (std::pair<std::string,Value> temp : old_val.map_val_)
            {
                map_val_[temp.first] = temp.second;
            }
            break;
        default:
            throw std::exception();
    }
}

Value &Value::operator=(int val)
{
    CheckDestructor();
    type_ = kInt;
    int_val_ = val;
    return *this;
}

void Value::CheckDestructor()
{
    if (type_ == kArray)
        array_val_.~Array();
    else if (type_ == kString)
        string_val_.std::string::~string();
    else if (type_ == kMap)
        map_val_.~Map();
}

Value &Value::operator=(bool val)
{
    CheckDestructor();
    type_ = kBool;
    bool_val_ = val;
    return *this;
}

Value &Value::operator=(double val)
{
    CheckDestructor();
    type_ = kDouble;
    double_val_ = val;
    return *this;
}

Value &Value::operator=(const char *val)
{
    CheckDestructor();
    new(&string_val_) std::string(val);
    type_ = kString;
    return *this;
}

Value &Value::operator=(const std::string &val)
{
    if (type_ == kString)
    {
        string_val_ = val;
    }
    else
    {
        CheckDestructor();
        new(&string_val_) std::string(val);
        type_ = kString;
    }
    return *this;
}

Value &Value::operator=(const Value::Map &val)
{
    type_ = kMap;
    map_val_ = val;
    return *this;
}

Value &Value::operator[](const std::string &key)
{
    if (type_ != kMap)
    {
        CheckDestructor();
        new(&map_val_) Map();
        type_ = kMap;
    }
    return map_val_[key];
}

Value &Value::operator[](int index)
{
    if (type_ != kArray)
    {
        CheckDestructor();
        new(&array_val_) Array(index + 1);
        type_ = kArray;
    }
    while (array_val_.size() <= index)
    {
        array_val_.resize(index + 1);
    }
    return array_val_[index];
}

Value &Value::operator=(const Value & old_val)
{
    CheckDestructor();
    type_ = old_val.type_;
    switch (type_)
    {
        case kNull:
            break;
        case kBool:
            bool_val_ = old_val.bool_val_;
            break;
        case kInt:
            int_val_ = old_val.int_val_;
            break;
        case kString:
            new(&string_val_) std::string(old_val.string_val_);
            break;
        case kDouble:
            double_val_ = old_val.double_val_;
            break;
        case kArray:
            new(&array_val_) Array(old_val.array_val_);
            for (const Value& temp : old_val.array_val_)
            {
                array_val_.emplace_back(temp);
            }
            break;
        case kMap:
            new(&map_val_) Map();
            for (std::pair<std::string,Value> temp : old_val.map_val_)
            {
                map_val_[temp.first] = temp.second;
            }
            break;
        default:
            throw std::exception();
    }
    return *this;
}

std::ostream &operator<<(std::ostream &os, const Value &object)
{
    switch (object.type_)
    {
        case kNull:
            os << "null";
            break;
        case kBool:
            if (object.bool_val_)
                os << "true";
            else
                os << "false";
            break;
        case kInt:
            os << object.int_val_;
            break;
        case kString:
            os << '\"' << object.string_val_ << '\"';
            break;
        case kDouble:
            os << object.double_val_;
            break;
        case kArray:
            os << '[';
            if (!object.array_val_.empty())
            {
                os << object.array_val_[0];
                for (int i = 1; i < object.array_val_.size(); ++i)
                {
                    os << ',' << object.array_val_[i];
                }
            }
            os << ']';
            break;
        case kMap:
            os << '{';
            auto iterator = object.map_val_.begin();
            if (iterator != object.map_val_.end())
            {
                std::pair<std::string,Value> temp = *iterator;
                os << '\"' << temp.first << '\"' << ':' << temp.second;
                ++iterator;
            }
            while (iterator != object.map_val_.end())
            {
                std::pair<std::string,Value> temp = *iterator;
                os  << ',' << '\"' << temp.first << '\"' << ':' << temp.second;
                ++iterator;
            }
            os << '}';
            break;
//        default :
//            throw std::exception();
    }
    return os;
}

UnionType Value::getType() const
{
    return type_;
}

Value &Value::operator=(const Value::Array &val)
{
    CheckDestructor();
    new(&array_val_) Array(val);
    type_ = kArray;
    return *this;
}

Value &Value::setNull()
{
    CheckDestructor();
    type_ = kNull;
    return *this;
}

std::string Value::transform() const
{
    std::ostringstream ostringstream;
    ostringstream << *this;
    std::string reply_json = ostringstream.str();

    replace_all(reply_json, "\n", "\\n");

    return reply_json;
}

Value Value::CreateValueByJson(std::string raw)
{
    std::stack<std::string> key_stack;

    std::stack<Value> array_stack;
    int array_i = 0;

    std::stack<Value> map_stack;
    //to regonize array or map when prasing ',',true is map,false is array
    std::stack<bool> bool_stack;

    std::string string_temp;
    Value value_temp;

    for (int i = 0; i < raw.size(); ++i)
    {
        char s = raw[i];
        switch (s)
        {
            case ' ':
            case '\n':
                break;
            case 't':
                //true
                if (raw.size() - i < 4)
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'r')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'u')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'e')
                    throw std::runtime_error("invalid json");
                value_temp = Value(true);
                break;
            case 'f':
                //false
                if (raw.size() - i < 5)
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'a')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'l')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 's')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'e')
                    throw std::runtime_error("invalid json");
                value_temp = Value(false);
                break;
            case 'n':
                //null
                if (raw.size() - i < 4)
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'u')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'l')
                    throw std::runtime_error("invalid json");
                ++i;
                if (raw[i] != 'l')
                    throw std::runtime_error("invalid json");
                value_temp = Value();
                break;
            case '\"':
                //string
                ++i;
                if (i >= raw.size())
                    throw std::runtime_error("invalid json");
                for (; i < raw.size(); ++i)
                {
                    s = raw[i];
                    if (s == '\"')
                        break;
                    if (s == '\\')
                    {
                        ++i;
                        if (i >= raw.size())
                            throw std::runtime_error("invalid json");
                        //escape: " \"message\" : ... "
                        if (raw[i] == '\"')
                            string_temp.push_back(raw[i]);
                        else
                        {
                            --i;
                            string_temp.push_back(raw[i]);
                        }
                    }
                    else
                        string_temp.push_back(s);
                }
                value_temp = string_temp;
                string_temp.clear();
                break;
            case '{':
                //json
                map_stack.push(Value());
                bool_stack.push(true);
                break;
            case '[':
                //array
                array_stack.push(Value());
                bool_stack.push(false);
                break;
            case ':':
            {
                std::string val_string= value_temp.transform();
                //remove ""
                key_stack.push(val_string.substr(1,val_string.size()-2));
                string_temp.clear();
                break;
            }
            case ',':
                if (bool_stack.empty())
                    throw std::runtime_error("invalid json");
                if (bool_stack.top())
                {
                    Value val = map_stack.top();
                    map_stack.pop();
                    val[key_stack.top()] = value_temp;
                    key_stack.pop();
                    map_stack.push(val);
                }
                else
                {
                    Value val = array_stack.top();
                    array_stack.pop();
                    val[array_i] = value_temp;
                    ++array_i;
                    array_stack.push(val);
                }
                break;
            case '}':
            {
                Value val = map_stack.top();
                map_stack.pop();
                val[key_stack.top()] = value_temp;
                key_stack.pop();

                value_temp = val;
                bool_stack.pop();
                break;
            }
            case ']':
            {
                Value val = array_stack.top();
                array_stack.pop();
                val[array_i] = value_temp;

                value_temp = val;
                array_i = 0;
                bool_stack.pop();
                break;
            }
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            {
                bool isDouble = false;
                for (; i < raw.size(); ++i)
                {
                    s = raw[i];
                    switch (s)
                    {
                        case '0':
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                            string_temp.push_back(s);
                            break;
                        case '.':
                            string_temp.push_back(s);
                            if (isDouble)
                                throw std::runtime_error("invalid json");
                            else
                                isDouble = true;
                            break;
                        case ',':
                        case ' ':
                        case '}':
                        case ']':
                            break;
                        default:
                            throw std::runtime_error("invalid json");
                    }
                    if (s == ',' || s == ' ' || s == '}' || s == ']')
                    {
                        if (isDouble)
                            value_temp = std::stod(string_temp);
                        else
                            value_temp = std::stoi(string_temp);
                        string_temp.clear();
                        --i;
                        break;
                    }
                }
                break;
            }
            default:
                throw std::runtime_error("invalid json");
        }
    }
    if (!key_stack.empty() || !array_stack.empty() || !map_stack.empty() || !bool_stack.empty())
        throw std::runtime_error("invalid json");
    return value_temp;
}

Value::Array Value::getArray()
{
    if (this->type_ == kArray)
        return this->array_val_;
    else
        throw std::runtime_error("invalid type");
}
