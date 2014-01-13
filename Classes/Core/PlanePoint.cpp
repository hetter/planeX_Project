//
//  PlanePoint.cpp
//  plane_demo
//
//  Created by dummy on 13-7-1.
//
//

#include "PlanePoint.h"

#include <fstream>
#include <sstream>

#include "cocos2d.h"

#include "../GameMain.h"
#include "layers/MainLogicLayer.h"
#include "PlaneUnit.h"
#include "../Base/IniReader.h"

USING_NS_CC;

float POINT_WIDTH  = 50.0f;
float POINT_HEIGHT = 50.0f;

BasePlanePoint::BasePlanePoint(CCSprite* sprite_,
                                                 const PLANE_FORCES& forces_,
                                                 const ROAD_POINT_DIRECTION& dir_,
                                                 const PLANE_POINT_JUMP_TYPE& jumpType_,
                                                 const CCPoint& offsetPoint_)
:m_pointForces(forces_)
,m_jumpType(jumpType_)
,m_sprite(sprite_)
,m_staticDir(dir_)
,m_index(-1)
{
    GetPointMgr->getParentLayer()->addChild(m_sprite);
    m_point = ccpAdd(m_sprite->getPosition(), offsetPoint_);
    m_labelTag = CCLabelTTF::create("", "微软雅黑", 25);
    m_labelTag->setColor(ccc3(160, 20, 60));
    m_labelTag->setPosition(ccp(POINT_WIDTH/2, POINT_HEIGHT/2));
    m_sprite->addChild(m_labelTag);
}

BasePlanePoint::~BasePlanePoint()
{
    m_sprite->removeFromParent();
    m_sprite->release();
}

const cocos2d::CCPoint& BasePlanePoint::getCCPoint()
{
    return m_point;
}

void BasePlanePoint::setSpriteAndOffset(cocos2d::CCSprite* sprite_, const cocos2d::CCPoint& offsetPoint_)
{
        if(m_sprite)
        {
            m_sprite->removeFromParent();
            m_sprite->release();
        }
        m_sprite = sprite_;
        m_point = ccpAdd(m_sprite->getPosition(), offsetPoint_);
}

float BasePlanePoint::getPlaneStaticRot(const PLANE_FORCES& force_)
{
    float rot = 0;//PLANE_FORCES_RED
    if(force_ == PLANE_FORCES_BLUE)
        rot = 0;
    else if(force_ == PLANE_FORCES_YELLOW)
        rot = 0;
    else if(force_ == PLANE_FORCES_GREEN)
        rot = 0;
    switch (m_staticDir)
    {
        case POINT_UP:
            rot += 0;
            break;
        case POINT_DOWN:
            rot += 180;
            break;
        case POINT_RIGHT:
            rot += 90;
            break;
        case POINT_LEFT:
            rot += -90;
            break;
        default:
            break;
    }
    return rot;
}

/////////////////////////////////////////

AtkPlanePoint::AtkPlanePoint(CCSprite* sprite_,
                                             const PLANE_FORCES& forces_,
                                             const ROAD_POINT_DIRECTION& dir_,
                                             const PLANE_POINT_JUMP_TYPE& jumpType_,
                                             const CCPoint& offsetPoint_)
:BasePlanePoint(sprite_, forces_, dir_, jumpType_, offsetPoint_)
{
    m_atk = GetIniConfigs["BattleConfig"].getAttributeInt("atk_point", "atk");
    m_dex = GetIniConfigs["BattleConfig"].getAttributeInt("atk_point", "dex");
    m_labelTag->setString("");
}

AtkPlanePoint::~AtkPlanePoint()
{}

void AtkPlanePoint::doReachEffect(BasePlaneUnit *plane_)
{
    int d1 = GET_DICE;
    int d2 = GET_DICE;
    SGameMainLogic->printfDiceLR(d1 + m_dex, d2 + plane_->getAttribute().agi);
    if(d1 + m_dex > d2 + plane_->getAttribute().agi)
        plane_->doDamage(m_atk);
}

/////////////////////////////////////////

StopPlanePoint::StopPlanePoint(CCSprite* sprite_,
                             const PLANE_FORCES& forces_,
                             const ROAD_POINT_DIRECTION& dir_,
                             const PLANE_POINT_JUMP_TYPE& jumpType_,
                             const CCPoint& offsetPoint_):
BasePlanePoint(sprite_, forces_, dir_, jumpType_, offsetPoint_)
{
    m_stopFactor = GetIniConfigs["BattleConfig"].getAttributeInt("stop_point", "stop");
    m_labelTag->setString("");
}

StopPlanePoint::~StopPlanePoint()
{}

PLANE_POINT_PASS_EFF StopPlanePoint::getPassEffect()
{
    int dice = GET_DICE;
    SGameMainLogic->printfDiceLR(dice, m_stopFactor);
    if(dice <= m_stopFactor)
        return STOP_EFFECT;
    else
        return NULL_EFFECT;
}


/////////////////////////////////////////
PlanePointMgr::PlanePointMgr()
:m_parentLayer(NULL)
{
}

PlanePointMgr::~PlanePointMgr()
{
}

void PlanePointMgr::init(CCLayer* parentLayer_)
{
    float point_width  = POINT_WIDTH;
    float point_height = POINT_HEIGHT;
    
    point_width = 50;//m_titleDataUnit.rectVec[RED_POINT_ATK].size.width;
    point_height = 50;//m_titleDataUnit.rectVec[RED_POINT_ATK].size.height;
    
    m_parentLayer = parentLayer_;

    IniReader* iniReader = new IniReader;
    std::string batConfigfullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("mapConfig.ini");
    iniReader->loadIniFile(batConfigfullPath.c_str());
    for(IniReader::IniGroup::iterator i = iniReader->m_iniGroupMap.begin();
        i != iniReader->m_iniGroupMap.end(); ++i)
    {
        float x = atof(i->second.find("x")->second.c_str());
        float y = atof(i->second.find("y")->second.c_str());

        int count = atoi(i->second.find("count")->second.c_str());

        std::string col = i->second.find("col")->second;
        PLANE_FORCES forces = PLANE_FORCES_RED;
        if (col == "blue")
            forces = PLANE_FORCES_BLUE;
        else if (col == "green")
            forces = PLANE_FORCES_GREEN;
        else if (col == "yellow")
            forces = PLANE_FORCES_YELLOW;

        ROAD_POINT_DIRECTION dirEnum = POINT_UP;
        std::string dir;
        if(i->second.find("dir") != i->second.end())
            dir = i->second.find("dir")->second;
        if (dir == "down")
            dirEnum = POINT_DOWN;
        else if (dir == "right")
            dirEnum = POINT_RIGHT;
        else if (dir == "left")
            dirEnum = POINT_LEFT;

        bool isGoalMark = false;
        if(i->second.find("goal") != i->second.end())
        {
           if (i->second.find("goal")->second == "true")
               isGoalMark = true;
        }

        _addRoadPoint(ccp(point_width * x,  point_height * y), forces, dirEnum, count, isGoalMark, isGoalMark);
    }
    delete iniReader;
    iniReader = NULL;

    if(m_ringPointsVector.size() <= 50)
        return;
        
    int pointTotal = -1;
    
    m_firstRoadPointIndex[PLANE_FORCES_RED] = pointTotal + 1;
    //_addRoadPoint(ccp(point_width * 5.25f, point_height * 1.6f), PLANE_FORCES_RED, POINT_UP, 3);
    pointTotal += 3;
    //_addRoadPoint(ccp(point_width * 4.75f,  point_height * 4.5f), PLANE_FORCES_GREEN, POINT_LEFT, 3);
    //_addRoadPoint(ccp(point_width * 2.0f,  point_height * 5.15f), PLANE_FORCES_BLUE, POINT_LEFT, 1);
    pointTotal += 4;
    m_ringPointsVector[pointTotal - 3]->setJumpType(SUPER_JUMP);
    //_addRoadPoint(ccp(point_width * 1.5f, point_height * 6.0f), PLANE_FORCES_GREEN, POINT_UP, 5);
    //_addRoadPoint(ccp(point_width * 2.0f, point_height * 10.85f), PLANE_FORCES_RED, POINT_UP, 1);
    pointTotal += 6;
    m_ringPointsVector[pointTotal - 3]->setJumpType(GOAL_JUMP);
    m_endRoadPointIndex[PLANE_FORCES_YELLOW] = pointTotal - 3;
    //_addRoadPoint(ccp(point_width * 2.5f,  point_height * 8.0f), PLANE_FORCES_YELLOW, POINT_RIGHT, 6, true, true);
    
    m_firstRoadPointIndex[PLANE_FORCES_YELLOW] = pointTotal + 1;
    //_addRoadPoint(ccp(point_width * 2.75f, point_height * 11.5f), PLANE_FORCES_YELLOW, POINT_RIGHT, 3);
    pointTotal += 3;
    //_addRoadPoint(ccp(point_width * 5.25f, point_height * 12.4f), PLANE_FORCES_RED, POINT_UP, 3);
    //_addRoadPoint(ccp(point_width * 5.7f, point_height * 15.3f), PLANE_FORCES_GREEN, POINT_UP, 1);
    pointTotal += 4;
    m_ringPointsVector[pointTotal - 3]->setJumpType(SUPER_JUMP);
    //_addRoadPoint(ccp(point_width * 6.4f, point_height * 16.0f), PLANE_FORCES_RED, POINT_RIGHT, 5);
    //_addRoadPoint(ccp(point_width * 11.05f, point_height * 15.3f), PLANE_FORCES_YELLOW, POINT_RIGHT, 1);
    pointTotal += 6;
    m_ringPointsVector[pointTotal - 3]->setJumpType(GOAL_JUMP);
    m_endRoadPointIndex[PLANE_FORCES_BLUE] = pointTotal - 3;
    //_addRoadPoint(ccp(point_width * 8.4f,  point_height * 15.0f), PLANE_FORCES_BLUE, POINT_DOWN, 6, true, true);
    
    m_firstRoadPointIndex[PLANE_FORCES_BLUE] = pointTotal + 1;
    //_addRoadPoint(ccp(point_width * 11.55f, point_height * 14.4f), PLANE_FORCES_BLUE, POINT_DOWN, 3);
    pointTotal += 3;
    //_addRoadPoint(ccp(point_width * 12.1f,  point_height * 11.5f), PLANE_FORCES_YELLOW, POINT_RIGHT, 3);
    //_addRoadPoint(ccp(point_width * 14.85f,  point_height * 10.85f), PLANE_FORCES_RED, POINT_RIGHT, 1);
    pointTotal += 4;
    m_ringPointsVector[pointTotal - 3]->setJumpType(SUPER_JUMP);
    //_addRoadPoint(ccp(point_width * 15.45f, point_height * 10.0f), PLANE_FORCES_YELLOW, POINT_DOWN, 5);
    //_addRoadPoint(ccp(point_width * 14.85f, point_height * 5.15f), PLANE_FORCES_BLUE, POINT_DOWN, 1);
    pointTotal += 6;
    m_ringPointsVector[pointTotal - 3]->setJumpType(GOAL_JUMP);
    m_endRoadPointIndex[PLANE_FORCES_GREEN] = pointTotal - 3;
    //_addRoadPoint(ccp(point_width * 14.45f,  point_height * 8.0f), PLANE_FORCES_GREEN, POINT_LEFT, 6, true, true);
   
    m_firstRoadPointIndex[PLANE_FORCES_GREEN] = pointTotal + 1;
    //_addRoadPoint(ccp(point_width * 14.1f, point_height * 4.5f), PLANE_FORCES_GREEN, POINT_LEFT, 3);
    pointTotal += 3;
    //_addRoadPoint(ccp(point_width * 11.55f,  point_height * 3.6f), PLANE_FORCES_BLUE, POINT_DOWN, 3);
    //_addRoadPoint(ccp(point_width * 10.95f,  point_height * 0.7f), PLANE_FORCES_YELLOW, POINT_DOWN, 1);
    pointTotal += 4;
    m_ringPointsVector[pointTotal - 3]->setJumpType(SUPER_JUMP);
    //_addRoadPoint(ccp(point_width * 10.4f, point_height * -0.05f), PLANE_FORCES_BLUE, POINT_LEFT, 5);
    //_addRoadPoint(ccp(point_width * 5.7f, point_height * 0.7f), PLANE_FORCES_GREEN, POINT_UP, 1);
    pointTotal += 6;
    m_ringPointsVector[pointTotal - 3]->setJumpType(GOAL_JUMP);
    m_endRoadPointIndex[PLANE_FORCES_RED] = pointTotal - 3;
    //_addRoadPoint(ccp(point_width * 8.4f,  point_height * 0.95f), PLANE_FORCES_RED, POINT_UP, 6, true, true);
}

BasePlanePoint* PlanePointMgr::getRingPlanePoint(const int &index_)
{
    if(index_ >= m_ringPointsVector.size())
        return m_ringPointsVector[index_ - m_ringPointsVector.size()];
    else
        return m_ringPointsVector[index_];
}

BasePlanePoint* PlanePointMgr::getGoalPlanePoint(const PLANE_FORCES& forces_, const int& index_)
{
    int checkIndex = index_ - 1;
    if(checkIndex + 1 == m_endRoadPointIndex[forces_])
        return getRingPlanePoint(m_endRoadPointIndex[forces_]);
    else if(checkIndex >= m_endRoadPointIndex[forces_])
        checkIndex -= m_endRoadPointIndex[forces_];
    if(checkIndex >= m_goalPointsVector[forces_].size())
    {
        int overStep = checkIndex - m_goalPointsVector[forces_].size() + 1;
        checkIndex = m_goalPointsVector[forces_].size() - overStep - 1;
    }
    
    return m_goalPointsVector[forces_][checkIndex];
}

cocos2d::CCPoint PlanePointMgr::getOrginPos(const PLANE_FORCES& forces_)
{
    int checkIndex = m_firstRoadPointIndex[forces_];
    CCPoint orign = m_ringPointsVector[checkIndex]->getCCPoint();
    switch (forces_)
    {
        case PLANE_FORCES_GREEN:
            orign.y -= POINT_HEIGHT;
            break;
        case PLANE_FORCES_BLUE:
            orign.x += POINT_WIDTH;
            break;
        case PLANE_FORCES_RED:
            orign.x -= POINT_WIDTH;
            break;
        case PLANE_FORCES_YELLOW:
            orign.y += POINT_HEIGHT;
            break;
        default:
            break;
    }
    return orign;
}

const cocos2d::CCPoint& PlanePointMgr::getPlaneRoadPos(const PLANE_FORCES& forces_, const int& ringIndex_)
{
    int checkIndex = m_firstRoadPointIndex[forces_] + ringIndex_;
    if(checkIndex >= m_ringPointsVector.size())
        checkIndex -= m_ringPointsVector.size();
    return m_ringPointsVector[checkIndex]->getCCPoint();
}

const cocos2d::CCPoint& PlanePointMgr::getPlaneGoalPos(const PLANE_FORCES& forces_, const int& ringIndex_)
{
    return getGoalPlanePoint(forces_, ringIndex_)->getCCPoint();
}

BasePlanePoint* PlanePointMgr::getKillPoint(const PLANE_FORCES& forces_)
{
    PLANE_FORCES dest = PLANE_FORCES_NULL;
    switch (forces_)
    {
        case PLANE_FORCES_GREEN:
            dest = PLANE_FORCES_YELLOW;
            break;
        case PLANE_FORCES_YELLOW:
            dest = PLANE_FORCES_GREEN;
            break;
        case PLANE_FORCES_RED:
            dest = PLANE_FORCES_BLUE;
            break;
        case PLANE_FORCES_BLUE:
            dest = PLANE_FORCES_RED;
        default:
            break;
    }
    return getGoalPlanePoint(dest, m_strikePointIndex[dest]);
}

int COLOR_POINT[4] = {0};//for test
void PlanePointMgr::_addRoadPoint(const cocos2d::CCPoint&   point_,
                                                       const PLANE_FORCES& head_,
                                                       const ROAD_POINT_DIRECTION& dir_,
                                                       const int& lenth_, bool isInvert_, bool isPure_)

{
    float point_width  = 0;
    float point_height = 0;
    


    if(isPure_)
    {
        point_width = 50;
        point_height = 50;
    }
    else
    {
        point_width = 50;
        point_height = 50;
    }
    
    CCPoint offset;
    switch (dir_)
    {
        case POINT_UP:
            offset = ccp(0, point_height);
            break;
        case POINT_DOWN:
            offset = ccp(0, -point_height);
            break;
        case POINT_LEFT:
            offset = ccp(-point_width, 0);
            break;
        case POINT_RIGHT:
            offset = ccp(point_width, 0);
            break;
        default:
            break;
    }
    
    int nowForces = head_;
    int targetIndex = 0;
    for (int i = 0; i < lenth_;  i ++)
    {        
        if(!isPure_)
        {
            CCSprite* sprite = NULL;
            switch (nowForces)
            {
                case PLANE_FORCES_GREEN:
                    if(COLOR_POINT[nowForces] % 2)
                        sprite = CCSprite::createWithSpriteFrameName("green_point_atk.png");
                    else
                        sprite = CCSprite::createWithSpriteFrameName("green_point_stop.png");
                    break;
                case PLANE_FORCES_RED:
                    if(COLOR_POINT[nowForces] % 2)
                        sprite = CCSprite::createWithSpriteFrameName("red_point_atk.png");
                    else
                        sprite = CCSprite::createWithSpriteFrameName("red_point_stop.png");;
                    break;
                case PLANE_FORCES_YELLOW:
                    if(COLOR_POINT[nowForces] % 2)
                        sprite = CCSprite::createWithSpriteFrameName("yellow_point_atk.png");
                    else
                       sprite = CCSprite::createWithSpriteFrameName("yellow_point_stop.png");
                    break;
                case PLANE_FORCES_BLUE:
                    if(COLOR_POINT[nowForces] % 2)
                        sprite = CCSprite::createWithSpriteFrameName("blue_point_atk.png");
                    else
                        sprite = CCSprite::createWithSpriteFrameName("blue_point_stop.png");
                    break;
            }
            ++COLOR_POINT[nowForces];
            
            sprite->setPosition(ccp(offset.x * i + point_.x, offset.y * i + point_.y));
            BasePlanePoint* bpPoint = NULL;
            if((COLOR_POINT[nowForces] - 1) % 2)
                bpPoint = new AtkPlanePoint(sprite, (PLANE_FORCES)nowForces, dir_);
            else
                bpPoint = new StopPlanePoint(sprite, (PLANE_FORCES)nowForces, dir_);

            bpPoint->setIndex(m_ringPointsVector.size());
            m_ringPointsVector.push_back(bpPoint);

            if(isInvert_)
            {
                if(PLANE_FORCES_GREEN == nowForces)
                    nowForces = PLANE_FORCES_BLUE;
                    else
                        --nowForces;
            }
            else
                ++nowForces;
        }
        else
        {
            CCSprite* sprite = NULL;
            switch (nowForces)
            {
                case PLANE_FORCES_GREEN:
                    sprite = CCSprite::createWithSpriteFrameName("green_goal.png");
                    break;
                case PLANE_FORCES_RED:
                    sprite = CCSprite::createWithSpriteFrameName("red_goal.png");
                    break;
                case PLANE_FORCES_YELLOW:
                    sprite = CCSprite::createWithSpriteFrameName("yellow_goal.png");
                    break;
                case PLANE_FORCES_BLUE:
                    sprite = CCSprite::createWithSpriteFrameName("blue_goal.png");
                    break;
            }
            
            BasePlanePoint* bpPoint = NULL;

            sprite->setPosition(ccp(offset.x * i + point_.x, offset.y * i + point_.y));
            

            if(i == 2)
            {
                m_strikePointIndex[nowForces] = i;
                bpPoint = new BasePlanePoint(sprite, (PLANE_FORCES)nowForces, dir_, GOAL_BREAK_BY_ENEMY_JUMP);
            }
            else if(i != lenth_ - 1)
                bpPoint = new BasePlanePoint(sprite, (PLANE_FORCES)nowForces, dir_, NULL_JUMP);
            else
                bpPoint = new BasePlanePoint(sprite, (PLANE_FORCES)nowForces, dir_, FINAL_JUMP);
            //if(i != lenth_ - 1)
            //    bpPoint = new BasePlanePoint(offset.x * i + point_.x, offset.y * i + point_.y, nowForces, NULL_JUMP);
            //else
            //    bpPoint = new BasePlanePoint(offset.x * i + point_.x, offset.y * i + point_.y, nowForces, FINAL_JUMP);
            bpPoint->setIndex(m_goalPointsVector[nowForces].size());
            m_goalPointsVector[nowForces].push_back(bpPoint);
        }
        
        if (PLANE_FORCES_COUNT == nowForces)
            nowForces = PLANE_FORCES_GREEN;
    }
}