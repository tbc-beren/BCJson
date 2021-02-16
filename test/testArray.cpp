/**
*
* The Black Codex Library: Chapter: BCJson - A minimal JSON implementation
*
* https://github.com/tbc-beren/BCJson
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCJson/blob/master/LICENSE
*
* This Source Code Form is subject to the terms of the Mozilla Public License, v. 2.0.
* If a copy of the MPL was not distributed with this file, You can obtain one at
* https://mozilla.org/MPL/2.0/.
*
*/
#include <BCJson/BCJson.hpp>

#include "testTools.h"

#include <gtest/gtest.h>

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
        static const int64_t PERFECT_POWER_SIGNED = 9814072356;

        BCJsonValue array;
        array.add("ItemString");
        array.add(PI);
        array.add(E);
        array.add(PERFECT_POWER);
        array.add(PERFECT_POWER_SIGNED);
        array.add(HUNDRED);

        ASSERT_EQ(BCJsonValueArray, array.getType());
        ASSERT_EQ(6UL, array.getSize());
        ASSERT_TRUE(array.has(5));
        ASSERT_FALSE(array.has(6));

        // Please note these lines test two different use cases.
        // Having both improves test coverage
        //
        // EXPECT_EQ(E, array.get(2).getDouble());  // Access to item and then value
        // EXPECT_EQ(E, array.getDouble(2));        // Direct access to item's value

        BCTestTools::checkString(array, 0, "ItemString");
        EXPECT_EQ(BCJsonValueFloat, array.get(1).getType());
        EXPECT_EQ(PI, array.get(1).getDouble());
        EXPECT_EQ(PI, array.getDouble(1));
        EXPECT_EQ(BCJsonValueFloat, array.get(2).getType());
        EXPECT_EQ(E, array.get(2).getDouble());
        EXPECT_EQ(E, array.getDouble(2));
        EXPECT_EQ(BCJsonValueNumber, array.get(3).getType());
        EXPECT_EQ(PERFECT_POWER, array.get(3).getUnsigned());
        EXPECT_EQ(PERFECT_POWER, array.getUnsigned(3));
        EXPECT_EQ(BCJsonValueNumber, array.get(4).getType());
        EXPECT_EQ(PERFECT_POWER_SIGNED, array.get(4).getUnsigned());
        EXPECT_EQ(PERFECT_POWER_SIGNED, array.getUnsigned(4));
        EXPECT_EQ(BCJsonValueNumber, array.get(5).getType());
        EXPECT_EQ(HUNDRED, array.get(5).getNumber());
        EXPECT_EQ(HUNDRED, array.getNumber(5));
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
