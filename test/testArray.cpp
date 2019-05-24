#include <gtest/gtest.h>

#include <BCJson/BCJson.hpp>

namespace {
    using namespace BlackCodex::BCJson;

    TEST(testBCJsonArray, EmptyArray) {
        BCJsonArray array;
        EXPECT_EQ(0, array.size());
    }
    TEST(testBCJsonArray, BasicFunctionality) {
        static const float PI = 3.14159f;
        static const double E = 2.7182818284590452353602874713527;
        static const char HUNDRED = 100;
        static const uint64_t PERFECT_POWER = 9814072356;

        BCJsonArray array;
        array.add("ItemString");
        array.add(PI);
        array.add(E);
        array.add(PERFECT_POWER);
        array.add(HUNDRED);

        ASSERT_EQ(5, array.size());
        EXPECT_EQ(BCJsonValueString, array.get(0).getType());
        EXPECT_EQ("ItemString", array.get(0).getString());
        EXPECT_EQ(BCJsonValueFloat, array.get(1).getType());
        EXPECT_EQ(PI, array.get(1).getDouble());
        EXPECT_EQ(BCJsonValueFloat, array.get(2).getType());
        EXPECT_EQ(E, array.get(2).getDouble());
        EXPECT_EQ(BCJsonValueNumber, array.get(3).getType());
        EXPECT_EQ(PERFECT_POWER, array.get(3).getNumber());
        EXPECT_EQ(BCJsonValueNumber, array.get(4).getType());
        EXPECT_EQ(HUNDRED, array.get(4).getNumber());
    }
    TEST(testBCJsonArray, SubArray) {
        BCJsonArray array;
        array.add("Item1");
        array.add(BCJsonValueArray);
        array.add("Item3");

        {
            BCJsonValue& valueEntry1 = array.get(1);
            BCJsonArray& arrayEntry1 = valueEntry1.getArray();
            arrayEntry1.add(std::string("Item2-1"));
            arrayEntry1.add(std::string("Item2-2"));
        }

        ASSERT_EQ(3, array.size());
        EXPECT_EQ(BCJsonValueString, array.get(0).getType());
        EXPECT_EQ("Item1", array.get(0).getString());
        EXPECT_EQ(BCJsonValueString, array.get(2).getType());
        EXPECT_EQ("Item3", array.get(2).getString());

        BCJsonValue& value1 = array.get(1);
        EXPECT_EQ(BCJsonValueArray, value1.getType());

        BCJsonArray& array1 = value1.getArray();
        ASSERT_EQ(2, array1.size());
        EXPECT_EQ("Item2-1", array1.get(0).getString());
        EXPECT_EQ("Item2-2", array1.get(1).getString());
    }
}
