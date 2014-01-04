#include "BaseEvent.h"
#include "cocos2d.h"
#include "../GameMain.h"
#include "PlanePoint.h"
#include "PlaneUnit.h"
#include "HeadQuarter.h"


USING_NS_CC;

void PlaneEvent::updateConditions()
{
    ConditionMap::iterator mapItr;
    
    mapItr = m_conditionMap.find(RECT_EVENT);
    if (mapItr != m_conditionMap.end() && !mapItr->second)
    {
        const EventRect& evRect = m_callBackDataMap[RECT_EVENT].rectData.targetRect;
        CCRect ccRect(evRect.x, evRect.y, evRect.width, evRect.height);
        RectRaiseFunc raisefunc = m_callBackDataMap[RECT_EVENT].rectData.rectRaiseFunc;
        if(raisefunc(m_nodeUnit, ccRect))
            mapItr->second = true;
    }
        
    mapItr = m_conditionMap.find(UNIT_EVENT);
    if (mapItr != m_conditionMap.end() && !mapItr->second)
    {
        BasePlaneUnit* targetUnit = m_callBackDataMap[UNIT_EVENT].unitData.targetUnit;
        UnitRaiseFunc raisefunc = m_callBackDataMap[UNIT_EVENT].unitData.unitRaiseFunc;
        if(raisefunc(m_nodeUnit, targetUnit))
            mapItr->second = true;
    }
}