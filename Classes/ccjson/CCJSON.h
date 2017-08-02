//
//  CCJSON.h
//  CCJSONTest
//
//  Created by doorxp on 12-10-22.
//
//

#ifndef CCJSONTest_CCJSON_h
#define CCJSONTest_CCJSON_h

#include <iostream>
#include <iomanip>
#include <sstream>
#include <typeinfo>
#include "cocos2d.h"
#include "CCJSONMarcos.h"
#include "StringToNumber.h"


#define CCJSONSTR(...) #__VA_ARGS__

NS_CC_BEGIN

class CCJSON;
typedef CCJSON* CCJSONPtr;

#if (COCOS2D_VERSION<0x00030100)
//typedef CCJSONPtr CCJSONPtr __deprecated_msg("use CCJSONPtr");
typedef int CHILDRENIterator;
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
typedef CCArray CHILDREN;
#pragma clang diagnostic pop

#if (COCOS2D_VERSION<0x00030000)
typedef CCObject CCJSONSUPER;
#else
typedef Object CCJSONSUPER;
#endif

#else
class CHILDREN : public Vector<CCJSONPtr>, public Ref, public Clonable {
    virtual CHILDREN* clone() const;
};

//typedef Vector<CCJSONPtr> CHILDREN;
typedef Ref CCJSONSUPER;
typedef ssize_t CHILDRENIterator;
#endif

class CCJSON : public CCJSONSUPER, public Clonable
{
    typedef CCJSONSUPER super;
public:

    virtual CCJSONPtr clone() const;
    
public:
    
    static CCJSONPtr  parse(const jsstring &strJson) CCJSON_EXPORT;
    
    static bool isValidRoot(const jsstring &strJson) CCJSON_EXPORT;
    
public:
    CCJSONPtr  OFK(const jsstring &key, bool bNull = true) CCJSON_EXPORT
    {
        return this->objectForKey(key, bNull);
    }
    
    CCJSONPtr  OFKS(const char *key, ...) CCJSON_EXPORT
    {
        va_list ap;
        va_start(ap, key);
        CCJSONPtr pRet = this->objectForKeyList(key, ap);
        va_end(ap);
        return pRet;
        
    }
    
    CCJSONPtr OFP(const char *key, bool bNull=true) CCJSON_EXPORT
    {
        return this->objectForPath(key, bNull);
    }
    
    CCJSONPtr objectForKeyList(const char *key, va_list ap);
    
    CCJSONPtr  AT(const size_t pos, bool bNull = true)CCJSON_EXPORT{
        return objectAt(pos, bNull);
    }
    
    CCJSONPtr  objectForKey(const jsstring &key, bool bNull = true) CCJSON_EXPORT;
    
    CCJSONPtr  objectAt(const size_t pos, bool bNull = true) CCJSON_EXPORT;
    
    CCJSONPtr  objectForKeys(const char *key, ...)  CCJSON_EXPORT;
    CCJSONPtr  objectForPath(const char *key, bool bNull=true) CCJSON_EXPORT;
    CCJSONPtr allKeys();
    
    CCJSONPtr  firstObject(bool bNull= true);
    
    CCJSONPtr  lastObject(bool bNull= true);
public:
    
    void push(CCJSONPtr node, size_t at = SIZE_T_MAX) CCJSON_EXPORT;
    
    template<typename T>
    CCJSONPtr push(const jsstring &key, T v)
    {
        CCJSONPtr n = CCJSON::node(key, v);
        push(n);
        return n;
    }

    template<typename T>
    CCJSONPtr push(T v)
    {
        this->m_type = JSON_ARRAY;
        CCJSONPtr n = CCJSON::node(v);
        push(n);
        return n;
    }
    
    //        template<>
    CCJSONPtr push(const jsstring &key, CCJSONPtr v)
    {
        CCJSONPtr newNode = v->clone();
        newNode->setName(key);
        push(newNode);
        //newNode->autorelease();
        return newNode;
    }
    
    CCJSONPtr  pop(const jsstring &key) CCJSON_EXPORT;
    
    CCJSONPtr  pop(CCJSONPtr  node) CCJSON_EXPORT;
   
    CCJSONPtr  pop(size_t idx)
    {
        return pop(AT(idx, false));
    }
    
public:
    
    size_t size() CCJSON_EXPORT;
    
    bool empty() CCJSON_EXPORT;
    
    void clear() CCJSON_EXPORT;
    
    bool isNull()
    {
        return this->getType() == JSON_NULL;
    }
    
    bool isNullType()
    {
        return isNull();
    }
    
    bool isBool()
    {
        return this->getType() == JSON_BOOL;
    }
    
    bool isNumeric()
    {
        return this->getType() == JSON_NUMBER;
    }
    
    bool isString()
    {
        return this->getType() == JSON_STRING;
    }
    
    bool isArray()
    {
        return this->getType() == JSON_ARRAY;
    }
    
    bool isObject()
    {
        return this->getType() == JSON_NODE;
    }
    
    bool isMember(const char *pszkey)
    {
        return this->OFP(pszkey, false) != nullptr;
    }
    
    bool isValidIndex(int nIndex)
    {
        return nIndex >=0 && nIndex<this->size();
    }
    
public:
    
    const bool as_bool() CCJSON_EXPORT;
    
    const int as_int() CCJSON_EXPORT;
    
    const long as_long() CCJSON_EXPORT;
    
    const long long as_longlong() CCJSON_EXPORT;
    
    const float as_float() CCJSON_EXPORT;
    
    const double as_double() CCJSON_EXPORT;
    
    const long double as_longdouble() CCJSON_EXPORT;
    
    const jsstring& as_string() CCJSON_EXPORT;
    
    const jsstring str() CCJSON_EXPORT;
    
    template<typename T>
    T as()
    {
        fetch();
        return StringToNumber::toNumber<T>(m_string);
    }
private:
    
    void fetch();
    
public:
    typedef enum _JSONType
    {
        JSON_UNKNOW,
        JSON_NULL,
        JSON_NODE,
        JSON_ARRAY,
        JSON_NUMBER,
        JSON_BOOL,
        JSON_STRING,
        JSON_MAX
    } JSONType;
    
    
    const JSONType & getType();
    
private:
    jsstring m_name;
public:
    const jsstring& getName();
    void setName(const jsstring &var);
    
protected: CHILDREN* m_Children;
public: virtual CHILDREN* getChildren(void);
public: virtual void setChildren(CHILDREN* var);
//    CC_PROPERTY(CHILDREN *, m_Children, Children);
private:
    jsstring m_string;
    jsstring m_unfetchString;
    JSONType m_type;
    
    void setType(const JSONType &var);
    
    const jsstring to_string(JSONType parentType, bool root = false);
    
public:
    
    static CCJSONPtr  NULLJSON() CCJSON_EXPORT;
    
    static CCJSONPtr  node(JSONType type = JSON_NODE) CCJSON_EXPORT;
    
    static CCJSONPtr  node(bool hasFetched, const jsstring& value) CCJSON_EXPORT;
    
    template<class T>
    static CCJSONPtr  node(const jsstring &key, T value)
    {
        CCJSONPtr  n = node(value);
        n->m_name = key;
        return n;
    }
    
    template<class T>
    static CCJSONPtr node(const char *key, T value)
    {
        return node((const jsstring)(key), value);
    }
    
private:
    
    void allocChildren();
    
    template<class T> void _init(T value)
    {
        this->m_Children = nullptr;
        this->m_name = "";
        this->m_type = JSON_UNKNOW;
        this->m_unfetchString = "";
        
        if(isStringType(value))
        {
            this->m_string = value;
            this->m_type = JSON_STRING;
        }
        else
        {
            std::ostringstream o;
            o << std::fixed << std::boolalpha << value;
            this->m_string = o.str();
            this->m_type = JSON_NUMBER;
        }
    }
public:
    
    template<class T>
    static CCJSONPtr  node(T value)
    {
        CCJSONPtr  node = NEW(CCJSON)(value);
        node->autorelease();
        return node;
    }
private:
    template<class T>
    CCJSON(const jsstring & key, T value)
    {
        _init(value);
        m_name = key;
    }
    
    template<class T>
    explicit CCJSON(T value)
    {
        _init(value);
    }
    
    CCJSON(JSONType type = JSON_NODE) CCJSON_EXPORT;
    
    CCJSON(bool hasFetched, const jsstring& value) CCJSON_EXPORT;
    
private:
    
    virtual ~CCJSON() CCJSON_EXPORT;
    
private:
    
    void doNode();
    
    void doArray();
    
    void doValue();
    
public:
    
//    virtual CCJSONPtr copy(bool deep = true);
    
};

NS_CC_END

#define GenerateUniqueVariable2(np1,np2) np1##np2
#define GenerateUniqueVariable1(np1,np2) GenerateUniqueVariable2(np1,np2)
#define GUVar(np1) GenerateUniqueVariable1(np1, __LINE__)

#if (COCOS2D_VERSION<0x00030000)

#define JSON_FOREACH(__parent__, __runner__) \
cocos2d::CHILDRENIterator GUVar(__it__)=0;\
if (__parent__ != nullptr && (__parent__)->getChildren() && (__parent__)->getChildren()->count() > 0)\
for (cocos2d::CCJSONPtr  __runner__ = nullptr;\
(GUVar(__it__) < (__parent__)->getChildren()->count())&&(__runner__= (cocos2d::CCJSONPtr )(__parent__)->getChildren()->objectAtIndex(GUVar(__it__)));\
GUVar(__it__)++)


#define JSON_RFOREACH(__parent__, __runner__) \
cocos2d::CCJSONPtr  (__runner__) = nullptr;\
if (__parent__ != nullptr && (__parent__)->getChildren() && (__parent__)->getChildren()->count() > 0)\
for (cocos2d::CHILDRENIterator __it__=(__parent__)->getChildren()->count()-1;\
(__it__ >= 0) &&((__runner__)= (cocos2d::CCJSONPtr )(__parent__)->getChildren()->objectAtIndex(__it__));\
__it__--)

#else
#define JSON_FOREACH(__parent__, __runner__) \
cocos2d::CHILDRENIterator GUVar(__it__)=0;\
if (__parent__ != nullptr \
    && (__parent__)->getChildren() \
    && (__parent__)->getChildren()->size() > 0)\
        for (cocos2d::CCJSONPtr  __runner__ = nullptr;\
            (GUVar(__it__) < (__parent__)->getChildren()->size())\
                &&(__runner__= (cocos2d::CCJSONPtr )(__parent__)\
                    ->getChildren()->at(GUVar(__it__)));\
                        GUVar(__it__)++)


#define JSON_RFOREACH(__parent__, __runner__) \
cocos2d::CCJSONPtr  (__runner__) = nullptr;\
if (__parent__ != nullptr && (__parent__)->getChildren() \
    && (__parent__)->getChildren()->size() > 0)\
        for (cocos2d::CHILDRENIterator __it__=(__parent__)->getChildren()->size()-1;\
            (__it__ >= 0) &&((__runner__)= \
                (cocos2d::CCJSONPtr )(__parent__)->getChildren()->at(__it__));\
            __it__--)

#endif

#define JSON_EACH(__parent__, __runner__) JSON_FOREACH(__parent__, __runner__)
#define JSON_REACH(__parent__, __runner__) JSON_RFOREACH(__parent__, __runner__)

#ifdef DEBUG
#define LOGJSON(__obj__) JSONOUT(__obj__)
#define JSONOUT(__obj__) if(__obj__)CCLOG("%s", __obj__->str().c_str());else CCLOG("JSON IS nullptr");
#else
#define JSONOUT(__obj__)
#define LOGJSON(__obj__)
#endif //DEBUG

#endif


