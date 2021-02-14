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

#include <BCJson/BCJson.hpp>
#include <BCJson/BCJsonException.hpp>

#include <cstring>
#include <stack>

namespace BlackCodex {
namespace BCJson {

class BCJsonParserException : public BCJsonException {
    size_t mOffset;
public:
    BCJsonParserException(const char* message, size_t offset=0)
        : BCJsonException(message)
        , mOffset(offset)
    {}
    BCJsonParserException(const std::string& message, size_t offset=0)
        : BCJsonException(message)
        , mOffset(offset)
    {}
    size_t getOffset() const {
        return mOffset;
    }
};

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

public:
    BCJsonParser(const char* const str)
        : mStart(str)
        , mPos(str)
    {}

    const char* getStart() const {
        return mStart;
    }
    std::pair<int, int> getLineAndColumn(size_t offset) const {
        std::pair<int, int> ret(1, 0);
        const char* tmp = mStart;
        while(0 < offset--) {
            ret.second++;
            if ('\r' == *tmp++ ) {
                ret.first++;
                ret.second = 0;
            }
        }
        return ret;
    }
    int getLine(size_t offset) const {
        auto lineCol = getLineAndColumn(offset);
        return lineCol.first;
    }
    int getColumn(size_t offset) const {
        auto lineCol = getLineAndColumn(offset);
        return lineCol.second;
    }

    const char* getError(const BCJsonParserException& ex) const {
        const char* err = getStart()+ex.getOffset();
        const char* base = mStart;
        while(err>base && '\r' != *err) {
            err--;
        }
        return err;
    }

    void advance() {
        while( isWhitespace(*mPos) ) {
            advanceChar();
        }
    }

    BCJsonValue parse() {
        BCJsonValue root;
        advance();
        mState.emplace(MScan, &root);
        while (!mState.empty() && *mPos != 0) {

            char chr = *mPos;
            MODE cMode = mState.top().mMode;

            if (isNumber(chr) || isMinus(chr)) {
                parseNumber();
                advance();
                continue;
            } else if(parseBool()) {
                continue;
            }

            switch (chr) {
            case '{':
                if (cMode != MObjectValue && cMode != MScan && MArray != cMode) {
                    throwParseException("invalid state");
                }
                if (cMode != MArray) {
                    setState(MObjectEnd);
                }
                pushState(BCJsonValueObject, MObjectName);
                break;
            case '}':
                if ((cMode == MObjectName && mState.top().mVal->isEmpty()) ||
                     cMode == MObjectEnd) {
                    advanceChar();
                    mState.pop();
                } else {
                    throwParseException('}');
                }
                break;
            case '[':
                if (MObjectValue== cMode) {
                    setState(MObjectEnd);
                }
                pushState(BCJsonValueArray, MArray);
                break;
            case ']':
                if (cMode != MArray) {
                    throwParseException("invalid state");
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
                        mState.top().mVal->add(str);
                        break;
                    case MObjectName:
                        mState.top().mName = str;
                        setState(MObjectColon);
                        break;
                    case MObjectValue:
                        mState.top().mVal->set(mState.top().mName, str);
                        setState(MObjectEnd);
                        break;

                    default:
                        throwParseException("invalid state");
                    }
                }
                break;
            case ',':
                {
                    switch (mState.top().mMode)
                    {
                    case MObjectEnd:
                        setState(MObjectName);
                        // Fallthru
                    case MArray:
                        advanceChar();
                        break;

                    default:
                        throwParseException("invalid state");
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
                    throwParseException("invalid state");
                }
            }
            break;
            default:
                throwParseException(std::string("invalid state: unexpected ")+chr);
            }

            advance();
        }
        return root;
    }

    std::string parseString() {
        if (*mPos == '\"') {
            advanceChar();

            auto start = mPos;
            size_t replacements = 0;
            char chr;
            while ('\"' != (chr = *mPos)) {
                if (isEscapeSequence(chr)) {
                    replacements++;
                    advanceChar();
                } else if (!isAllowedInString(chr)) {
                    throwParseException(chr);
                }
                advanceChar();
            }

            advanceChar();

            std::string ret;
            ret.resize(mPos - 1 - start - replacements);
            char* tgt = &(ret[0]);
            while (mPos > start+1) {
                chr = *start;
                if (isEscapeSequence(chr)) {
                    start++;
                    chr = *start;
                    *tgt = replaceStringChar(*start);
                } else if (start != tgt) {
                    *tgt = *start;
                }

                tgt++;
                start++;
            }
            return ret;
        }
        return std::string();
    }

    bool parseBool() {
        bool isValidBool = false;
        BCJsonValue val;
        if (strncmp(mPos, "true", 4)==0) {
            val.setType(BCJsonValueTrue);
            isValidBool = true;
            mPos+=4;
        } else if (strncmp(mPos, "false", 5)==0) {
            val.setType(BCJsonValueFalse);
            isValidBool = true;
            mPos+=5;
        }

        if (isValidBool) {
            MODE cMode = mState.top().mMode;
            switch (cMode)
            {
            case MArray:
                mState.top().mVal->add(val);
                break;
            case MObjectValue:
                mState.top().mVal->set(mState.top().mName, val);
                setState(MObjectEnd);
                break;

            default:
                throwParseException("invalid state");
            }
            advance();
        }

        return isValidBool;
    }
    void parseNumber() {
        bool isSigned = (*mPos == '-');
        bool isFloat = false;
        const char* str = mPos;

        if (isSigned) {
            advanceChar();
        }

        while (isNumber(*mPos) || *mPos=='e' || *mPos=='E' || *mPos=='.') {
            if (*mPos=='.') {
                if (isFloat) {
                    throwParseException('.');
                }
                isFloat = true;
            }
            advanceChar();
        }

        BCJsonValue val;
        if (isFloat) {
            val.set(atof(str));
        } else if (isSigned) {
            val.set((int64_t)atoi(str));
        } else {
            val.set((uint64_t)atoi(str));
        }

        MODE cMode = mState.top().mMode;
        switch (cMode)
        {
        case MArray:
            mState.top().mVal->add(val);
            break;
        case MObjectValue:
            mState.top().mVal->set(mState.top().mName, val);
            setState(MObjectEnd);
            break;

        default:
            throwParseException("invalid state");
        }
    }
    static bool isEscapeSequence(char chr) {
        return chr == '\\';
    }
    static bool isMinus(char chr) {
        return chr == '-';
    }
    static bool isNumber(char chr) {
        return '0' <= chr && '9' >= chr;
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
    static char replaceStringChar(char src) {
        static const char table[] = "\"nr\\tfb";
        static const char tablr[] = "\"\n\r\\\t\f\b";
        int idx = 0;
        char chr;
        while (0 != (chr = table[idx])) {
            if (src == chr) {
                return tablr[idx];
            }
            idx++;
        }
        return 0;
    }
    void pushState(BCJsonValueType type, MODE mode) {
        BCJsonValue* newVal = mState.top().mVal;

        advanceChar();

        BCJsonValueType cType = (newVal->getType());
        
        if (BCJsonValueNull == cType) {
            mState.top().mVal->setType(type);
        } else if ( BCJsonValueArray == cType ) {
            BCJsonValue& r = newVal->add(BCJsonValueNull);
            newVal = &r;
        }  else if (BCJsonValueObject == cType) {
            BCJsonValue& r = newVal->set(mState.top().mName, BCJsonValue());
            newVal = &r;
        } else {
            throwParseException("invalid state");
        }
        newVal->setType(type);
        mState.emplace(mode, newVal);
    }

    void setState(MODE m) {
        mState.top().mMode = m;
    }

    void throwParseException() {
        throwParseException("parse error");
    }
    void throwParseException(char chr) {
        throwParseException(std::string("unexpected char ")+chr);
    }
    void throwParseException(const std::string& message) {
        throw BCJsonParserException(message, mPos-getStart());
    }
};



} // BCJson
} // BlackCodex

