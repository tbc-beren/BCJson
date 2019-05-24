
#pragma once

#include <BCJson/BCJson.hpp>

#include <gtest/gtest.h>

namespace BlackCodex {
namespace BCJson {

class BCTestTools {
public:
    /**
     * \brief Validates the given val is an array, and a matching String is contained in the value at the given index.
     */
    static void validateString(const BlackCodex::BCJson::BCJsonValue&val, size_t arrayIndex, const std::string& expected);
    /**
     * \brief Validates the given val is an object, and a matching String is contained in the value at the given key.
     */
    static void validateString(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, const std::string& expected);
};

} // BCJson
} // BlackCodex
