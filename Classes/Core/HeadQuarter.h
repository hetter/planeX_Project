//
//  HeadQuarter.h
//  plane_demo
//
//  Created by dummy on 13-8-22.
//
//

#ifndef __plane_demo__HeadQuarter__
#define __plane_demo__HeadQuarter__

#include "../GAME_DECLARATION.h"

class BasePlaneUnit;

class HeadQuarter
{
public:
    HeadQuarter(const PLANE_FORCES& forces_);
    ~HeadQuarter();
    bool onTouch(const cocos2d::CCPoint& touch_);
    const PLANE_FORCES& getForces() {return m_forces;}
    void emptyAirFieldOnce(const cocos2d::CCPoint& pos_);
    void addTolayer(cocos2d::CCLayer* layer_);
    
    bool isReachGameEndPoint();
    void turnStartRefresh();
private:
    struct Airfield
    {
        Airfield():bIsUsed(false){}
        cocos2d::CCPoint position;
        bool bIsUsed;
    };
    Airfield m_airfields[MAX_PLANE_UNITS];
private:
    PLANE_FORCES m_forces;
    cocos2d::CCSprite* m_hqSprite;
    BasePlaneUnit* m_UnitsInHq[MAX_PLANE_UNITS];
    
    int m_gold;
    int m_fuelOil;
    int m_fInshPlaneCount;
    int m_brokenPlaneCount;
};

#endif /* defined(__plane_demo__HeadQuarter__) */
