//
//  IniDataConfigs.hpp
//  PlaneX
//
//  Created by dummy on 14-1-4.
//
//

#ifndef PlaneX_IniDataConfigs_hpp
#define PlaneX_IniDataConfigs_hpp

#include <map>

#include "../../Base/Singleton.hpp"
#include "../../Base/iniReader.h"
#include "platform/CCFileUtils.h"

class IniDataConfigs:public Singleton<IniDataConfigs>
{
public:
    IniDataConfigs()
    {
        _loadIniFile("battleConfig.ini", "BattleConfig");
        _loadIniFile("battleResConfig.ini", "BattleResConfig");
    }
    
    ~IniDataConfigs()
    {
        IniDataMap.clear();
    }
public:
    typedef std::map<std::string, IniReader> IniMap;
    IniMap IniDataMap;
private:
    void _loadIniFile(const char* fileName_, const std::string& keyName_)
    {
        std::string configfullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename(fileName_);
        IniReader iniReader;
        iniReader.loadIniFile(configfullPath.c_str());
        IniDataMap[keyName_] = iniReader;
    }
};

#define GetIniConfigs (IniDataConfigs::GetInstance()->IniDataMap)

#endif
