//
//  IniReader.h
//  plane_demo
//
//  Created by dummy on 13-8-18.
//
//

#ifndef __plane_demo__IniReader__
#define __plane_demo__IniReader__

#include <map>
#include <string>

class IniReader
{
public:
    IniReader();
    ~IniReader();
    
    void loadIniFile(const char* filePath_);
    
    const char* getAttribute(const char* group_, const char* var_);
    int getAttributeInt(const char* group_, const char* var_);
    float getAttributeFloat(const char* group_, const char* var_);
public:
    typedef std::map<std::string, std::string>IniAttribute;
    typedef std::map<std::string, IniAttribute> IniGroup;
    
    IniGroup m_iniGroupMap;
};


#endif /* defined(__plane_demo__IniReader__) */
