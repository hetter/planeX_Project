//
//  HeadQuarter.cpp
//  plane_demo
//
//  Created by dummy on 13-8-22.
//
//

#include "HeadQuarter.h"

#include "cocos2d.h"
#include "../Compent/EgCommon.h"
#include "../GameMain.h"
#include "MainLogicLayer.h"
#include "PlaneUnit.h"

USING_NS_CC;

HeadQuarter::HeadQuarter(const PLANE_FORCES& forces_)
:m_forces(forces_)
{
    extern float POINT_WIDTH;
    extern float POINT_HEIGHT;
    
    float girdX = 2.5f;
    float girdY = 2.5f;
    float girdXStep = 1.0f;
    float girdYStep = 1.0f;
    
    CCPoint hqPoint;
    
    switch(m_forces)
    {
        case PLANE_FORCES_BLUE:
            m_hqSprite = CCSprite::create("headquarter_blue.png");
            hqPoint = ccp(13 * POINT_WIDTH, 13 * POINT_HEIGHT);
            m_hqSprite->setPosition(hqPoint);
            
            girdX = 12.7f;
            girdY = 12.7f;
            girdXStep = -girdXStep;
            girdYStep = -girdYStep;
            break;
        case PLANE_FORCES_RED:
            m_hqSprite = CCSprite::create("headquarter_red.png");
            hqPoint = ccp(1.5 * POINT_WIDTH, 1.5 * POINT_HEIGHT);
            m_hqSprite->setPosition(hqPoint);
            
            girdX = 1.8f;
            girdY = 1.8f;
            break;
        case PLANE_FORCES_YELLOW:
            m_hqSprite = CCSprite::create("headquarter_yellow.png");
            hqPoint = ccp(1.5 * POINT_WIDTH, 13 * POINT_HEIGHT);
            m_hqSprite->setPosition(hqPoint);
            
            girdX = 1.8f;
            girdY = 12.7f;
            girdYStep = -girdYStep;
            break;
        case PLANE_FORCES_GREEN:
            m_hqSprite = CCSprite::create("headquarter_green.png");
            hqPoint = ccp(13 * POINT_WIDTH, 1.5 * POINT_HEIGHT);
            m_hqSprite->setPosition(hqPoint);
            
            girdX = 12.7f;
            girdY = 1.8f;
            girdXStep = -girdXStep;
            break;
    }
    
    for(int i = 0; i < MAX_PLANE_UNITS; ++i)
    {
//        switch (i)
//        {
//            case 1:
//                girdX -= girdXStep;
//                break;
//            case  2:
//                girdY -= girdYStep;
//                break;
//            case  3:
//                girdX += girdXStep;
//                // girdY += girdYStep;
//            default:
//                break;
//        }
        
       // m_airfields[i].position = ccp(POINT_WIDTH * girdX, POINT_WIDTH * girdY);
        
        CCPoint office;
        float gapFactory = 0.7f;
        float yOfficeFactory = 0.9f;
        
        int planeIndex = i;
        if(PLANE_FORCES_BLUE == m_forces|| PLANE_FORCES_YELLOW == m_forces) //upside
            planeIndex = MAX_PLANE_UNITS - i - 1;
        switch (planeIndex)
        {
            case 0:
                office.x += POINT_WIDTH * gapFactory;
                office.y += POINT_HEIGHT * gapFactory;
                break;
            case 1:
                office.x -= POINT_WIDTH * gapFactory;
                office.y += POINT_HEIGHT * gapFactory;
                break;
            case  2:
                office.x += POINT_WIDTH * gapFactory;
                office.y -= POINT_HEIGHT * gapFactory * yOfficeFactory;
                break;
            case  3:
                office.x -= POINT_WIDTH * gapFactory;
                office.y -= POINT_HEIGHT * gapFactory * yOfficeFactory;
                break;
        }

        
        m_airfields[i].position = ccpAdd(hqPoint, office);
    }
    
    SGameMain->getMainLogicLayer()->addChild(m_hqSprite);
}

HeadQuarter::~HeadQuarter()
{
    SGameMain->getMainLogicLayer()->removeChild(m_hqSprite);
}

void HeadQuarter::emptyAirFieldOnce(const cocos2d::CCPoint& pos_)
{
    for(int i = MAX_PLANE_UNITS - 1; i > -1; --i)
    {
        CCPoint subPoint = ccpSub(m_airfields[i].position, pos_);
        if(floatEqu(subPoint.x, 0.0f) && floatEqu(subPoint.y, 0.0f))
        {
            m_airfields[i].bIsUsed = false;
            break;
        }
    }
}

bool HeadQuarter::onTouch(const cocos2d::CCPoint &touch_)
{
    if(CheckSpriteTouch(m_hqSprite, touch_))
    {        
        for(int i = 0; i < MAX_PLANE_UNITS; ++i)
        {
            if(!m_airfields[i].bIsUsed)
            {
                if(PlaneUnitMgr::GetInstance()->addPlaneUnit(m_forces, PLANE_UNIT_NORMAL, m_airfields[i].position))
                    m_airfields[i].bIsUsed = true;
                break;
            }
        }
        
        return true;
    }
    else
        return false;
}

