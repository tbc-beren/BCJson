#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJson.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    TEST(testBCJsonObject, BasicFunctionality) {
        static const float PI = 3.14159f;
        static const double E = 2.7182818284590452353602874713527;
        static const char HUNDRED = 100;
        static const uint64_t PERFECT_POWER = 9814072356;

        BCJsonValue object(BCJsonValueObject);
        object.set("kStr", "ItemString");
        object.set("kFloat", PI);
        object.set("kDouble", E);
        object.set("kChar", PERFECT_POWER);
        object.set("kUint", HUNDRED);

        EXPECT_EQ(BCJsonValueObject, object.getType());
        ASSERT_TRUE(object.has("kStr"));
        ASSERT_FALSE(object.has("noFIeld"));

        BCTestTools::checkString(object, "kStr", "ItemString");
        EXPECT_EQ(BCJsonValueFloat, object.get("kFloat").getType());
        EXPECT_EQ(PI, object.get("kFloat").getDouble());
        EXPECT_EQ(BCJsonValueFloat, object.get("kDouble").getType());
        EXPECT_EQ(E, object.get("kDouble").getDouble());
        EXPECT_EQ(BCJsonValueNumber, object.get("kChar").getType());
        EXPECT_EQ(PERFECT_POWER, object.get("kChar").getUnsigned());
        EXPECT_EQ(BCJsonValueNumber, object.get("kUint").getType());
        EXPECT_EQ(HUNDRED, object.get("kUint").getNumber());
    }
    TEST(testBCJsonObject, keyDoesNotExist) {
        BCJsonValue object;
        object.set("key", "SomeString");
        BCTestTools::checkString(object, "key", "SomeString");

        bool thrown = false;
        try {
            object.getString("KeyNotFound");
        } catch (const BCJsonInvalidKeyException& e) {
            thrown = true;
            ASSERT_EQ("KeyNotFound", e.getKey());
        }
        EXPECT_TRUE(thrown);
    }
}
