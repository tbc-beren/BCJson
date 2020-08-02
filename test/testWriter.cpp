#include <BCJson/BCJsonParser.hpp>
#include <BCJson/BCJsonWriter.hpp>

#include "testTools.h"

#include <gtest/gtest.h>

#include <iostream>

namespace {
    using namespace BlackCodex::BCJson;

    TEST(testBCJsonWriter, testWriteString) {
        BCJsonValue root("string");
        std::string str = BCJsonWriter::write(root);
        ASSERT_EQ("string", root.getString());
    }
    TEST(testBCJsonWriter, testWriteArray) {
        BCJsonValue root(BCJsonValueArray);
        root.add("str");
        root.add(1);
        root.add(-1);
        root.add(0.01);
        root.add(-0.01);
        root.add("str2");
        root.add(true);
        root.add(false);
        std::string result = BCJsonWriter::write(root);
        EXPECT_EQ("[\"str\",1,-1,0.010000,-0.010000,\"str2\",true,false]", result);
    }
    TEST(testBCJsonWriter, testWriteObject) {
        BCJsonValue root(BCJsonValueObject);
        root.set("i1", "str");
        root.set("i2", 1);
        root.set("i3", -1);
        root.set("i4", 0.01);
        root.set("i5", -0.01);
        root.set("i6", "str2");
        root.set("i7", true);
        root.set("i8", false);
        std::string result = BCJsonWriter::write(root);
        EXPECT_EQ("{\"i1\":\"str\",\"i2\":1,\"i3\":-1,\"i4\":0.010000,\"i5\":-0.010000,\"i6\":\"str2\","
            "\"i7\":true,"
            "\"i8\":false"
            "}", result);
    }
}
