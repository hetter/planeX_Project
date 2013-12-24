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
    BaseEvent()
    :m_bIsEventDead(false)
    {}
    
    virtual ~BaseEvent()
    {}
    
    virtual bool updateConditions() = 0;
protected:
    int m_eventId;
    typedef std::map<int, bool> ConditionMap;
    ConditionMap m_conditionMap;
    bool m_bIsEventDead;
};

class PlaneEvent:public BaseEvent
{
public:
    //用与ccrect互转，为了实现union结构
    struct EventRect
    {
        int x;
        int y;
        int width;
        int height;
    };
    
    enum EventEnum
    {
        RECT_EVENT = 0,
        UNIT_EVENT = 1,
    };
    
    typedef void (*RectEventFunc) (BasePlaneUnit*,  const cocos2d::CCRect&);
    typedef void (*UnitEventFunc) (BasePlaneUnit*,  BasePlaneUnit*);
    
    typedef bool (*RectRaiseFunc) (BasePlaneUnit*,  const cocos2d::CCRect&);
    typedef bool (*UnitRaiseFunc) (BasePlaneUnit*,  BasePlaneUnit*);
public:
    PlaneEvent(BasePlaneUnit* nodeUnit_);
    ~PlaneEvent();
    
    void initWithRectEvent(RectRaiseFunc raiseFunc_, RectEventFunc eventFunc_, const cocos2d::CCRect& hitRect_);
    void initWithUnitEvent(UnitRaiseFunc raiseFunc_, UnitEventFunc eventFunc_, BasePlaneUnit* targetUnit_);
    bool updateConditions();
private:
    union CallBackData
    {
        struct RectData
        {
            EventRect            targetRect;
            RectRaiseFunc     rectRaiseFunc;
            RectEventFunc     rectEventFunc;
        } rectData;
        struct UnitData
        {
            BasePlaneUnit*    targetUnit;
            UnitRaiseFunc     unitRaiseFunc;
            UnitEventFunc     unitEventFunc;
        } unitData;
    };
    BasePlaneUnit* m_nodeUnit;
    std::map<int, CallBackData> m_callBackDataMap;
};

class EventFactor:public Singleton<EventFactor>
{
public:
    EventFactor();
    ~EventFactor();
    
    void updateEvents();
    
    PlaneEvent* createPlaneEvent(BasePlaneUnit* nodeUnit_);
private:
    typedef std::map<int, BaseEvent*> EventMap;
    EventMap m_eventMap;
};

#endif /* defined(__plane_demo__PlaneUnit__) */
