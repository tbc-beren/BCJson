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

