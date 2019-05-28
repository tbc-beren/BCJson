
#include "testTools.h"

namespace BlackCodex {
namespace BCJson {

void BCTestTools::checkArraySize(const BlackCodex::BCJson::BCJsonValue& value, size_t expectedSize) {
    ASSERT_EQ(BCJsonValueArray, value.getType());
    ASSERT_EQ(expectedSize, value.getArray().size());
}

void BCTestTools::checkString(const BlackCodex::BCJson::BCJsonArray& array, size_t arrayIndex, const std::string& expected) {
    ASSERT_LT(arrayIndex, array.size());

    const BCJsonValue& itemValue = array.get(arrayIndex);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getString());
}

void BCTestTools::checkString(const BlackCodex::BCJson::BCJsonValue&val, size_t arrayIndex, const std::string& expected) {
    ASSERT_EQ(BCJsonValueArray, val.getType());
    checkString(val.getArray(), arrayIndex, expected);
}

void BCTestTools::checkString(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, const std::string& expected) {
    ASSERT_EQ(BCJsonValueObject, val.getType());

    const BCJsonValue& itemValue = val.get(key);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getString());
}

void BCTestTools::checkUnsigned(const BlackCodex::BCJson::BCJsonArray& array, size_t arrayIndex, uint64_t expected) {
    ASSERT_LT(arrayIndex, array.size());

    const BCJsonValue& itemValue = array.get(arrayIndex);
    ASSERT_EQ(BCJsonValueNumber, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getUnsigned());
}

void BCTestTools::checkUnsigned(const BlackCodex::BCJson::BCJsonValue& val, size_t arrayIndex, uint64_t expected) {
    ASSERT_EQ(BCJsonValueArray, val.getType());
    checkUnsigned(val.getArray(), arrayIndex, expected);
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
