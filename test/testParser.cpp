#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJsonParser.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    void assertInvalidJson(const std::string& txt);

    TEST(testBCJsonParser, testParseObject) {
        BCJsonParser parser(" { \"k\": \"val\" } ");
        BCJsonValue root = parser.parse();
        BCTestTools::validateString(root, "k", "val");
    }
    TEST(testBCJsonParser, testParseArray) {
        BCJsonParser parser(" \n \r \n \t  [ \"Str1\", \"Str2\" , \"\" ]  ");
        BCJsonValue root = parser.parse();
        ASSERT_EQ(BCJsonValueArray, root.getType());
        ASSERT_EQ(3, root.getArray().size());
        BCTestTools::validateString(root, 0, "Str1");
        BCTestTools::validateString(root, 1, "Str2");
        BCTestTools::validateString(root, 2, "");
    }
    TEST(testBCJsonParser, testParseSubArray) {
        BCJsonParser parser(" [ \t \r\n \"v11\" \t , [ \"v21\" , \"v22\" ] , \"v12\" ] ");
        BCJsonValue root = parser.parse();
        ASSERT_EQ(BCJsonValueArray, root.getType());
        ASSERT_EQ(3, root.getArray().size());
        BCTestTools::validateString(root, 0, "v11");

        BCJsonValue& valueInner = root.getArray().get(1);
        BCTestTools::validateString(valueInner, 0, "v21");
        BCTestTools::validateString(valueInner, 1, "v22");

        BCTestTools::validateString(root, 2, "v12");
    }
    TEST(testBCJsonParser, testInvalidJson) {
        // Invalid here ------------v
        assertInvalidJson(" { \"k\":: \"val\" } ");
        assertInvalidJson(" { \"k\":, \"val\" } ");
        assertInvalidJson(" { \"k\"\" \"val\" } ");

        // Invalid here ----------------------v
        assertInvalidJson(" { \"k\": \"val\", : } ");
        assertInvalidJson(" { \"k\": \"val\", ; } ");
        assertInvalidJson(" { \"k\": \"val\", }   ");
    }

    void assertInvalidJson(const std::string& txt) {
        bool invalid = false;
        try {
            BCJsonParser parser(txt.c_str());
            BCJsonValue root = parser.parse();
        }
        catch (const std::runtime_error&) {
            invalid = true;
        }
        ASSERT_TRUE(invalid);
    }
}