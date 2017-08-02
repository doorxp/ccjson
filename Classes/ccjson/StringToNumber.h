//
//  StringToNumber.h
//  CCJSONTest
//
//  Created by doorxp on 12-10-22.
//
//

#ifndef CCJSONTest_StringToNumber_h
#define CCJSONTest_StringToNumber_h

#include "cocos2d.h"
#include <string>
#include <sstream>
#include <iomanip>
#include <typeinfo>
#include "CCJSONMarcos.h"

#ifndef TRANSITIONS
#define TRANSITIONS(__type__, __defVal__)\
    do {\
        __type__ retValue = __defVal__;\
        std::stringstream ss(astr);\
        ss>>std::boolalpha>>retValue;\
        return retValue;\
    }while(0)
#endif //TRANSITIONS

namespace cocos2d
{

    class StringToNumber
    {
    public:
        static bool isNull(const jsstring& astr)
        {
            return (astr == "null");
        }

        template<class T>
        static T toNumber(const jsstring & astr)
        {
            if (astr == "true")
            {
                return 1;
            }
            else if(typeid(T) == typeid(bool))
            {
                return (astr != ""
                        && astr != "0"
                        && (strcasecmp(astr.c_str(), "false") != 0)
                        && (strcasecmp(astr.c_str(), "null") != 0));
            }
            
            
            TRANSITIONS(T, 0);
        }
    };
}


#endif
