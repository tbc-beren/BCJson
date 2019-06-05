#include <gtest/gtest.h>

#include "testTools.h"

#include <BCJson/BCJson.hpp>

namespace {
    using namespace BlackCodex::BCJson;
    TEST(testBCJsonArray, EmptyArray) {
        BCJsonValue array(BCJsonValueArray);
        EXPECT_EQ(0UL, array.getSize());
    }
    TEST(testBCJsonArray, BasicFunctionality) {
        static const float PI = 3.14159f;
        static const double E = 2.7182818284590452353602874713527;
        static const char HUNDRED = 100;
        static const uint64_t PERFECT_POWER = 9814072356;

        BCJsonValue array;
        array.add("ItemString");
        array.add(PI);
        array.add(E);
        array.add(PERFECT_POWER);
        array.add(HUNDRED);

        ASSERT_EQ(BCJsonValueArray, array.getType());
        ASSERT_EQ(5UL, array.getSize());
        ASSERT_TRUE(array.has(4));
        ASSERT_FALSE(array.has(5));

        BCTestTools::checkString(array, 0, "ItemString");
        EXPECT_EQ(BCJsonValueFloat, array.get(1).getType());
        EXPECT_EQ(PI, array.get(1).getDouble());
        EXPECT_EQ(BCJsonValueFloat, array.get(2).getType());
        EXPECT_EQ(E, array.get(2).getDouble());
        EXPECT_EQ(BCJsonValueNumber, array.get(3).getType());
        EXPECT_EQ(PERFECT_POWER, array.get(3).getUnsigned());
        EXPECT_EQ(BCJsonValueNumber, array.get(4).getType());
        EXPECT_EQ(HUNDRED, array.get(4).getNumber());
    }
    TEST(testBCJsonArray, SubArray) {
        BCJsonValue array;
        array.add("Item1");
        array.add(BCJsonValueArray);
        array.add("Item3");

        {
            BCJsonValue& valueEntry1 = array.get(1);
            valueEntry1.add(std::string("Item2-1"));
            valueEntry1.add(std::string("Item2-2"));
        }

        ASSERT_EQ(3UL, array.getSize());
        BCTestTools::checkString(array, 0, "Item1");
        BCTestTools::checkString(array, 2, "Item3");

        BCJsonValue& value1 = array.get(1);
        EXPECT_EQ(BCJsonValueArray, value1.getType());

        ASSERT_EQ(2UL, value1.getSize());
        BCTestTools::checkString(value1, 0, "Item2-1");
        BCTestTools::checkString(value1, 1, "Item2-2");
    }
}
