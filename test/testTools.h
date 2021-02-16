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

#pragma once

#include <BCJson/BCJson.hpp>

#include <gtest/gtest.h>

namespace BlackCodex {
namespace BCJson {

class BCTestTools {
public:
    /**
     * \brief Checks the given object is an array and it has the expected size.
     */
    static void checkArraySize(const BlackCodex::BCJson::BCJsonValue& value, size_t expectedSize);
    /**
     * \brief Validates the given val is an array, and a matching String is contained in the value at the given index.
     */
    static void checkString(const BlackCodex::BCJson::BCJsonValue& val, size_t arrayIndex, const std::string& expected);
    /**
     * \brief Validates the given val is an object, and a matching String is contained in the value at the given key.
     */
    static void checkString(const BlackCodex::BCJson::BCJsonValue& val, const std::string& key, const std::string& expected);
    /**
     * \brief Validates the given val is an object, and a matching unsigned is contained in the value at the given index.
     */
    static void checkUnsigned(const BlackCodex::BCJson::BCJsonValue& val, size_t arrayIndex, uint64_t expected);
    /**
     * \brief Validates the given val is an object, and a matching unsigned is contained in the value at the given key.
     */
    static void checkUnsigned(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, uint64_t expected);
    /**
     * \brief Validates the given val is an object, and a matching signed is contained in the value at the given key.
     */
    static void checkSigned(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, int64_t expected);
};

} // BCJson
} // BlackCodex
