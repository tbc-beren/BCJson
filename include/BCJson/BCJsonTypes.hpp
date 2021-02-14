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

