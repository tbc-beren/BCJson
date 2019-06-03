#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJsonParser.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    void assertInvalidJson(const std::string& txt);

    TEST(testBCJsonParser, testParseObjectString) {
        BCJsonParser parser(" { \"k\": \"val\" } ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkString(root, "k", "val");
    }
    TEST(testBCJsonParser, testParseObjectInteger) {
        BCJsonParser parser(" { \"int\": 65535 } ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkUnsigned(root, "int", 65535);
    }
    TEST(testBCJsonParser, testParseSubObject) {
        BCJsonParser parser(" { \"k\": { \"k2\":\"val\" } } ");
        BCJsonValue root = parser.parse();

        ASSERT_EQ(BCJsonValueObject, root.getType());

        BCJsonValue& innerK = root.get("k");
        assert(BCJsonValueObject == innerK.getType());
        BCTestTools::checkString(innerK, "k2", "val");  // Validates root's element "k2" is "val"
    }
    TEST(testBCJsonParser, testParseArrayComplex) {
        BCJsonParser parser(" [ \"str\" , 65535 , { \"key\": 2 } , [ 1 , 2 , 3 ] ] ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkArraySize(root, 4);           // Validates root is an array containing 4 elements
        BCTestTools::checkString(root, 0, "str");       // Validates root's element 0 is "Str1"
        BCTestTools::checkUnsigned(root, 1, 65535);     // Validates root's element 1 is 65535

        BCJsonValue& item2 = root.get(2);
        ASSERT_EQ(BCJsonValueObject, item2.getType());  // Item2 is an Object
        BCTestTools::checkUnsigned(item2, "key", 2);    // Validates root's element "key" is 2

        BCJsonValue& item3 = root.get(3);
        ASSERT_EQ(BCJsonValueArray, item3.getType());   // Item3 is an Array
        BCTestTools::checkUnsigned(item3, 0, 1);        // Validates item's element 0 is 1
        BCTestTools::checkUnsigned(item3, 1, 2);        // Validates item's element 1 is 2
        BCTestTools::checkUnsigned(item3, 2, 3);        // Validates item's element 2 is 3
    }
    TEST(testBCJsonParser, testParseArrayEnding) {
        BCJsonParser parser(" { \"arr\" : [ 0 , 1 ] , \"k\": \"val\" } ");
        BCJsonValue root = parser.parse();
        BCTestTools::checkArraySize(root.get("arr"), 2);
        BCTestTools::checkUnsigned(root.get("arr"), 0, 0);
        BCTestTools::checkUnsigned(root.get("arr"), 1, 1);
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
        BCJsonParser parser(" [ \t \r\n \"v11\" \t , [ \"v21\" , 12345 , \"v22\" ] , \"v12\" ] ");
        BCJsonValue root = parser.parse();
        ASSERT_EQ(BCJsonValueArray, root.getType());
        BCTestTools::checkArraySize(root, 3);
        BCTestTools::checkString(root, 0, "v11");

        BCJsonValue& valueInner = root.get(1);
        BCTestTools::checkString(valueInner, 0, "v21");
        BCTestTools::checkUnsigned(valueInner, 1, 12345);
        BCTestTools::checkString(valueInner, 2, "v22");

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