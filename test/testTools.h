
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
     * \brief Validates the given array, and a matching String is contained in the value at the given index.
     */
    static void checkString(const BlackCodex::BCJson::BCJsonArray& array, size_t arrayIndex, const std::string& expected);
    /**
     * \brief Validates the given val is an array, and a matching String is contained in the value at the given index.
     */
    static void checkString(const BlackCodex::BCJson::BCJsonValue& val, size_t arrayIndex, const std::string& expected);
    /**
     * \brief Validates the given val is an object, and a matching String is contained in the value at the given key.
     */
    static void checkString(const BlackCodex::BCJson::BCJsonValue& val, const std::string& key, const std::string& expected);
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
