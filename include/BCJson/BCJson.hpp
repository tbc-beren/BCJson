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
    class iterator
    {
        BCJsonValue& mParent;
        std::map<std::string, BCJsonValue>::iterator mItMembers;
        std::vector<BCJsonValue>::iterator mItArr;

        void _seekBegin() {
            switch (mParent.getType()) {
            case BCJsonValueArray:      mItArr      = mParent.mArr.begin();     break;
            case BCJsonValueObject:     mItMembers  = mParent.mMembers.begin(); break;
            default:
                throw BCJsonInvalidTypeException(mParent.getType());
            }
        }
    public:
        iterator(BCJsonValue& parent)
            : mParent(parent)
            , mItMembers(parent.mMembers.end())
            , mItArr(parent.mArr.end())
        {}
        std::string getKey() const {
            switch (mParent.getType()) {
            case BCJsonValueObject:     return mItMembers->first;
            default:
                throw BCJsonInvalidTypeException(mParent.getType());
            }
        }
        BCJsonValue& getValue() const {
            switch (mParent.getType()) {
            case BCJsonValueObject:     return mItMembers->second;
            default:
                throw BCJsonInvalidTypeException(mParent.getType());
            }
        }
        static iterator begin(BCJsonValue& parent) {
            iterator it = iterator(parent);
            it._seekBegin();
            return it;
        }
        void operator++() {
            switch (mParent.getType()) {
            case BCJsonValueArray:  mItArr++;       break;
            case BCJsonValueObject: mItMembers++;   break;
            default:
                throw BCJsonInvalidTypeException(mParent.getType());
            }
        }
        bool operator==(const iterator& it) {
            return  it.mParent.getType() != this->mParent.getType() &&
                    it.mItArr == this->mItArr &&
                    it.mItMembers == this->mItMembers;
        }
        bool operator!=(const iterator& it) {
            return  it.mParent.getType() != this->mParent.getType() ||
                    it.mItArr != this->mItArr ||
                    it.mItMembers != this->mItMembers;
        }
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
    BCJsonValue(const char* str)
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
    BCJsonValue(bool b)
        : mType(b?BCJsonValueTrue:BCJsonValueFalse)
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

    iterator begin() {
        return iterator::begin(*this);
    }
    iterator end() {
        return iterator(*this);
    }
    void assertType(BCJsonValueType type, bool nullIsValid = true) const {
        if(( !nullIsValid && (BCJsonValueNull == mType || BCJsonValueNull == type)) ||
           ( type != mType && BCJsonValueNull != mType) ) {
            throw BCJsonInvalidTypeException(mType);
        }
    }
    bool isEmpty()  const {
        return   BCJsonValueNull   == mType ||
                (BCJsonValueObject == mType && mMembers.empty()) ||
                (BCJsonValueArray  == mType && mArr.empty());
    }
    bool has(size_t index) const {
        return getSize() > index;
    }
    bool has(const std::string& key) const {
        assertType(BCJsonValueObject, false);
        return mMembers.end() != mMembers.find(key);
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
    bool getBool() const {
        switch (mType) {
        case BCJsonValueTrue:   return true;
        case BCJsonValueFalse:  return false;
        default:
            throw BCJsonInvalidTypeException(mType);
        }
    }
    bool getBool(size_t index) const {
        return get(index).getBool();
    }
    bool getBool(const std::string& key) const {
        return get(key).getBool();
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
