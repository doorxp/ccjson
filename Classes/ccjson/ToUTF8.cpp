//
//  ToUTF8.cpp
//  TKPoker
//
//  Created by doorxp on 1/17/14.
//
//

#include "DeENCodeing.h"
#include "CCJSONMarcos.h"
#include "cocos2d.h"
#include "CCJSONUtils.h"

USING_NS_CC;

//"8"->0X8
inline u_int8_t char2Hex(u_int8_t ch);
//"86"->0X86
inline uint8_t str2Hex(jsstring::const_iterator & pos);

inline jsstring doToUTF8(jsstring::const_iterator & pos,
                  const jsstring::const_iterator & end);


#define CASE(__o__,__oo__) case __o__:{resString.append(1, __oo__);}break

bool needT(char var)
{
    return strchr("0nrtvabf'/\"\\", var) != nullptr;
}

jsstring toUTF8(const jsstring& s)
{
    jsstring resString("");
    resString.reserve(s.size());
#if !defined(NEEDCODE) || (NEEDCODE == 0)
    for (
#if (__cplusplus < 201103L)
         jsstring::const_iterator
#else
       auto
#endif
         i = s.begin();
         i!=s.end();
         i++)
    {
        if(*i == '\\'
           && (i+1) != s.end()
           && needT(*(i+1)))
        {
            switch (*(i+1)) {
                    CASE('0', '\0');
                    CASE('n', '\n');
                    CASE('r', '\r');
                    CASE('t', '\t');
                    CASE('v', '\v');
                    CASE('a', '\a');
                    CASE('b', '\b');
                    CASE('\'', '\'');
                    CASE('"', '"');
                    CASE('\\', '\\');
                    CASE('/', '/');
                default:
                    resString.append(1, *i);
                    break;
            }
            i++;
            if (i == s.end())
            {
                break;
            }
        }
        else
        {
            resString.append(1, *i);
        }
    }
    
#else
    
    jsstring::const_iterator end = s.end();
    for(jsstring::const_iterator i = s.begin();
        i != end;
        i++)
    {
        if((i+1) != end
           && *i == '\\'
           && *(i + 1) == 'u')
        {
            i += 2;
            resString.append(doToUTF8(i, end));
            if (i == end)
            {
                break;
            }
        }
        else if(*i == '\\'
                && (i + 1) != end
                && (strchr("0nrtvabf'/\"\\", *(i + 1)) != nullptr))
        {
            switch(*(i + 1))
            {
                    CASE('0', '\0');
                    CASE('n', '\n');
                    CASE('r', '\r');
                    CASE('t', '\t');
                    CASE('v', '\v');
                    CASE('a', '\a');
                    CASE('b', '\b');
                    CASE('\'', '\'');
                    CASE('"', '"');
                    CASE('\\', '\\');
                    CASE('/', '/');
                default:
                    break;
            }
            
            i += 1;
        }
        else
        {
            resString.append(1, *i);
        }
        
    }
    
#endif
    return resString;
}

jsstring _doToUTF8(uint32_t var)
{
    jsstring s = "";
    if(var <= 0X7F)
    {
        s.append(1, (char)(var & 0X007F));
    }
    else if(var <= 0X7FF)
    {
        char tmp = 0XC0 | ((var & 0X07FF) >> 6);
        s.append(1, tmp);
        tmp = 0x80 | (var & 0x003F);
        s.append(1, tmp);
    }
    else if(var <= 0XFFFF)
    {
        char tmp = 0XE0 | (~0XFFE0 & (var >> 12));
        s.append(1, tmp);
        tmp = 0X80 | ((var & 0XFC0) >> 6);
        s.append(1, tmp);
        
        tmp = 0X80 | (var & 0X3F);
        s.append(1, tmp);
    }
    else if(var <= 0X1FFFFF)
    {
        char tmp = 0XF0 | ((0X1C0000 & var) >> 18);
        s.append(1, tmp);
        tmp = 0X80 | ((var & 0X3F000) >> 12);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0XFC0) >> 6);
        s.append(1, tmp);
        
        tmp = 0X80 | (var & 0X3F);
        s.append(1, tmp);
    }
    else if(var <= 0X3FFFFFF)
    {
        char tmp = 0XF0 | ((0x3000000 & var) >> 24);
        s.append(1, tmp);
        tmp = 0X80 | ((var & 0XFC0000) >> 18);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0X3FC000) >> 12);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0XFC0) >> 6);
        s.append(1, tmp);
        
        tmp = 0X80 | (var & 0X3F);
        s.append(1, tmp);
    }
    else
    {
        char tmp = 0XFC | ((0x60000000 & var) >> 30);
        s.append(1, tmp);
        
        tmp = 0X80 | ((0x3F000000 & var) >> 24);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0XFC0000) >> 18);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0X3F000) >> 12);
        s.append(1, tmp);
        
        tmp = 0X80 | ((var & 0XFC0) >> 6);
        s.append(1, tmp);
        
        tmp = 0X80 | (var & 0X3F);
        s.append(1, tmp);
    }
    return s;
}

jsstring doToUTF8(jsstring::const_iterator & pos,
                  const jsstring::const_iterator & end)
{
    jsstring res = "";
    
    if (end-pos<4)
    {
        return  "";
    }
    
    uint32_t a = str2Hex(pos) & 0X00FF;
    uint32_t tmp = a << 8;
    tmp &= 0X0FF00;
    
    pos++;
    
    
    uint32_t b = str2Hex(pos) & 0X00FF;
    tmp |= b;
    tmp &= 0X0FFFF;
    
    
    if(end-pos>2
       && isxdigit(*(pos+1))
       && isxdigit(*(pos+2)))
    {
        pos++;
        tmp <<= 8;
        
        uint32_t c = str2Hex(pos) & 0X00FF;
        tmp |= c;
        tmp &= 0X0FFFFFF;
        
        if(!(tmp >= 0X01F300 && tmp <= 0X01F64F))
        {
            pos -= 2;
            tmp >>= 8;
            tmp &= 0X0FFFF;
        }
    }
    res.append(_doToUTF8(tmp));
    return res;
}

//"8"->0X8
u_int8_t char2Hex(u_int8_t ch)
{
    if(ch - 'a' >= 0)
    {
        ch = ch - 'a' + 10;
    }
    else if (ch - 'A' >= 0)
    {
        ch = ch - 'A' + 10;
    }
    else if (ch - '0' >= 0)
    {
        ch -= '0';
    }
    else
    {
        CCASSERT(0, "out of bound");
    }
    
    CCASSERT(ch >= 0 && ch<=15, "out of boun");
    
    return ch & 0X0F;
}

//"86"->0X86
uint8_t str2Hex(jsstring::const_iterator & pos)
{
    
    uint8_t hi = char2Hex(*pos++);
    uint8_t lo = char2Hex(*pos);
    return (uint8_t)((hi << 4) | lo);
}