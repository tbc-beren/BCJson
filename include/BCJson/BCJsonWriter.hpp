/**
*
* The Black Codex Library: Chapter: BCJson - A minimal JSON implementation
*
* https://github.com/tbc-beren/BCJson
*
* Mozilla Public License Version 2.0
* https://github.com/tbc-beren/BCJson/blob/master/LICENSE
*
*/
#pragma once

#include <BCJson/BCJson.hpp>
#include <BCJson/BCJsonException.hpp>

#include <iostream>

namespace BlackCodex {
namespace BCJson {

class BCJsonWriter
{
public:
    static std::string write(const BCJsonValue& value) {
        switch (value.getType()) {
            case BCJsonValueString:     return std::string("\"") + value.getString() + std::string("\"");
            case BCJsonValueNumber:     return std::to_string(value.getNumber());
            case BCJsonValueFloat:      return std::to_string(value.getDouble());
            case BCJsonValueArray:
            {
                const size_t sz = value.getSize();
                std::string rv = std::string("[");
                for (size_t x = 0; x < sz; ++x) {
                    rv += write(value.get(x));
                    if (x + 1 != sz) {
                        rv += ",";
                    }
                }
                rv += std::string("]");
                return rv;
            }
            case BCJsonValueObject:
            {
                std::string rv = std::string("{");
                auto itBegin = value.begin();
                auto itEnd  = value.end();
                for (auto it = itBegin; it != itEnd; ++it) {
                    if (it != itBegin) {
                        rv += ",";
                    }
                    rv += "\""+it->first+"\":";
                    rv += write(it->second);
                }
                rv += std::string("}");
                return rv;
            }
            default:
                throw BCJsonException("invalid type");
        }
    }
};



} // BCJson
} // BlackCodex

