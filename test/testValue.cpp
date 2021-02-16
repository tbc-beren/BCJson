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
    TEST(BCJsonValue, testAssertTypeException) {
        bool thrown = false;
        BCJsonValue obj(BCJsonValueObject);
        try {
            obj.assertType(BCJsonValueNull, false);
        } catch (const BCJsonInvalidTypeException& e) {
            thrown = true;
            EXPECT_EQ(BCJsonValueObject, e.getType());
        }
        EXPECT_TRUE(thrown);
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
    TEST(BCJsonValue, ValueDouble) {
        static const double TEST_VALUE = 333.0;

        BCJsonValue obj;
        obj.set("ObjDouble", 2.0);

        BCJsonValue& s = obj.get("ObjDouble");
        EXPECT_EQ(BCJsonValueFloat, s.getType());

        s.set(TEST_VALUE);
        EXPECT_EQ(TEST_VALUE, s.getDouble());
    }
    TEST(BCJsonValue, testBoolType) {
        BCJsonValue objTrue(BCJsonValueTrue);
        BCJsonValue objFalse(BCJsonValueFalse);
        BCJsonValue objString("SomeString");

        ASSERT_TRUE(objTrue.getBool());
        ASSERT_FALSE(objFalse.getBool());
        EXPECT_THROW(objString.getBool(), BCJsonInvalidTypeException);
    }
}
