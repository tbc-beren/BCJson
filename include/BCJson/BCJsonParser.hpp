#pragma once

#include <BCJson/BCJson.hpp>
#include <BCJson/BCJsonException.hpp>

#include <stack>

namespace BlackCodex {
namespace BCJson {

class BCJsonParserException : public BCJsonException {
    int mLine;
    int mColumn;
public:
    BCJsonParserException(const char* message, int line = 0, int column = 0)
        : BCJsonException(message)
        , mLine(line)
        , mColumn(column)
    {}

    BCJsonParserException(const std::string& message, int line = 0, int column = 0)
        : BCJsonException(message)
        , mLine(line)
        , mColumn(column)
    {}
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

            if (isNumber(chr) || isMinus(chr)) {
                parseNumber();
                advance();
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
                if ( cMode != MObjectEnd ) {
                    throwParseException('}');
                }
                advanceChar();
                mState.pop();
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
                        throw std::runtime_error("invalid state");
                    }
                }
                break;
            case ',':
                {
                    switch (mState.top().mMode)
                    {
                    case MObjectEnd:
                        setState(MObjectName);
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
                throw BCJsonParserException(std::string("invalid state: unexpected ")+chr);
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

    void parseNumber() {
        bool isSigned = (*mPos == '-');
        bool isFloat = false;
        const char* str = mPos;

        if (isSigned) {
            mPos++;
        }

        while (isNumber(*mPos) || *mPos=='e' || *mPos=='E' || *mPos=='.') {
            if (*mPos=='.') {
                if (isFloat) {
                    throwParseException('.');
                }
                isFloat = true;
            }
            mPos++;
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
                cur.add(str);
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
            BCJsonValue& r = newVal->add(BCJsonValueNull);
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
        throwParseException("parse error");
    }
    void throwParseException(char chr) {
        throwParseException(std::string("unexpected char ")+chr);
    }
    void throwParseException(const std::string& message) {
        throw BCJsonException(message);
    }
};



} // BCJson
} // BlackCodex

