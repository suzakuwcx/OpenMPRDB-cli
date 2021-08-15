#include <gtest/gtest.h>
#include "value.h"
#include <iostream>

TEST(ValueConstructorTest,TestNullConstructor)
{
    Value value;
    EXPECT_EQ(value.getType(),UnionType::kNull);
    EXPECT_EQ(value.transform(),"null");
}

TEST(ValueConstructorTest,TestIntConstructor)
{
    Value value(10);
    EXPECT_EQ(value.getType(),UnionType::kInt);
    EXPECT_EQ(value.transform(),"10");

    value = Value(-10);
    EXPECT_EQ(value.transform(),"-10");
}

TEST(ValueConstructorTest,TestBoolConstructor)
{
    Value value(true);
    EXPECT_EQ(value.getType(),UnionType::kBool);
    EXPECT_EQ(value.transform(),"true");

    value = Value(false);
    EXPECT_EQ(value.transform(),"false");
}

TEST(ValueConstructorTest,TestDoubleConstructor)
{
    Value value(1.1);
    EXPECT_EQ(value.getType(),UnionType::kDouble);
    EXPECT_EQ(value.transform(),"1.1");

//    value = Value(-2.111111112313);
//    ostringstream.str("");
//    ostringstream << value;
//    EXPECT_EQ(ostringstream.str(),"-2.111111112313");
}

TEST(ValueConstructorTest,TestStringConstructor)
{
    Value value("1.1");
    EXPECT_EQ(value.getType(),UnionType::kString);
    EXPECT_EQ(value.transform(),"\"1.1\"");

    value = Value(std::string("test"));
    EXPECT_EQ(value.getType(),UnionType::kString);
    EXPECT_EQ(value.transform(),"\"test\"");
}

TEST(ValueConstructorTest,TestIteratorConstructor)
{
    std::vector<Value> temp;
    temp.emplace_back(1);
    temp.emplace_back(1.1);
    temp.emplace_back("test");

    Value value(temp.begin(),temp.end());
    EXPECT_EQ(value.getType(),UnionType::kArray);
    EXPECT_EQ(value.transform(),"[1,1.1,\"test\"]");
}

TEST(ValueOperatorTest,TestOperatorPairOverwrite)
{
    Value value;

    value["test_overwrite"] = 1;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":1}");

    value["test_overwrite"] = 2;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":2}");

    value["test_overwrite"].setNull();
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":null}");

    value["test_overwrite"] = false;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":false}");

    value["test_overwrite"] = 2.33;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":2.33}");

    value["test_overwrite"] = "debug";
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":\"debug\"}");

    std::vector<Value> test_array;
    test_array.emplace_back(1);
    test_array.emplace_back(1.1);
    test_array.emplace_back("test");
    value["test_overwrite"] = test_array;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":[1,1.1,\"test\"]}");

    Value temp;
    temp["test_json"] = 1;
    value["test_overwrite"] = temp;
    EXPECT_EQ(value.transform(),"{\"test_overwrite\":{\"test_json\":1}}");
}

TEST(ValueOperatorTest,TestOperatorIndex)
{
    Value value;

    value[5] = 10;
    EXPECT_EQ(value.transform(),"[null,null,null,null,null,10]");

    value[7] = "test";
    EXPECT_EQ(value.transform(),"[null,null,null,null,null,10,null,\"test\"]");

    value[2] = 3.1415;
    EXPECT_EQ(value.transform(),"[null,null,3.1415,null,null,10,null,\"test\"]");

    Value temp;
    temp["test_json"] = 1;
    value[0] = temp;
    EXPECT_EQ(value.transform(),"[{\"test_json\":1},null,3.1415,null,null,10,null,\"test\"]");
}

TEST(ValueFunctionTest,TestCreateValueByJson)
{
    Value value = Value::CreateValueByJson("{\"test_json\" : 1}");
    EXPECT_EQ(value.transform(),"{\"test_json\":1}");
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}

