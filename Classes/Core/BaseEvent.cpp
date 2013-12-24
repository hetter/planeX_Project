#include "BaseEvent.h"
#include "cocos2d.h"
#include "../GameMain.h"
#include "PlanePoint.h"
#include "PlaneUnit.h"
#include "HeadQuarter.h"


using namespace cocos2d;

PlaneEvent::PlaneEvent(BasePlaneUnit* nodeUnit_)
:m_nodeUnit(nodeUnit_)
{}

PlaneEvent::~PlaneEvent()
{}

void PlaneEvent::initWithRectEvent(RectRaiseFunc raiseFunc_, RectEventFunc eventFunc_, const cocos2d::CCRect &hitRect_)
{
    CallBackData cbd;
    cbd.rectData.rectRaiseFunc = raiseFunc_;
    cbd.rectData.rectEventFunc = eventFunc_;
    EventRect evRect;
    evRect.x = hitRect_.origin.x;
    evRect.y = hitRect_.origin.y;
    evRect.width = hitRect_.size.width;
    evRect.height = hitRect_.size.height;
    cbd.rectData.targetRect = evRect;
    
    m_conditionMap[RECT_EVENT] = false;
    m_callBackDataMap[RECT_EVENT] = cbd;
}

void PlaneEvent::initWithUnitEvent(UnitRaiseFunc raiseFunc_, UnitEventFunc eventFunc_, BasePlaneUnit *targetUnit_)
{
    CallBackData cbd;
    cbd.unitData.unitRaiseFunc = raiseFunc_;
    cbd.unitData.unitEventFunc = eventFunc_;
    cbd.unitData.targetUnit = targetUnit_;
    
    m_conditionMap[UNIT_EVENT] = false;
    m_callBackDataMap[UNIT_EVENT] = cbd;
}

bool PlaneEvent::updateConditions()
{
    ConditionMap::iterator mapItr;
    
    mapItr = m_conditionMap.find(RECT_EVENT);
    if (mapItr != m_conditionMap.end() && !mapItr->second)
    {
        EventRect evRect = m_callBackDataMap[RECT_EVENT].rectData.targetRect;
        CCRect ccRect(evRect.x, evRect.y, evRect.width, evRect.height);
        RectRaiseFunc raisefunc = m_callBackDataMap[RECT_EVENT].rectData.rectRaiseFunc;
        if(raisefunc(m_nodeUnit, ccRect))
        {
            RectEventFunc eventfunc = m_callBackDataMap[RECT_EVENT].rectData.rectEventFunc;
            mapItr->second = true;
            eventfunc(m_nodeUnit, ccRect);
        }
    }
        
    mapItr = m_conditionMap.find(UNIT_EVENT);
    if (mapItr != m_conditionMap.end() && !mapItr->second)
    {
        BasePlaneUnit* targetUnit = m_callBackDataMap[UNIT_EVENT].unitData.targetUnit;
        UnitRaiseFunc raisefunc = m_callBackDataMap[UNIT_EVENT].unitData.unitRaiseFunc;
        if(raisefunc(m_nodeUnit, targetUnit))
        {
            UnitEventFunc eventfunc = m_callBackDataMap[UNIT_EVENT].unitData.unitEventFunc;
            mapItr->second = true;
            eventfunc(m_nodeUnit, targetUnit);
        }
    }
    
    mapItr = m_conditionMap.begin();
    while(mapItr != m_conditionMap.end())
    {
        if (mapItr->second)
        {
            m_conditionMap.erase(mapItr++);
        }
        else
            ++mapItr;
    }
    
    if (m_conditionMap.empty())
    {
        m_bIsEventDead = true;
    }
    
    return m_bIsEventDead;
}

////////////////////////////////////////////////////////////

EventFactor::EventFactor()
{}

EventFactor::~EventFactor()
{}

PlaneEvent* EventFactor::createPlaneEvent(BasePlaneUnit *nodeUnit_)
{
    PlaneEvent* event = new PlaneEvent(nodeUnit_);
   // temp key
    m_eventMap[m_eventMap.size()] = event;
    return event;
}

void EventFactor::updateEvents()
{
    EventMap::iterator mapItr = m_eventMap.begin();
    while (mapItr != m_eventMap.end())
    {
        bool bIsClose = mapItr->second->updateConditions();
        if (bIsClose)
        {
            delete mapItr->second;
            mapItr->second = NULL;
            m_eventMap.erase(mapItr++);
        }
        else
            ++mapItr;
    }
}
