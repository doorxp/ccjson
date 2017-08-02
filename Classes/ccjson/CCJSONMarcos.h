//
//  CCJSONMarcos.h
//  CCJSONTest
//
//  Created by doorxp on 12-10-22.
//
//

#ifndef CCJSONTest_CCJSONMarcos_h
#define CCJSONTest_CCJSONMarcos_h

#ifndef NEW
#define NEW(__OBJ__) new __OBJ__
#endif //NEW

#define CCJSONDEBUG 1
#define CCJSONLOGENABLED 0
#define CCJsONASSERTENABLED 1

#ifdef CCJSONDEBUG

#if (CCJSONLOGENABLED == 1)
#define JSONLOG(format, ...) CCLOG(format, ##__VA_ARGS__)
#else
#define JSONLOG(format, ...)
#endif //CCJSONLOGENABLED

#if (CCJsONASSERTENABLED == 1)
#define JSONASSERT(__con__, error) if(!(__con__)){CCLOG("%s",error); assert(0);}
#else
#define JSONASSERT(__con__, error)
#endif //CCJsONASSERTENABLED

#else 
#define JSONLOG(format, ...)
#define JSONASSERT(__con__, error)

#endif //CCJSONDEBUG

#define jsstring std::string


#define CCJSON_INLINE inline
#define CCJSON_EXPORT __attribute__((visibility("default")))

template<typename T>
CCJSON_INLINE bool isStringType(T value)
{
    return false;
}

#include <string>

template<>
CCJSON_INLINE bool isStringType(const std::string value)
{
    return true;
}

template<>
CCJSON_INLINE bool isStringType(const std::string& value)
{
    return true;
}

template<>
CCJSON_INLINE bool isStringType(const char * value)
{
    return true;
}

template<>
CCJSON_INLINE bool isStringType(char * value)
{
    return true;
}

#if __cplusplus < 201103L
#define nullptr NULL
#endif

#ifndef CCASSERT
#define CCASSERT(oo, des)
#endif

#endif
