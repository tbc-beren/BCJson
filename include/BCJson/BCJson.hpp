#pragma once

#include <BCJson/BCJsonTypes.hpp>
#include <BCJson/BCJsonException.hpp>

#include <map>
#include <memory>
#include <string>
#include <vector>

namespace BlackCodex {
namespace BCJson {

class BCJsonValue
{
private:
    BCJsonValueType mType;
    std::map<std::string, BCJsonValue> mMembers;
    std::vector<BCJsonValue> mArr;
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
    void set(double f) {
        mType = BCJsonValueFloat;
        mValFloat = f;
    }
    void set(uint64_t u) {
        mType = BCJsonValueNumber;
        mValUnsigned = u;
    }
    void set(int64_t n) {
        mType = BCJsonValueNumber;
        mValSigned = n;
    }
    template<typename T> BCJsonValue& add(const T& item) {
        setType(BCJsonValueArray);
        mArr.push_back(BCJsonValue(item));
        return mArr.back();
    }
    BCJsonValue& add(BCJsonValueType type) {
        setType(BCJsonValueArray);
        mArr.push_back(BCJsonValue(type));
        return mArr.back();
    }
    BCJsonValue& get(size_t index) {
        assertType(BCJsonValueArray, false);
        return mArr[index];
    }
    const BCJsonValue& get(size_t index) const{
        assertType(BCJsonValueArray, false);
        return mArr[index];
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
    size_t getSize() const {
        assertType(BCJsonValueArray, false);
        return mArr.size();
    }
    const std::string& getString() const {
        return mValString;
    }
    const std::string& getString(size_t index) const {
        return get(index).getString();
    }
    const std::string& getString(const std::string& key) const {
        return get(key).getString();
    }
    double getDouble() const {
        return mValFloat;
    }
    double getDouble(size_t index) const {
        return get(index).getDouble();
    }
    double getDouble(const std::string& key) const {
        return get(key).getDouble();
    }
    int64_t getNumber() const {
        return mValSigned;
    }
    int64_t getNumber(size_t index) const {
        return get(index).getNumber();
    }
    int64_t getNumber(const std::string& key) const {
        return get(key).getNumber();
    }
    uint64_t getUnsigned() const {
        return mValUnsigned;
    }
    uint64_t getUnsigned(size_t index) const {
        return get(index).getUnsigned();
    }
    uint64_t getUnsigned(const std::string& key) const {
        return get(key).getUnsigned();
    }
};

} // BCJson
} // BlackCodex
