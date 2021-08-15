#ifndef OPENMPRDB_CLI_VALUE_H
#define OPENMPRDB_CLI_VALUE_H

#include <string>
#include <map>
#include <vector>
#include <ostream>

enum UnionType
{
    kNull = 0,
    kBool,
    kInt,
    kDouble,
    kString,
    kArray,
    kMap
};

class Value
{
public:
    typedef std::vector<Value> Array;
    typedef std::map<std::string, Value> Map;
    typedef std::vector<Value>::iterator Iterator;
private:
    union {
        int int_val_;
        bool bool_val_;
        double double_val_;
        std::string string_val_;
        Array array_val_;
        Map map_val_;
    };
    UnionType type_;
public:
//========================================================================
    Value();
    ~Value();
    Value(const Value&);
    explicit Value(int intVal);
    explicit Value(bool boolVal);
    explicit Value(double doubleVal);
    explicit Value(const char *stringVal);
    explicit Value(std::string stringVal);
    Value(const Iterator &begin, const Iterator &end);
//======================================================================
    static Value CreateValueByJson(std::string);
//==============================================================
    UnionType getType() const;
    friend std::ostream &operator<<(std::ostream &os, const Value &object);
//=============================================================
    //Map operator: value["index"] = 1 ==> {"index"=1}
    Value &operator[](const std::string &key);
    //array operator: value[1] = "index" ==> [null,"index"]
    Value &operator[](int index);
//========================================================
    Value &operator=(const Value&);
    Value &operator=(int val);
    Value &operator=(bool val);
    Value &operator=(double val);
    Value &operator=(const char *val);
    Value &operator=(const std::string& val);
    Value &operator=(const Array &val);
    Value &operator=(const Map &val);
    Value &setNull();
//====================================================
    //return json
    std::string transform() const;
private:
    //destructor the class when changing union type
    void CheckDestructor();
    static Value CreateValue(std::string& raw, UnionType type);
};

#endif //OPENMPRDB_CLI_VALUE_H
