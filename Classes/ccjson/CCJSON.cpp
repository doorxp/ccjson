//
//  CCJSON.cpp
//  CCJSONTest
//
//  Created by doorxp on 12-10-22.
//
//

#include "CCJSON.h"
#include "CCJSONUtils.h"
#include "DeENCodeing.h"
#include <regex.h>

using namespace cocos2d;

CCJSONPtr  CCJSON::parse(const jsstring &strJson)
{
    JSONLOG("(%ld)\n\n%s\n\n", strJson.length(), strJson.c_str());
    JSONLOG("length : %X %X", strJson.at(0), strJson.at(strJson.size() - 1));
    if (strJson.empty())
    {
        return NULLJSON();
    }
    else
    {
        return CCJSON::node(false, strJson);
    }
}

bool CCJSON::isValidRoot(const jsstring &strJson)
{
    jsstring value = strJson;
    trim(value);
    
    size_t pos = 0;
    if (value[0] == '{') {
        pos = findNextRelevant('}', value, 0);
    }
    else if (value[0] == '[')
    {
        pos = findNextRelevant(']', value, 0);
    }
    else
    {
        return  false;
    }
    
    return ((pos != jsstring::npos) && (pos == value.size() - 1));
}


CCJSONPtr  CCJSON::objectForKey(const jsstring &key, bool bNull/* = true*/)
{
    fetch();
    
    JSON_FOREACH(this, runner)
    {
        if(strcmp(runner->getName().c_str(), key.c_str()) == 0/* runner->getName() == key*/)
        {
            return (CCJSONPtr )runner;
        }
    }
    
    return bNull ? NULLJSON() : nullptr;
}

CCJSONPtr CCJSON::objectForKeyList(const char *key, va_list ap)
{
    CCJSONPtr  node = this;
    const char *vaKey = key;
    do
    {
        node = node->objectForKey(vaKey, false);
        if(node == nullptr)
        {
            break;
        }
        vaKey = va_arg(ap, const char *);
    } while(vaKey != nullptr);
    
    if(node == nullptr)
    {
        return CCJSON::NULLJSON();
    }
    else
    {
        return node;
    }
}

CCJSONPtr  CCJSON::objectForKeys(const char *key, ...)
{
    CCJSONPtr  node = nullptr;
    
    va_list ap;
    va_start(ap, key);
    
    node = objectForKeyList(key, ap);
    
    va_end(ap);
    
    return node;
}

bool isAllNumber(const char *pszStr)
{
    const char *p = pszStr;
    while (*p!=0) {
        
        if (!isdigit(*p))
        {
            return false;
        }
        p++;
    }
    
    return true;
}

#include <sstream>
inline size_t needAt(const char *pszStr)
{
    size_t iret = SIZE_T_MAX;
    if (pszStr && strlen(pszStr) > 1 && pszStr[0] == '@' && isAllNumber(pszStr+1))
    {
        std::istringstream i(pszStr + 1);
        i>>iret;
    }
    return iret;
}

CCJSONPtr CCJSON::objectForPath(const char *key, bool bNull/*=true*/)
{
    CCJSONPtr  node = this;
    char *src = strdup(key);
    
    char *p = strtok(src, "/");
    while (p)
    {
        size_t at = needAt(p);
        if (at == SIZE_T_MAX)
        {
            node = node->objectForKey(p, bNull);
        }
        else
        {
            node = node->objectAt(at, bNull);
        }
        
        p = strtok(nullptr, "/");
        if (node == nullptr)
        {
            break;
        }
    }
    
    free(src);
    
    if(node == nullptr && bNull)
    {
        return CCJSON::NULLJSON();
    }
    else
    {
        return node;
    }
}

CCJSONPtr  CCJSON::objectAt(const size_t pos, bool bNull/* = true*/)
{
    fetch();
    
    if(m_Children != 0
       && m_Children->size() > 0
       && pos < m_Children->size())
    {
#if (COCOS2D_VERSION<0x00030000)
        return (CCJSONPtr )(m_Children->objectAtIndex(pos));
#else
        return (CCJSONPtr )(m_Children->at(pos));
#endif
    }
    return bNull ? NULLJSON() : nullptr;
}

CCJSONPtr  CCJSON::firstObject(bool bNull/* = true*/)
{
    return objectAt(0, bNull);
}

CCJSONPtr  CCJSON::lastObject(bool bNull/* = true*/)
{
    size_t s = this->size();
    CCJSONPtr pRet = nullptr;
    if (s < 1)
    {
        pRet= bNull?NULLJSON():nullptr;
    }
    else
    {
        pRet=this->objectAt(s-1,bNull);
    }
    return pRet;
}

void CCJSON::push(CCJSONPtr  node, size_t at)
{
#ifdef __APPLE__
    JSONASSERT(pthread_main_np(), "push main thread only");
#endif
    
    if(node == nullptr)
        return;
    
    this->allocChildren();
    
    if(this->getType() == JSON_NODE)
    {
        CCJSONPtr  tmp_json =
        this->objectForKey(node->getName(),false);
        if(tmp_json)
        {
            this->pop(tmp_json);
        }
    }
    
    if(m_Children)
    {
        if (at == SIZE_T_MAX || at > m_Children->size())
        {
            m_Children->pushBack(node);
        }
        else
        {
            m_Children->insert(at, node);
        }
    }
}

CCJSONPtr  CCJSON::pop(const jsstring &key)
{
    if(m_Children == 0 || m_Children->size() < 1)
    {
        return nullptr;
    }
    
    CCJSONPtr  runner = this->objectForKey(key, false);
    
    return pop(runner);
    
}

CCJSONPtr  CCJSON::pop(CCJSONPtr  node)
{
    if(m_Children != 0 && node != nullptr)
    {
        CC_SAFE_RETAIN(node);
        m_Children->eraseObject(node);
        node->autorelease();
    }
    return node;
}


size_t CCJSON::size()
{
    fetch();
    return m_Children != 0 ? m_Children->size() : 0;
}

bool CCJSON::empty()
{
    fetch();
    return m_Children == 0 ? true : !m_Children->size();
}

void CCJSON::clear()
{
    m_Children->clear();
}

const bool CCJSON::as_bool()
{
    return as<bool>();
}

const int CCJSON::as_int()
{
    return as<int>();
    
}

const long CCJSON::as_long()
{
    return as<long>();
}

const long long CCJSON::as_longlong()
{
    return as<long long>();
}

const float CCJSON::as_float()
{
    return as<float>();
}

const double CCJSON::as_double()
{
    return as<double>();
}

const long double CCJSON::as_longdouble()
{
    return as<long double>();
}

const jsstring& CCJSON::as_string()
{
    fetch();
    return m_string;
}
const jsstring& CCJSON::getName()
{
    fetch();
    return m_name;
}

void CCJSON::setName(const jsstring &var)
{
    fetch();
    m_name = var;
}

const CCJSON::JSONType & CCJSON::getType()
{
    fetch();
    return m_type;
}

void CCJSON::setType(const JSONType &var)
{
    fetch();
    m_type = var;
}



const jsstring CCJSON::to_string(JSONType parentType, bool root/* = false*/)
{
    jsstring res = "";
    
    if(JSON_UNKNOW == m_type)
    {
        if(!root && parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
        }
        
        res += this->m_unfetchString;
    }
    else if(m_type == JSON_NODE)
    {
        if(!root && parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
        }
        
        res += "{";
        
        jsstring tmpRes = "";
        jsstring sub;
        
        JSON_FOREACH(this, runner){
            sub = runner->to_string(JSON_NODE);
            if(!sub.empty() && sub.size() > 0)
            {
                tmpRes += sub;
                tmpRes += ",";
            }
        }
        
        if(tmpRes.size() > 0)
            res += tmpRes.substr(0, tmpRes.size() - (!sub.empty() && sub.size() > 0 ? 1 : 0));
        
        res += "}";
        
    }
    else if(m_type == JSON_ARRAY)
    {
        if(parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
        }
        
        res += "[";
        
        jsstring tmpRes = "";
        jsstring sub;
        
        JSON_FOREACH(this, runner){
            sub = runner->to_string(JSON_ARRAY);
            
            if(!sub.empty() && sub.size() > 0)
            {
                tmpRes += sub;
                tmpRes += ",";
            }
        }
        if(tmpRes.size() > 0)
            res +=
            tmpRes.substr(0, tmpRes.size() - (!sub.empty() && sub.size() > 0 ? 1 : 0));
        
        res += "]";
    }
    else if(m_type == JSON_NUMBER)
    {
        if(parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
        }
        res += escapeKeyWord(m_string);
        
    }
    else if(m_type == JSON_STRING)
    {
        if(parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
        }
        res += "\"";
        res += toUnicode(escapeKeyWord(m_string));
        res += "\"";
    }
    else if(m_type == JSON_NULL)
    {
        if(parentType == JSON_NODE)
        {
            res += "\"";
            res += toUnicode(escapeKeyWord(this->m_name));
            res += "\":";
            
            res += "null";
        }
        else if(parentType == JSON_ARRAY)
        {
            res += "null";
        }
        else
        {
            assert(0);
        }
    }
    else if (m_type == JSON_BOOL)
    {
        res += "\"";
        res += toUnicode(escapeKeyWord(this->m_name));
        res += "\":";
        res += m_string;
    }
    else
    {
        JSONASSERT(0, "current has no type");
    }
    
    return res;
}

const jsstring CCJSON::str()
{
    return to_string(m_type != JSON_ARRAY ? JSON_NODE : JSON_ARRAY, true);
}

void CCJSON::setChildren(CHILDREN *var)
{
    if(var != m_Children)
    {
        if(m_Children)
        {
            m_Children->release();
            m_Children = nullptr;
        }

        m_Children = var;

        if (m_Children) {
            m_Children->retain();
        }
    }
}

CHILDREN *CCJSON::getChildren()
{
    fetch();
    return this->m_Children;
}

void CCJSON::doNode()
{
    size_t value_ending;
    size_t name_ending;
    size_t name_starting = 1;
    jsstring name;
    jsstring value;
    
    name_ending =
    findNextRelevant(':', this->m_unfetchString, name_starting);
    
    if(name_ending == jsstring::npos)
    {
        return;
    }
    
    for(value_ending = findNextRelevant(',', this->m_unfetchString, name_ending);
        value_ending != jsstring::npos;
        value_ending = findNextRelevant(',', this->m_unfetchString, name_ending))
    {
        jsstring::iterator st = this->m_unfetchString.begin() + name_ending + 1;
        jsstring::iterator en = this->m_unfetchString.begin() + value_ending;
        value.assign(st, en);
        CCJSONPtr newNode = CCJSON::node(false, value);
        
        name.assign(this->m_unfetchString.begin() + name_starting,
                    this->m_unfetchString.begin() + name_ending);
        name = trim(name);
        name.assign(name.begin() + 1, name.end() - 1);
        newNode->m_name = toUTF8(name);
        
        this->push(newNode);
        
        JSONLOG("node: %s = %s", name.c_str(), value.c_str());
        
        name_starting = value_ending + 1;
        name_ending = findNextRelevant(':', this->m_unfetchString, name_starting);
        if(name_ending == jsstring::npos)
        {
            JSONASSERT(0, ": error");
            return;
        }
    }
    
    name_ending = findNextRelevant(':', this->m_unfetchString, name_starting);
    
    if(name_ending == jsstring::npos)
    {
        JSONASSERT(0, ": error");
        return;
    }
    //    JSONLOG("abc:%x",this->m_unfetchString.at(m_unfetchString.size() - 1));
    value.assign(this->m_unfetchString.begin() + name_ending + 1,
                 this->m_unfetchString.end() - 1);
    
    CCJSONPtr  newNode = CCJSON::node(false, value);
    
    name.assign(this->m_unfetchString.begin() + name_starting,
                this->m_unfetchString.begin() + name_ending);
    name = trim(name);
    name.assign(name.begin() + 1, name.end() - 1);
    
    newNode->m_name = toUTF8(name);
    
    this->push(newNode);
    JSONLOG("end node:(%ld, %ld, %ld) %s = %s", value.size(), value.length(), value.end() - value.begin(), name.c_str(), value.c_str());
    
    
}

void CCJSON::doArray()
{
    size_t ending;
    size_t starting = 1;
    jsstring value;
    for(ending = findNextRelevant(',', this->m_unfetchString, starting);
        ending != jsstring::npos;
        ending = findNextRelevant(',', this->m_unfetchString, starting))
    {
        value.assign(this->m_unfetchString.begin() + starting,
                     this->m_unfetchString.begin() + ending);
        
        JSONLOG("array:\n %s\n\n", value.c_str());
        
        CCJSONPtr  newNode = CCJSON::node(false, value);
        this->push(newNode);
        starting = ending + 1;
    }
    
    jsstring::iterator sta = this->m_unfetchString.begin() + starting;
    jsstring::iterator end = this->m_unfetchString.end() - 1;
    
    if(sta >= end)
    {
        return;
    }
    
    value.assign(sta, end);
    JSONLOG("end array:\n %s\n\n", value.c_str());
    CCJSONPtr  newNode = CCJSON::node(false, value);
    this->push(newNode);
}

void CCJSON::doValue()
{
    if ((strcasecmp(this->m_unfetchString.c_str(), "true") == 0)
        || (strcasecmp(this->m_unfetchString.c_str(), "false") == 0))
    {
        m_type = JSON_BOOL;
        this->m_string = this->m_unfetchString;
    }
    else if((m_unfetchString[0] == '.')
            || (m_unfetchString[0] == '-')
            || (m_unfetchString[0] >= '0'
                && m_unfetchString[0] <= '9'))
    {
        m_type = JSON_NUMBER;
        this->m_string = this->m_unfetchString;
    }
    else if(StringToNumber::isNull(this->m_unfetchString))
    {
        m_type = JSON_NULL;
        this->m_string = "null";
    }
    else
    {
        m_type = JSON_STRING;
        
        
        jsstring value(this->m_unfetchString.begin() + 1,
                       this->m_unfetchString.end() - 1);
        
        this->m_string = toUTF8(value);
    }
    
}

/*CCJSON_INLINE*/ void CCJSON::fetch()
{
#ifdef __APPLE__
    //    JSONASSERT(pthread_main_np(), "fetch main thread only");
#endif
    if(JSON_UNKNOW != m_type)
    {
        return;
    }
    
    
    const jsstring::size_type& pos = 0;
    
    if(m_unfetchString[pos] == '{')
    {
        m_type = JSON_NODE;
        this->doNode();
    }
    else if(m_unfetchString[pos] == '[')
    {
        m_type = JSON_ARRAY;
        this->doArray();
    }
    else
    {
        this->doValue();
    }
    
    this->m_unfetchString = "";
}


CCJSONPtr  CCJSON::NULLJSON()
{
    return node(JSON_NULL);
}

CCJSONPtr  CCJSON::node(JSONType type/* = JSON_NODE*/)
{
    CCJSONPtr  n = NEW(CCJSON)(type);
    n->autorelease();
    return n;
}

CCJSONPtr  CCJSON::node(bool hasFetched, const jsstring& value)
{
    CCJSONPtr n = NEW(CCJSON)(hasFetched, value);
    n->autorelease();
    return n;
}

CCJSON::CCJSON(JSONType type/* = JSON_NODE*/)
{
    this->m_Children = nullptr;
    this->m_name = "";
    this->m_type = type;
    this->m_string = (type == JSON_NULL) ? "null" : "";
    this->m_unfetchString = "";
}


CCJSON::CCJSON(bool hasFetched, const jsstring& value)
{
    this->m_Children = nullptr;
    this->m_name = "";
    if(hasFetched)
    {
        this->m_string = value;
        this->m_unfetchString = "";
        this->m_type = JSON_STRING;
    }
    else
    {
        this->m_type = JSON_UNKNOW;
        this->m_string = "";
        this->m_unfetchString = value;
        trim(m_unfetchString);
    }
}

void CCJSON::allocChildren()
{
    if(!m_Children)
    {
        /*
         CHILDREN *children = NEW(CHILDREN)();
         this->setChildren(children);
         children->autorelease();
         /*/
#if (COCOS2D_VERSION > 0x00020000)
        this->setChildren(new CHILDREN);
#else
        this->setChildren(CHILDREN::array());
#endif
        //*/
    }
}

CCJSONPtr CCJSON::clone() const {
    CCJSONPtr  tmp = CCJSON::node();

    tmp->m_type = this->m_type;
    tmp->m_name = this->m_name;
    tmp->m_string = this->m_string;
    tmp->m_unfetchString = this->m_unfetchString;

    if (tmp->m_type != JSONType::JSON_UNKNOW) {

        tmp->allocChildren();
        if (m_Children!=nullptr) {
            for (CHILDREN::const_iterator i = m_Children->begin();
                 i!=m_Children->end();
                 i++) {
                CCJSONPtr sub = (*i)->clone();
                tmp->push(sub);
            }
        }
    }

    return tmp;
}

/*
CCJSONPtr  CCJSON::copy(bool deep)
{
    CCJSONPtr  tmp = CCJSON::node();
    CC_SAFE_RETAIN(tmp);

    tmp->m_type = this->m_type;
    tmp->m_name = this->m_name;
    tmp->m_string = this->m_string;
    tmp->m_unfetchString = this->m_unfetchString;

    if (deep)
    {
        JSON_FOREACH(this, runner)
        {
            tmp->allocChildren();
            CCJSONPtr sub = runner->copy(deep);
            tmp->push(sub);
           sub->autorelease();
        }
    } else {

        CHILDREN *children = this->getChildren();

        tmp->setChildren(children);

        for (CHILDREN::iterator i=children->begin();
             i!=children->end();
             i++) {
            (*i)->retain();
        }
    }

    return tmp;
}
 //*/

CCJSONPtr CCJSON::allKeys()
{
    if (this->getType() != JSON_NODE)
    {
        return NULLJSON();
    }
    else
    {
        CCJSONPtr arra = CCJSON::node(JSON_ARRAY);
        JSON_FOREACH(this, runner)
        {
            arra->push(CCJSON::node(runner->getName()));
        }
        return arra;
    }
}


CCJSON::~CCJSON()
{
    setChildren(nullptr);
    this->m_string = "";
    this->m_name = "";
    this->m_unfetchString = "";
}


#if (COCOS2D_VERSION > 0x00020000)
CHILDREN* CHILDREN::clone() const {
    CHILDREN *ret = new(std::nothrow) CHILDREN();
    ret->autorelease();

    CCJSONPtr tmpObj = nullptr;
    CCJSONPtr obj = nullptr;

    for (CHILDREN::const_iterator i = this->begin();
         i!=this->end();
         i++) {
        obj = *i;

        tmpObj = dynamic_cast<CCJSONPtr>(obj->clone());
        if (tmpObj) {
            ret->pushBack(tmpObj);
        }
    }

    return ret;
}
#endif
