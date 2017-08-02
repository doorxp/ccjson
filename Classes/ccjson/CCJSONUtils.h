//
//  CCJSONUtils.h
//  CCJSONTest
//
//  Created by doorxp on 12-10-23.
//
//

#ifndef CCJSONTest_CCJSONUtils_h
#define CCJSONTest_CCJSONUtils_h

#include "CCJSONMarcos.h"
#include "cocos2d.h"
#include <string>

const static std::string notChars(" \n\r\t\0",5);
CCJSON_INLINE jsstring& ltrim(jsstring& str)
{
    if (str.empty())
    {
        return str;
    }
    
    jsstring::size_type pos = str.find_first_not_of(notChars);
    if(pos == jsstring::npos)
    {
        return str;
    }
    else
    {
        str.erase(0,pos);
    }
    
    return str;
}

CCJSON_INLINE jsstring& rtrim(jsstring& str)
{
    if (str.empty())
    {
        return str;
    }
    
    jsstring::size_type pos = str.find_last_not_of(notChars);
    if(pos == jsstring::npos)
    {
        return str;
    }
    else
    {
        str.erase(pos+1);
        return str;
    }
}

CCJSON_INLINE jsstring& trim(jsstring& str)
{
    return ltrim(rtrim(str));
}

/*
 
 //*/
CCJSON_INLINE bool evenSlash(jsstring::const_iterator bb, jsstring::const_iterator p)
{
    int slash = 0;
    
    while(p >= bb && *(p--) == '\\')
    {
        slash++;
    }
    return !(slash&0x01);
}

#define NULLCASE()\
    case '\0':\
        return jsstring::npos;\
    break;

#define QUOTECASE()\
    case '\"':\
        while (*(++p) != '\"' || !evenSlash(beginChars, p-1)){\
            if(*p == '\0' || p == str.end()) return jsstring::npos;\
        }\
        break;

#define BRACKET(left, right)\
    case left: {\
        size_t brac = 1;\
        while (brac){\
            CCASSERT(p != str.end() && p + 1 != str.end(), "out");\
            switch (*(++p)){\
                case right:\
                    if(evenSlash(beginChars, p-1))--brac;\
                break;\
                case left:\
                    if(evenSlash(beginChars, p-1))++brac;\
                break;\
                QUOTECASE()\
                NULLCASE()\
            }\
        }\
        break;}\
        case right:\
            return jsstring::npos;

CCJSON_INLINE size_t findNextRelevant(char ch, const jsstring& str, const size_t pos)
{
    jsstring::const_iterator start = str.begin();
    jsstring::const_iterator beginChars = start + pos;
    jsstring::const_iterator p = beginChars;
    for(; p != str.end(); p++)
    {
        if(*p == ch)
        {
            return p - start;
        }
//        if (*p == '\\'){p++;continue;}
        switch(*p)
        {
                BRACKET('[', ']');
                BRACKET('{', '}');
                QUOTECASE();
        }
    }
    return jsstring::npos;

}

CCJSON_INLINE jsstring escapeKeyWord(const jsstring &str)
{
    jsstring res = "";
    for(jsstring::const_iterator i = str.begin(); i < str.end(); i++)
    {
        if(strchr("\"\\", *i) != nullptr)
        {
            res.append(1, '\\');
        }
        res.append(1, *i);
    }
    return res;
}
#endif
