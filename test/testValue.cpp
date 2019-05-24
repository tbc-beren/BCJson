#include <gtest/gtest.h>

#include <BCJson/BCJson.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    TEST(BCJsonValue, DefaultValue) {
        BCJsonObject obj;
        BCJsonValue& s = obj.get("ObjString");
        EXPECT_EQ(BCJsonValueNull, s.getType());
    }
    TEST(BCJsonValue, GenericString) {
        BCJsonObject obj;
        obj.add("ObjString", "Value1");

        BCJsonValue& s = obj.get("ObjString");
        EXPECT_EQ(BCJsonValueString, s.getType());
        EXPECT_EQ("Value1", s.getString());
    }
    TEST(BCJsonValue, EmptyValue) {
        BCJsonObject obj;
        obj.add("ObjFloat", 1.0f);

        BCJsonValue& s = obj.get("ObjString");
        EXPECT_EQ(BCJsonValueNull, s.getType());

        BCJsonValue& f = obj.get("ObjFloat");
        EXPECT_EQ(BCJsonValueFloat, f.getType());
    }
}
