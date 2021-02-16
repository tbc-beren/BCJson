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

    TEST(testBCJsonObject, BasicFunctionality) {
        static const float PI = 3.14159f;
        static const double E = 2.7182818284590452353602874713527;
        static const char HUNDRED = 100;
        static const unsigned int THOUSAND = 1000U;
        static const uint64_t PERFECT_POWER = 9814072356;

        BCJsonValue object(BCJsonValueObject);
        object.set("kStr", "ItemString");
        object.set("kFloat", PI);
        object.set("kDouble", E);
        object.set("kUint64", PERFECT_POWER);
        object.set("kChar", HUNDRED);
        object.set("kUint", THOUSAND);
        object.set("kBool", true);

        EXPECT_EQ(BCJsonValueObject, object.getType());
        ASSERT_TRUE(object.has("kStr"));
        ASSERT_FALSE(object.has("noFIeld"));

        // Please note these lines test two different use cases.
        // Having both improves test coverage
        //
        // EXPECT_EQ(E, array.get("Item").getDouble());  // Access to item and then value
        // EXPECT_EQ(E, array.getDouble("Item"));        // Direct access to item's value

        BCTestTools::checkString(object, "kStr", "ItemString");
        EXPECT_EQ(BCJsonValueFloat, object.get("kFloat").getType());
        EXPECT_EQ(PI, object.get("kFloat").getDouble());
        EXPECT_EQ(PI, object.getDouble("kFloat"));
        EXPECT_EQ(BCJsonValueFloat, object.get("kDouble").getType());
        EXPECT_EQ(E, object.get("kDouble").getDouble());
        EXPECT_EQ(E, object.getDouble("kDouble"));
        EXPECT_EQ(BCJsonValueNumber, object.get("kUint").getType());
        EXPECT_EQ(THOUSAND, object.get("kUint").getUnsigned());
        EXPECT_EQ(THOUSAND, object.getUnsigned("kUint"));
        EXPECT_EQ(BCJsonValueNumber, object.get("kUint64").getType());
        EXPECT_EQ(PERFECT_POWER, object.get("kUint64").getUnsigned());
        EXPECT_EQ(PERFECT_POWER, object.getUnsigned("kUint64"));
        EXPECT_EQ(BCJsonValueNumber, object.get("kChar").getType());
        EXPECT_EQ(HUNDRED, object.get("kChar").getNumber());
        EXPECT_EQ(HUNDRED, object.getNumber("kChar"));
        EXPECT_EQ(BCJsonValueTrue, object.get("kBool").getType());
        EXPECT_TRUE(object.get("kBool").getBool());
        EXPECT_TRUE(object.getBool("kBool"));
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
