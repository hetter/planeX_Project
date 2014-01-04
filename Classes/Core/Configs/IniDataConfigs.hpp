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
        //battleConfig
        std::string batConfigfullPath = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("battleConfig.ini");
        IniReader bcIniReader;
        bcIniReader.loadIniFile(batConfigfullPath.c_str());
        IniDataMap["BattleConfig"] = bcIniReader;
    }
    
    ~IniDataConfigs()
    {
        IniDataMap.clear();
    }
public:
    typedef std::map<std::string, IniReader> IniMap;
    IniMap IniDataMap;
};

#define GetIniConfigs (IniDataConfigs::GetInstance()->IniDataMap)

#endif
