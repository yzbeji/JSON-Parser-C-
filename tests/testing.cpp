#include <gtest/gtest.h>
#include <json.h>
#include <argument.h>
#include <fstream>

TEST(JsonClass, AssertNestedValuesFromRawStrings) {     

    std::string example = R"({
                                  "a": {
                                    "b": [
                                      2,
                                      3,
                                      { "c": "test" },
                                      [ 11, 12 ]
                                    ]
                                  }
                                })";
    std::string value;
    ASSERT_NO_THROW(yzbeji::json j(example));    
    yzbeji::json j(example);
    ASSERT_NO_THROW(
        value = (std::string)j["a"]["b"][1];
        ASSERT_EQ(value, "3");
    );
    ASSERT_NO_THROW(
        value = (std::string)j["a"]["b"][2]["c"];
        ASSERT_EQ(value, "test");
    );
    ASSERT_NO_THROW(
        value = (std::string)j["a"]["b"];
        ASSERT_EQ(value, "[2,3,{c:test},[11,12]]");
    );
    ASSERT_NO_THROW(
        (int)j["a"]["b"][0];
        ASSERT_EQ((int)j["a"]["b"][0], 2);
    );
    ASSERT_NO_THROW(
        (double)j["a"]["b"][0];
    ASSERT_EQ((double)j["a"]["b"][0], 2.0);
    );
}

TEST(JsonClass, AssertNestedValuesFromFiles)
{   
    std::ifstream fileThatDoesNotExist(R"(.\jsonfiles\fifth-test.json)", std::ios::binary);   
    std::ifstream fileThatDoesExist(R"(.\jsonfiles\fourth-test.json)", std::ios::binary); /* .\jsonfiles\fourth-test.json */
    yzbeji::json* j = nullptr;
    EXPECT_ANY_THROW(j = new yzbeji::json(fileThatDoesNotExist));
    ASSERT_NO_THROW(
        if (j != nullptr) 
            delete j ; 
        j = new yzbeji::json(fileThatDoesExist)
    );
    EXPECT_NO_THROW(
        EXPECT_EQ((std::string)(*j)["web-app"]["servlet"][0]["servlet-name"][0], "cofaxCDS");
    );
    if (j != nullptr)
        delete j;
}   

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv); 
    return RUN_ALL_TESTS(); 
}