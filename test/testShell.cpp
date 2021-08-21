#include <gtest/gtest.h>
#include "shell.hpp"

TEST(ShellTest,TestEscapeCharacter)
{
    std::string test_1;
    test_1 = test_1 + "{" + "\"message\":" + "\"-----BEGIN PGP SIGNED MESSAGE-----\n\"}";
    replace_all(test_1, "\n", "\\n");
    replace_all(test_1, " ", "\\ ");
    EXPECT_STREQ(test_1.c_str(),"{\"message\":\"-----BEGIN\\ PGP\\ SIGNED\\ MESSAGE-----\\n\"}");
}

int main()
{
    ::testing::InitGoogleTest();
    return RUN_ALL_TESTS();
}