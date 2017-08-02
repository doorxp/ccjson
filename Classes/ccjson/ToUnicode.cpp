//
//  ToUnicode.cpp
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

//0X8->'8'
inline uint8_t hex2Char(uint8_t a);
//0X86->"86"
inline jsstring hex2Str(uint8_t cc);

inline jsstring doToUnicode(jsstring::const_iterator &p, const jsstring::const_iterator &end);


jsstring toUnicode(const jsstring& s)
{
#if !defined(NEEDCODE) || (NEEDCODE == 0)
    return  s;
#else
    jsstring res = "";
    jsstring::const_iterator end = s.end();
    for(jsstring::const_iterator i = s.begin(); i < end; i++)
    {
        res.append(doToUnicode(i, end));
    }
    return res;
#endif
}

#define CHECKOUTBOUND() if (p >= end){return s;}

jsstring doToUnicode(jsstring::const_iterator &p, const jsstring::const_iterator &end)
{
    jsstring s = "";
    uint8_t a = *p;
    
    if((a & 0X80) == 0X00)
    {
        s.append(1, *p);
    }
    else if((a & 0XE0) == 0XC0)
    {
        p++;
        CHECKOUTBOUND()
        uint8_t b = *p;
        if((b & 0XC0) != 0X80)
        {
            assert(0);
        }
        s.append("\\u");
        s.append(hex2Str((uint8_t)((a >> 2) & 0X07)));
        s.append(hex2Str((uint8_t)(((a << 6) & 0XC0) | (b & 0X3F))));
    }
    else if((a & 0XF0) == 0XE0)
    {
        p++;
        CHECKOUTBOUND()
        uint8_t b = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t c = *p;
        if((b & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((c & 0XC0) != 0X80)
        {
            assert(0);
        }
        s.append("\\u");
        s.append(hex2Str((uint8_t)(((a & 0X0F) << 4) | ((b & 0X3C) >> 2))));
        s.append(hex2Str((uint8_t)(((b & 0X03) << 6) | (c & 0X3F))));
    }
    else if((a & 0XF8) == 0XF0)
    {
        p++;
        CHECKOUTBOUND()
        uint8_t b = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t c = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t d = *p;
        
        if((b & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((c & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((d & 0XC0) != 0X80)
        {
            assert(0);
        }
        s.append("\\u");
        s.append(hex2Str((uint8_t)(((a & 0X07) << 2) | ((b & 0X30) >> 4))));
        s.append(hex2Str((uint8_t)(((b & 0X0F) << 4) | ((c & 0X3C) >> 2))));
        s.append(hex2Str((uint8_t)(((c & 0X03) << 6) | (d & 0X3F))));
    }
    else if((a & 0XFC) == 0XF8)
    {
        p++;
        CHECKOUTBOUND()
        uint8_t b = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t c = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t d = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t e = *p;
        
        
        if((b & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((c & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((d & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((e & 0XC0) != 0X80)
        {
            assert(0);
        }
        s.append("\\u");
        s.append(hex2Str((uint8_t)(a & 0X03)));
        s.append(hex2Str((uint8_t)(((b & 0X3F) << 2) | ((c & 0X30) >> 4))));
        s.append(hex2Str((uint8_t)(((c & 0X0F) << 4) | ((d & 0X3C) >> 2))));
        s.append(hex2Str((uint8_t)(((d & 0X03) << 6) | (e & 0X3F))));
    }
    else if((a & 0XFE) == 0XFC)
    {
        p++;
        CHECKOUTBOUND()
        uint8_t b = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t c = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t d = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t e = *p;
        p++;
        CHECKOUTBOUND()
        uint8_t f = *p;
        
        
        if((b & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((c & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((d & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((e & 0XC0) != 0X80)
        {
            assert(0);
        }
        if((f & 0XC0) != 0X80)
        {
            assert(0);
        }
        
        s.append("\\u");
        s.append(hex2Str((uint8_t)(((a & 0X01) << 6) | (b & 0X3F))));
        s.append(hex2Str((uint8_t)(((c & 0X3F) << 2) | ((d & 0X30) >> 4))));
        s.append(hex2Str((uint8_t)(((d & 0X0F) << 4) | ((e & 0X3C) >> 2))));
        s.append(hex2Str((uint8_t)(((e & 0X03) << 2) | (f & 0X3F))));
    }
    else
    {
        s.append(1, (char)a);
    }
    return s;
}

//0X8->"8"
uint8_t hex2Char(uint8_t a)
{
    if(a >= 0 && a <= 9)
    {
        return '0' + a;
    }
    else
    {
        return a - 10 + 'a';
    }
}

//0X86->"86"
jsstring hex2Str(uint8_t cc)
{
    jsstring s = "";
    uint8_t a = (cc >> 4) & 0X0F;
    uint8_t b = (0X0F & cc);
    s.append(1, (char)hex2Char(a));
    s.append(1, (char)hex2Char(b));
    return s;
}


