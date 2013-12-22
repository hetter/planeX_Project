//
//  PlaneUnit.h
//  plane_demo
//
//  Created by dummy on 13-7-6.
//
//

#ifndef  __plane_demo__BaseEvent__
#define  __plane_demo__BaseEvent__

#include <vector>
#include <map>

#include "../GAME_DECLARATION.h"

class BaseEvent
{
public:
    BaseEvent();
    virtual ~BaseEvent();
    
    virtual void updateConditions() = 0;
private:
    int m_eventId;
    std::vector<int> m_conditionLIst;
};

class EventFactor:public Singleton<PlaneUnitMgr>
{
public:
private:
};

#endif /* defined(__plane_demo__PlaneUnit__) */
