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

#include <BCJson/BCJsonTypes.hpp>

#include <stdexcept>
#include <string>

namespace BlackCodex {
namespace BCJson {

class BCJsonException : public std::runtime_error
{
public:
    BCJsonException(const char* msg)
        : runtime_error(msg)
    {}

    BCJsonException(const std::string& msg)
        : runtime_error(msg)
    {}
};

class BCJsonInvalidTypeException : public BCJsonException
{
    BCJsonValueType mType;

public:
    BCJsonInvalidTypeException(BCJsonValueType type )
        : BCJsonException("invalid json type")
        , mType(type)
    {}

    BCJsonValueType getType() const {
        return mType;
    }
};

class BCJsonInvalidKeyException : public BCJsonException
{
    std::string mKey;

public:
    BCJsonInvalidKeyException(const std::string& key)
        : BCJsonException("invalid key")
        , mKey(key)
    {}

    const std::string& getKey() const {
        return mKey;
    }
};

} // BCJson
} // BlackCodex
