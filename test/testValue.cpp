#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJson.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    TEST(BCJsonValue, testAssertType) {
        BCJsonValue obj;

        // Validates nullIsAlsoValid
        EXPECT_NO_THROW(obj.assertType(BCJsonValueArray));
        EXPECT_THROW(obj.assertType(BCJsonValueArray, false), BCJsonInvalidTypeException);

        BCJsonValue obj2(BCJsonValueObject);
        ASSERT_EQ(BCJsonValueObject, obj2.getType());
        EXPECT_NO_THROW(obj2.assertType(BCJsonValueObject, false));
        EXPECT_THROW(obj2.assertType(BCJsonValueNull, false), BCJsonInvalidTypeException);
        EXPECT_THROW(obj2.assertType(BCJsonValueArray, false), BCJsonInvalidTypeException);
    }
    TEST(BCJsonValue, DefaultValue) {
        BCJsonValue obj;
        BCJsonValue& s = obj.get("ObjString");
        EXPECT_EQ(BCJsonValueNull, s.getType());
    }
    TEST(BCJsonValue, GenericString) {
        BCJsonValue obj;
        obj.set("ObjString", "Value1");

        BCTestTools::checkString(obj, "ObjString", "Value1");
    }
    TEST(BCJsonValue, EmptyValue) {
        BCJsonValue obj;
        obj.set("ObjFloat", 1.0f);

        BCJsonValue& s = obj.get("ObjString");
        EXPECT_EQ(BCJsonValueNull, s.getType());

        BCJsonValue& f = obj.get("ObjFloat");
        EXPECT_EQ(BCJsonValueFloat, f.getType());
    }
}
