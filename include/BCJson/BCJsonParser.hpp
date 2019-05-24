#pragma once

#include <BCJson/BCJson.hpp>
#include <BCJson/BCJsonException.hpp>

#include <stack>

namespace BlackCodex {
namespace BCJson {

class BCJsonParser
{
    enum MODE {
        MScan,
        MObjectName,
        MObjectColon,
        MObjectValue,
        MObjectEnd,
        MArray,
        MString
    };

    class PState {
    public:
        MODE mMode;
        BCJsonValue* mVal;
        std::string mName;

        PState(MODE m, BCJsonValue* v)
            : mMode(m)
            , mVal(v)
        {}
    };

    const char* mStart;
    const char* mPos;
    std::stack<PState> mState;

    //std::vector<PError> mErrorList;

public:
    BCJsonParser(const char* const str)
        : mStart(str)
        , mPos(str)
    {}

    void advance() {
        while( isWhitespace(*mPos) ) {
            advanceChar();
        }
    }

    void advanceOne() {
        if (*mPos != 0) {
            advanceChar();
            advance();
        }
    }

    BCJsonValue parse() {
        BCJsonValue root;
        advance();
        mState.emplace(MScan, &root);
        while (!mState.empty() && *mPos != 0) {

            char chr = *mPos;
            MODE cMode = mState.top().mMode;
            switch (chr) {
            case '{':
                pushState(BCJsonValueObject, MObjectName);
                break;
            case '}':
                advanceChar();
                mState.pop();
                setState(MObjectEnd);
                break;
            case '[':
                pushState(BCJsonValueArray, MArray);
                break;
            case ']':
                if (mState.top().mMode != MArray) {
                    throw std::runtime_error("invalid state");
                }
                mState.pop();
                advanceChar();
                break;
            case '\"':
                {
                    std::string str = parseString();
                    switch (cMode)
                    {
                    case MArray:
                        mState.top().mVal->getArray().add(str);
                        break;
                    case MObjectName:
                        mState.top().mName = str;
                        setState(MObjectColon);
                        break;
                    case MObjectValue:
                        mState.top().mVal->set(mState.top().mName, str);
                        break;

                    default:
                        throw std::runtime_error("invalid state");
                    }
                }
                break;
            case ',':
                {
                    switch (mState.top().mMode)
                    {
                    case MObjectEnd:
                    case MArray:
                        advanceChar();
                        break;

                    default:
                        throw std::runtime_error("invalid state");
                    }
                }
                break;
            case ':':
            {
                switch (mState.top().mMode)
                {
                case MObjectColon:
                    advanceChar();
                    setState(MObjectValue);
                    break;
                default:
                    throw std::runtime_error("invalid state");
                }
            }
            break;
            default:
                throw std::runtime_error("invalid state");
            }

            advance();
        }
        return root;
    }

    std::string parseString() {
        if (*mPos == '\"') {
            mPos++;

            auto start = mPos;
            while (*mPos != '\"') {
                if (!isAllowedInString(*mPos)) {
                    throwParseException();
                    break;
                }
                advanceChar();
            }

            mPos++;
            return std::string(start, mPos - 1 - start);
        }
        return std::string();
    }

    static bool isMinus(char chr) {
        return chr == '-';
    }
    static bool isWhitespace(char chr) {
        return chr == ' ' || chr == '\t' || chr == '\n' || chr == '\r';
    }
    static bool isAllowedInString(char chr) {
        return chr != '\r' && chr != '\n' && chr != 0;
    }

private:
    void advanceChar() {
        mPos++;
    }

    void parseArray(BCJsonValue& cur) {
        char chr;
        do {
            chr = *mPos;
            switch (chr) {
            case '{':   advanceChar();  break;
            case '[':   advanceChar();  cur.setType(BCJsonValueArray);      break;
            case '\"':
            {
                std::string str = parseString();
                cur.getArray().add(str);
            }
            break;
            default:
                throwParseException();
                break;
            }
            advance();
            chr = *mPos;
            if (chr == ',') {
                advanceOne();
            }
        } while (chr == ',');

        if (chr == ']') {
            advanceOne();
            mState.pop();
        }
        else {
            throwParseException();
        }
    }

    void pushState(BCJsonValueType type, MODE mode) {
        BCJsonValue* newVal = mState.top().mVal;

        advanceChar();

        BCJsonValueType cType = (newVal->getType());
        
        if (BCJsonValueNull == cType) {
            mState.top().mVal->setType(type);
        } else if ( BCJsonValueArray == cType ) {
            BCJsonValue& r = newVal->getArray().add(BCJsonValueNull);
            newVal = &r;
        }  else if (BCJsonValueObject == cType) {
            BCJsonValue& r = newVal->set(mState.top().mName, BCJsonValue());
            newVal = &r;
        } else {
            throw std::runtime_error("invalid state");
        }
        newVal->setType(type);
        mState.emplace(mode, newVal);
    }

    void setState(MODE m) {
        mState.top().mMode = m;
    }

    void throwParseException() {
        throw BCJsonException("parse error");
    }
};



} // BCJson
} // BlackCodex
