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

namespace BlackCodex {
namespace BCJson {

enum BCJsonValueType {
    BCJsonValueNull = 0,
    BCJsonValueString,
    BCJsonValueNumber,
    BCJsonValueFloat,
    BCJsonValueObject,
    BCJsonValueArray,
    BCJsonValueTrue,
    BCJsonValueFalse
};

} // BCJson
} // BlackCodex

