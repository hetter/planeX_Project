//
//  IniReader.cpp
//  plane_demo
//
//  Created by dummy on 13-8-18.
//
//

#include "IniReader.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>

void trim(std::string& output_)
{
    if (output_.empty())
    {
        return;
    }
    output_.erase(0,output_.find_first_not_of(0x20));
    output_.erase(output_.find_last_not_of(0x20) + 1);
    output_.erase(0,output_.find_first_not_of(0x09));
    output_.erase(output_.find_last_not_of(0x09) + 1);
    output_.erase(0,output_.find_first_not_of(0x0d));
    output_.erase(output_.find_last_not_of(0x0d) + 1);
}

IniReader::IniReader()
{}

IniReader::IniReader(const IniReader& reader_)
{
    _doCopy(reader_);
}

IniReader& IniReader::operator=(const IniReader &reader_)
{
     _doCopy(reader_);
    return *this;
}

void IniReader::_doCopy(const IniReader &reader_)
{
    for (IniGroup::const_iterator iItr = reader_.m_iniGroupMap.begin();
         iItr != reader_.m_iniGroupMap.end(); ++iItr)
    {
        for (IniAttribute::const_iterator jItr = iItr->second.begin();
             jItr != iItr->second.end(); ++jItr)
        {
            m_iniGroupMap[iItr->first][jItr->first] = jItr->second;
        }
    }
}

IniReader::~IniReader()
{}

void IniReader::loadIniFile(const char *filePath_)
{
    std::fstream file(filePath_, std::ios::binary | std::ios::in);
    if(!file.is_open())
        return;
    std::string line;
    std::string group;
    while(std::getline(file,line))
    {
        int index1 = line.find_first_of('[') + 1;
        int index2 = line.find_first_of(']');
        if(index1 > 0 && index2 > -1)
        {
            group = line.substr(index1, index2 - index1);
            IniAttribute att;
            m_iniGroupMap[group] = att;
        }
        else
        {
            std::string name, var;
            int equIndex = line.find_first_of('=');
            if(equIndex == -1)
                continue;
            name = line.substr(0, equIndex);
            trim(name);
            var = line.substr(equIndex + 1, line.length() - equIndex - 1);
            trim(var);
            m_iniGroupMap[group][name] = var;
        }
    }
    file.close();
}

const char* IniReader::getAttribute(const char* group_, const char* var_)
{
    IniGroup::iterator gItr = m_iniGroupMap.find(group_);
    if(m_iniGroupMap.end() != gItr)
    {
        IniAttribute::iterator altr = gItr->second.find(var_);
        if(gItr->second.end() != altr)
            return altr->second.c_str();
        else
            return 0;
    }
    return 0;
}

int IniReader::getAttributeInt(const char* group_, const char* var_)
{
    return atoi(getAttribute(group_, var_));
}
float IniReader::getAttributeFloat(const char* group_, const char* var_)
{
    return atof(getAttribute(group_, var_));
}

