//
//  JSON.h
//  CCJSONTest
//
//  Created by doorxp on 12-10-23.
//
//

#ifndef CCJSONTest_JSON_h
#define CCJSONTest_JSON_h

#define __CCJSON__ 1

#include "CCJSON.h"

#if (COCOS2D_VERSION<0x00030000)

#include "CCJSONMarcos.h"
template <typename T>
CCJSON_INLINE
cocos2d::CCJSONPtr ZXJS(jsstring key, T val)
{
    cocos2d::CCJSONPtr tmp = cocos2d::CCJSON::node(key, val);
    return tmp;
}
#endif

#endif
