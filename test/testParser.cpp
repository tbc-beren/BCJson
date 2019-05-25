#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJsonParser.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    void assertInvalidJson(const std::string& txt);

    TEST(testBCJsonParser, testParseObject) {
        BCJsonParser parser(" { \"k\": \"val\" } ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkString(root, "k", "val");
    }
    TEST(testBCJsonParser, testParseArray) {
        BCJsonParser parser(" \n \r \n \t  [ \"Str1\", \"Str2\" , \"\" ]  ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkArraySize(root, 3);       // Validates root is an array containing 3 elements
        BCTestTools::checkString(root, 0, "Str1");  // Validates root's element 0 is "Str1"
        BCTestTools::checkString(root, 1, "Str2");  // Validates root's element 1 is "Str2"
        BCTestTools::checkString(root, 2, "");      // Validates root's element 2 is ""
    }
    TEST(testBCJsonParser, testParseSubArray) {
        BCJsonParser parser(" [ \t \r\n \"v11\" \t , [ \"v21\" , \"v22\" ] , \"v12\" ] ");
        BCJsonValue root = parser.parse();
        ASSERT_EQ(BCJsonValueArray, root.getType());
        BCTestTools::checkArraySize(root, 3);
        BCTestTools::checkString(root, 0, "v11");

        BCJsonValue& valueInner = root.getArray().get(1);
        BCTestTools::checkString(valueInner, 0, "v21");
        BCTestTools::checkString(valueInner, 1, "v22");

        BCTestTools::checkString(root, 2, "v12");
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