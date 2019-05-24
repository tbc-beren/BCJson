
#include "testTools.h"

namespace BlackCodex {
namespace BCJson {

void BCTestTools::validateString(const BlackCodex::BCJson::BCJsonValue&val, size_t arrayIndex, const std::string& expected) {
    using namespace BlackCodex::BCJson;

    ASSERT_EQ(BCJsonValueArray, val.getType());

    const BCJsonArray& array = val.getArray();
    ASSERT_LT(arrayIndex, array.size());

    const BCJsonValue& itemValue = array.get(arrayIndex);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getString());
}

void BCTestTools::validateString(const BlackCodex::BCJson::BCJsonValue&val, const std::string& key, const std::string& expected) {
    using namespace BlackCodex::BCJson;

    ASSERT_EQ(BCJsonValueObject, val.getType());

    const BCJsonValue& itemValue = val.get(key);
    ASSERT_EQ(BCJsonValueString, itemValue.getType());
    ASSERT_EQ(expected, itemValue.getString());
}

} // BCJson
} // BlackCodex