#pragma once

#include <BCJson/BCJsonTypes.hpp>
#include <BCJson/BCJsonException.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace BlackCodex {
namespace BCJson {

class BCJsonArray;

class BCJsonValue
{
private:
    BCJsonValueType mType;
    std::map<std::string, BCJsonValue> mMembers;
    std::shared_ptr<BCJsonArray> mArr;
    std::string mValString;
    union {
        uint64_t    mValUnsigned;
        int64_t     mValSigned;
        double      mValFloat;
    };
public:
    BCJsonValue()
        : mType(BCJsonValueNull)
        {}
    BCJsonValue(BCJsonValueType type)
        : mType(type)
        {}
    BCJsonValue(const std::string& str)
        : mType(BCJsonValueString)
        , mValString(str)
        {}
    BCJsonValue(float f)
        : mType(BCJsonValueFloat)
        , mValFloat(f)
        {}
    BCJsonValue(double f)
        : mType(BCJsonValueFloat)
        , mValFloat(f)
        {}
    BCJsonValue(int i)
        : mType(BCJsonValueNumber)
        , mValUnsigned(i)
        {}
    BCJsonValue(int64_t i)
        : mType(BCJsonValueNumber)
        , mValSigned(i)
        {}
    BCJsonValue(uint64_t i)
        : mType(BCJsonValueNumber)
        , mValUnsigned(i)
        {}

    void assertType(BCJsonValueType type, bool nullIsValid = true) const {
        if(( !nullIsValid && (BCJsonValueNull == mType || BCJsonValueNull == type)) ||
           ( type != mType && BCJsonValueNull != mType) ) {
            throw BCJsonInvalidTypeException(mType);
        }
    }
    void setType(BCJsonValueType type) {
        assertType(type, true);
        mType = type;
    }
    template<typename T> BCJsonValue& set(const std::string& key, const T& value) {
        setType(BCJsonValueObject);
        mMembers[key] = BCJsonValue(value);
        return mMembers[key];
    }
    BCJsonValue& get(const std::string& key) {
        return mMembers[key];
    }
    const BCJsonValue& get(const std::string& key) const {
        assertType(BCJsonValueObject, false);
        const std::map<std::string, BCJsonValue>::const_iterator it = mMembers.find(key);
        if (mMembers.end() == it) {
            throw BCJsonInvalidKeyException(key);
        }
        return it->second;
    }
    BCJsonValueType getType() const {
        return mType;
    }
    const std::string& getString() const {
        return mValString;
    }
    double getDouble() const {
        return mValFloat;
    }
    int64_t getNumber() const {
        return mValSigned;
    }
    uint64_t getUnsigned() const {
        return mValUnsigned;
    }
    BCJsonArray& getArray() {
        if (BCJsonValueArray == getType() && nullptr == mArr) {
            mArr = std::make_unique<BCJsonArray>();
        }
        return *mArr;
    }
    const BCJsonArray& getArray() const {
        if (BCJsonValueArray != getType() || !mArr) {
            throw BCJsonInvalidTypeException(getType());
        }
        return *mArr;
    }
};

class BCJsonArray
{
    std::vector<BCJsonValue> mArray;

public:
    template<typename T> BCJsonValue& add(const T& item) {
        mArray.push_back(BCJsonValue(item));
        return mArray[mArray.size()-1];
    }
    BCJsonValue& add(BCJsonValueType type) {
        mArray.push_back(BCJsonValue(type));
        return mArray[mArray.size()-1];
    }
    size_t size() const {
        return mArray.size();
    }
    const BCJsonValue& get(size_t index) const {
        return mArray[index];
    }
    BCJsonValue& get(size_t index) {
        return mArray[index];
    }
};

} // BCJson
} // BlackCodex
