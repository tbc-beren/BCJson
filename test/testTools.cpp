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
#include "testTools.h"

namespace BlackCodex {
namespace BCJson {

void BCTestTools::checkArraySize(const BlackCodex::BCJson::BCJsonValue& value, size_t expectedSize) {
    ASSERT_EQ(BCJsonValueArray, value.getType());
    ASSERT_EQ(expectedSize, value.getSize());
}

void BCTestTools::checkString(const BlackCodex::BCJson::BCJsonValue&val, size_t arrayIndex, const std::string& expected) {
    ASSERT_EQ(BCJsonValueArray, val.getType());
    ASSERT_LT(arrayIndex, val.getSize());
    const BCJsonValue& itemValue = val.get(arrayIndex);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, val.getString(arrayIndex));
}

void BCTestTools::checkString(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, const std::string& expected) {
    ASSERT_EQ(BCJsonValueObject, val.getType());
    const BCJsonValue& itemValue = val.get(key);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getString());
}

void BCTestTools::checkUnsigned(const BlackCodex::BCJson::BCJsonValue& val, size_t arrayIndex, uint64_t expected) {
    ASSERT_EQ(BCJsonValueArray, val.getType());
    ASSERT_LT(arrayIndex, val.getSize());

    const BCJsonValue& itemValue = val.get(arrayIndex);
    ASSERT_EQ(BCJsonValueNumber, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getUnsigned());
}

void BCTestTools::checkUnsigned(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, uint64_t expected) {
    ASSERT_EQ(BCJsonValueObject, val.getType());

    const BCJsonValue& itemValue = val.get(key);
    ASSERT_EQ(BCJsonValueNumber, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getUnsigned());
}

void BCTestTools::checkSigned(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, int64_t expected) {
    ASSERT_EQ(BCJsonValueObject, val.getType());

    const BCJsonValue& itemValue = val.get(key);
    ASSERT_EQ(BCJsonValueNumber, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getNumber());
}

} // BCJson
} // BlackCodex
