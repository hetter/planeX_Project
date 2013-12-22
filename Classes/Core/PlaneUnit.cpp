//
//  PlaneUnit.cpp
//  plane_demo
//
//  Created by dummy on 13-7-6.
//
//

#include "PlaneUnit.h"

#include "cocos2d.h"

#include "../Compent/EgCommon.h"
#include "PlanePoint.h"
#include "../GameMain.h"
#include "MainLogicLayer.h"
#include "HeadQuarter.h"
#include "../Compent/TroughCompent.h"

USING_NS_CC;

const float DeltaAngleTime = 0.5f;//time of rotation 90 degree
const float FlyPeriodTime = 0.2f;//time of go forward 1 gird

ROAD_POINT_DIRECTION checkDirection(const CCPoint& src_, const CCPoint& dst_)
{
    float sub = dst_.x - src_.x;
    if(sub < 0.0000001f && sub > -0.0000001f)
    {
        if(dst_.y - src_.y > 0)
            return  POINT_UP;
        else
            return POINT_DOWN;
    }
    else if(sub > 0)
        return POINT_RIGHT;
    else
        return POINT_LEFT;
}

BasePlaneUnit::BasePlaneUnit(const PLANE_FORCES& forces_, const PLANE_UNIT_TYPE& type_)
:m_forces(forces_)
,m_unitType(type_)
,m_pointIndex(POINT_IN_HQ)
,m_sprite(NULL)
,m_bIsDead(false)
,m_bIsInGoal(false)
{
    m_sprite = CCSprite::create("plane_shadow.png");
    m_sprite->setAnchorPoint(ccp(0.5f, 0.5f));   
    CCSprite* addSprite = NULL;
    switch (m_forces)
    {
        case PLANE_FORCES_GREEN:
            addSprite = CCSprite::create("plane_green.png");
            SGameMain->getMainLogicLayer()->addChild(m_sprite);
            break;
        case PLANE_FORCES_BLUE:
            addSprite = CCSprite::create("plane_blue.png");
            SGameMain->getMainLogicLayer()->addChild(m_sprite);
            break;
        case PLANE_FORCES_RED:
            addSprite = CCSprite::create("plane_red.png");
            SGameMain->getMainLogicLayer()->addChild(m_sprite);
            break;
        case PLANE_FORCES_YELLOW:
            addSprite = CCSprite::create("plane_yellow.png");
            SGameMain->getMainLogicLayer()->addChild(m_sprite);
            break;
        default:
            break;
    }
    
    m_sprite->addChild(addSprite);
    addSprite->setPosition(ccp(30, 40));
    
    const char* planeType = NULL;
    switch (m_unitType)
    {
        case PLANE_UNIT_NORMAL:
            planeType = "normal_plane";
            break;
        case PLANE_UNIT_SPEED:
            planeType = "speed_plane";
            break;
        case PLANE_UNIT_ARMER:
            planeType = "armer_plane";
            break;
        default:
            break;
    }

    m_attribute.health = SGameMainIni->getAttributeInt(planeType, "hp");
    m_attribute.atk = SGameMainIni->getAttributeInt(planeType, "atk");
    m_attribute.def = SGameMainIni->getAttributeInt(planeType, "def");
    m_attribute.dex = SGameMainIni->getAttributeInt(planeType, "dex");
    m_attribute.agi = SGameMainIni->getAttributeInt(planeType, "agi");
    
    m_hpTrough = new TroughCompent("hp_trough.png", m_attribute.health);
    m_sprite->addChild(m_hpTrough->getSprite());
    m_sprite->retain();
    m_hpTrough->getSprite()->setPosition(ccp(0 , 40));
}

BasePlaneUnit::~BasePlaneUnit()
{
    delete m_hpTrough;
    SGameMain->getMainLogicLayer()->removeChild(m_sprite);
    CC_SAFE_RELEASE(m_sprite);
}

void BasePlaneUnit::setCCPoint(const cocos2d::CCPoint& point_)
{
    m_sprite->setPosition(point_);
}

void BasePlaneUnit::goForward(const int &step_)
{
    if(m_bIsDead)
        return;
    
    if(m_pointIndex != POINT_IN_HQ)
    {
        CCFiniteTimeAction* runActions = NULL;
        CCFiniteTimeAction* rotActions = NULL;
        
        std::vector<CCFiniteTimeAction*> runActionVec;
        std::vector<CCFiniteTimeAction*> rotActionVec;
        
        bool bIsJump = false;
        int endIndex = m_pointIndex + step_ + 1;
        BasePlanePoint* nowPoint = NULL;
        BasePlanePoint* oldPoint = NULL;
        do
        {
            ROAD_POINT_DIRECTION oldDir = POINT_UNKNOW;
            if(m_pointIndex != POINT_IN_START)
            {
                if(!oldPoint)
                    oldPoint = PlanePointMgr::GetInstance()->getRingPlanePoint(m_pointIndex);
                oldDir = PlanePointMgr::GetInstance()->getRingPlanePoint(m_pointIndex)->getStaticDir();
            }
            else
            {
                m_pointIndex = PlanePointMgr::GetInstance()->getOrginPosIndex(m_forces) - 1;
                endIndex = PlanePointMgr::GetInstance()->getOrginPosIndex(m_forces) + step_;
            }
            
            bool bIsStopMark = false;
            int goSteps = 0;
            for(int i = m_pointIndex + 1;  i != endIndex; ++i )
            {
                ++goSteps;
                if(m_bIsInGoal)
                    nowPoint = PlanePointMgr::GetInstance()->getGoalPlanePoint(m_forces, i);
                else
                    nowPoint = PlanePointMgr::GetInstance()->getRingPlanePoint(i);
                
                if(nowPoint->getForces() != m_forces && nowPoint->getJumpType() != SKIP_JUMP)
                {
                    if(STOP_EFFECT == nowPoint->getPassEffect())
                    {
                        bIsStopMark = true;
                    }
                }
                
                _checkPointBattle(nowPoint, NORMAL_BATTALE);
                
                if(nowPoint->getForces() == m_forces && nowPoint->getJumpType() == GOAL_JUMP)
                {                    
                    _pushActionVec(_getEaseElasticInOutAct(i), runActions, runActionVec);
                    m_pointIndex += goSteps;
                    goSteps = 0;
                    
                    m_bIsInGoal = true;
                    
                    float r = PlanePointMgr::GetInstance()->getGoalPlanePoint(m_forces, i + 1)->getPlaneStaticRot(m_forces);
                    float periodTime = FlyPeriodTime * (i - m_pointIndex - 1);
                    if (periodTime < 0) periodTime = 0;
                    _pushActionVec(CCDelayTime::create(periodTime), rotActions, rotActionVec);
                    _pushActionVec(CCRotateTo::create(DeltaAngleTime, r), rotActions, rotActionVec);
                    if(endIndex - i - 1 > 0)
                        goSteps += endIndex - i - 1;
                    break;
                }
                
                if(nowPoint->getJumpType() == FINAL_JUMP)
                {
                    _pushActionVec(_getEaseElasticInOutAct(i), runActions, runActionVec);
                    m_pointIndex += goSteps;
                    goSteps = 0;
                    
                    if(endIndex - 1 == i)
                        m_bIsDead = true;
                    else
                        goSteps += endIndex - i - 1;
                    break;
                }
                
                if(oldDir != nowPoint->getStaticDir() && !m_bIsInGoal)
                {
                    float periodTime = FlyPeriodTime * (goSteps - 2)/ 0.8 ;
                    if (periodTime < 0) periodTime = 0;
                    _pushActionVec(CCDelayTime::create(periodTime), rotActions, rotActionVec);// speed hack
                    _pushActionVec(CCRotateTo::create(DeltaAngleTime, nowPoint->getPlaneStaticRot(m_forces)), rotActions, rotActionVec);
                    
                    int conerIndex = i;
                    if(conerIndex < 0)
                        conerIndex = 0;
                    
                    _pushActionVec(_getEaseElasticInOutAct(conerIndex), runActions, runActionVec);
                    m_pointIndex += goSteps;
                    goSteps = 0;
                }
                
                oldDir = nowPoint->getStaticDir();
                if(!bIsJump && nowPoint->getJumpType() == SKIP_JUMP)
                {
                    ++endIndex;
                }
                
                if(bIsStopMark)
                {
                    endIndex = i + 1;
                    break;
                }
            }
            

            if(!m_bIsDead)
                _pushActionVec(_getEaseElasticInOutAct(endIndex - 1), runActions, runActionVec);
            m_pointIndex += goSteps;
            goSteps = 0;
            
            if(m_bIsInGoal)
                break;
            
            if(bIsStopMark)
                break;
            
            nowPoint = PlanePointMgr::GetInstance()->getRingPlanePoint(endIndex - 1);
            _checkPointBattle(nowPoint, STRIKE_BATTALE);
            if(nowPoint->getForces() == m_forces && nowPoint->getJumpType() == SUPER_JUMP)
            {

                
                int jumpCount = 0;
                do
                {
                    nowPoint = PlanePointMgr::GetInstance()->getRingPlanePoint(endIndex++);
                    if(nowPoint->getForces() == m_forces)
                    {                        
                        if(++jumpCount == 3)
                            break;
                    }
                }while(true);
                m_pointIndex = endIndex - 3;//speed hack,see _getEaseElasticInOutAct()
                _pushActionVec(_getEaseElasticInOutAct(endIndex - 1), runActions, runActionVec);
                float periodTime = FlyPeriodTime * (endIndex - 2 - m_pointIndex);
                if (periodTime < 0) periodTime = 0;
                _pushActionVec(CCDelayTime::create(periodTime), rotActions, rotActionVec);
                _pushActionVec(CCRotateTo::create(DeltaAngleTime, nowPoint->getPlaneStaticRot(m_forces)), rotActions, rotActionVec);
                m_pointIndex = endIndex - 1;
                
                _checkPointBattle(PlanePointMgr::GetInstance()->getKillPoint(m_forces), KILL_BATTALE);
                break;
            }
            else if(!bIsJump && nowPoint->getForces() == m_forces && nowPoint->getJumpType() == NORMAL_JUMP)
            {
                bIsJump = true;
                do
                {
                    BasePlanePoint* nextPoint = PlanePointMgr::GetInstance()->getRingPlanePoint(endIndex++);
                    if(nextPoint->getForces() == m_forces)
                    {
                        break;
                    }
                }while(true);
            }
            else if(bIsJump)
                break;
        }while(bIsJump);
        
        if(nowPoint)
        {
            if(nowPoint->getForces() != m_forces)
                nowPoint->doReachEffect(this);
            
            _checkPointBattle(nowPoint, STRIKE_BATTALE);
        }
        
        m_sprite->stopAllActions();
        _runSeqActions(rotActions, rotActionVec);
        _runSeqActions(runActions, runActionVec);
    }
    else if(m_pointIndex == POINT_IN_HQ)
    {
        //if(step_ != 6)
        //    return;
        CCPoint point = PlanePointMgr::GetInstance()->getOrginPos(m_forces);
        m_pointIndex = POINT_IN_START;
        float rot = 0;
        switch (m_forces)
        {
            case PLANE_FORCES_GREEN:
                rot = 0;
                break;
            case PLANE_FORCES_BLUE:
                rot = -90;
                break;
            case PLANE_FORCES_RED:
                rot = 90;
                break;
            case PLANE_FORCES_YELLOW:
                rot = 180;
                break;
            default:
                break;
        }
        m_sprite->runAction(CCRotateTo::create(1, rot));
        m_sprite->runAction(CCMoveTo::create(1, point));
        SGameMainLogic->getHeadQuarter(m_forces)->emptyAirFieldOnce(m_sprite->getPosition());
    }
    
    if( !m_bIsInGoal && m_pointIndex >= 52)
        m_pointIndex -= 52;
    
    if(m_bIsInGoal && m_pointIndex > PlanePointMgr::GetInstance()->getGoalPosIndex(m_forces) + 6)
    {
        int overStep = m_pointIndex - (PlanePointMgr::GetInstance()->getGoalPosIndex(m_forces) + 6);
        m_pointIndex = (PlanePointMgr::GetInstance()->getGoalPosIndex(m_forces) + 6)  - overStep;
    }
    
    PlaneUnitMgr::GetInstance()->updatePlaneUnits();
}

void BasePlaneUnit::doDamage(const int& dg_)
{
    m_attribute.health -= dg_;
    
    m_hpTrough->changeData(-dg_);
    
    CCPoint nowCcp = m_sprite->getPosition();
    
    CCMoveTo*  move1 = CCMoveTo::create(0.1, ccp(nowCcp.x + 3.0f, nowCcp.y));
    CCMoveTo*  move2 = CCMoveTo::create(0.1, ccp(nowCcp.x - 3.0f, nowCcp.y));
    CCMoveTo*  move3 = CCMoveTo::create(0.1, ccp(nowCcp.x, nowCcp.y));
    CCEaseElasticInOut*  move_ease_inout1= CCEaseElasticInOut::create((CCActionInterval*)(move1->copy()->autorelease()), 0.1);
    CCEaseElasticInOut*  move_ease_inout2= CCEaseElasticInOut::create((CCActionInterval*)(move2->copy()->autorelease()), 0.1);
    CCEaseElasticInOut*  move_ease_inout3= CCEaseElasticInOut::create((CCActionInterval*)(move3->copy()->autorelease()), 0.1);
    
    CCFiniteTimeAction*  move_ease_out1 = CCEaseSineInOut::create((CCActionInterval*)(move1->copy()->autorelease()));
    CCFiniteTimeAction*  move_ease_out2 = CCEaseSineInOut::create((CCActionInterval*)(move2->copy()->autorelease()));
    CCFiniteTimeAction*  move_ease_out3 = CCEaseSineInOut::create((CCActionInterval*)(move3->copy()->autorelease()));
    
    CCFiniteTimeAction* actions= CCSequence::create(move_ease_out1,
                                                                                   move_ease_out2,
                                                                                   move_ease_inout3, NULL);

    m_sprite->runAction(actions);

    
    if(m_attribute.health <= 0)
        m_bIsDead = true;
}

void BasePlaneUnit::atkOtherPlane(BasePlaneUnit* plane_, const ATK_FUNCTION& fun_)
{
    int dexFactor = GET_DICE;
    int agiFactor = GET_DICE;
    
    int calcDex = m_attribute.dex; // normal_atk default
    int calcAgi = plane_->getAttribute().agi; // normal_atk default
    int calcAtk = m_attribute.atk;
    
    switch (fun_)
    {
        case RUNAWAY_ATK:
            calcAtk -= 1;
            calcAgi += 1;
            break;
        case STRIKE_ATK:
            calcAtk += 1;
            calcDex += 1;
            break;
        case KILL_ATK:
            calcDex += 99;
            calcAtk += 99;
        default:
            break;
    }
    
    SGameMainLogic->printfDiceLR(agiFactor + calcDex, agiFactor + calcAgi);
    if(dexFactor + calcDex > agiFactor + calcAgi)
        plane_->doDamage(calcAtk);
}

void BasePlaneUnit::goToPoint(const int &pointIndex_)
{

}

void BasePlaneUnit::_checkPointBattle(BasePlanePoint* point_, const POINT_BATTALE_FUNCTION& fun_)
{
    if( point_->getJumpType() == NULL_JUMP ||
        point_->getJumpType() == FINAL_JUMP||
        point_->getJumpType() == GOAL_BREAK_BY_ENEMY_JUMP )
        return;
    
    for(int force = 0; force < PLANE_FORCES_COUNT; ++force)
    {
        if(force == m_forces)
            continue;//if same forecs, skip
        for(int pi = 0; pi < PlaneUnitMgr::GetInstance()->getPlanes((PLANE_FORCES)force).size(); ++pi)
        {
            BasePlaneUnit* plane = PlaneUnitMgr::GetInstance()->getPlanes((PLANE_FORCES)force)[pi];
            if(plane->m_pointIndex == point_->getIndex())
            {
                if(fun_ == KILL_BATTALE)
                    atkOtherPlane(plane, KILL_ATK);
                else if(fun_ == STRIKE_BATTALE)
                    atkOtherPlane(plane, STRIKE_ATK);
                else
                {
                    atkOtherPlane(plane, NORMAL_ATK);
                    if(!plane->isDead())
                    {
                        plane->atkOtherPlane(this, RUNAWAY_ATK);
                    }
                }
            }
        }
    }
}

CCFiniteTimeAction* BasePlaneUnit::_getEaseElasticInOutAct(const int& index_)
{
    CCPoint destPoint;
    if(m_bIsInGoal)
        destPoint = PlanePointMgr::GetInstance()->getPlaneGoalPos(m_forces, index_);
    else
        destPoint = PlanePointMgr::GetInstance()->getPlaneRoadPos(PLANE_FORCES_RED, index_);
    CCActionInterval*  move = CCMoveTo::create(FlyPeriodTime * (index_ - m_pointIndex), ccp(destPoint.x, destPoint.y));
    //CCFiniteTimeAction*  move_ease_inout = CCEaseElasticInOut::create((CCActionInterval*)(move->copy()->autorelease()),
    //                                                                  FlyPeriodTime * (index_ - m_pointIndex));
    CCFiniteTimeAction*  move_ease_inout = CCEaseSineInOut::create((CCActionInterval*)(move->copy()->autorelease()));
    return move_ease_inout;
}

void BasePlaneUnit::_pushActionVec(cocos2d::CCFiniteTimeAction* inAction_,
                                                        cocos2d::CCFiniteTimeAction*& outAction_,
                                                        CCActionVec& outActionVec_)
{
    if(NULL == outAction_)
        outAction_ = inAction_;
    else
        outActionVec_.push_back(inAction_);
}

void BasePlaneUnit::_runSeqActions(cocos2d::CCFiniteTimeAction* action_, const CCActionVec& actionVec_)
{
    for(int i = 0; i < actionVec_.size(); ++i)
    {
        CCFiniteTimeAction *pNow = actionVec_[i];
        action_ = CCSequence::createWithTwoActions(action_, pNow);
    }
    action_ = CCSequence::create(action_, NULL);
    if(action_)
        m_sprite->runAction((CCSequence*) action_);
}

////////////

PlaneUnitMgr::PlaneUnitMgr()
{
    ///////////
}

PlaneUnitMgr::~PlaneUnitMgr()
{}

void PlaneUnitMgr::updatePlaneUnits()
{
    for(int i = 0; i < PLANE_FORCES_COUNT; ++i)
    {
        for(int j = 0; j < m_planeUnits[i].size(); ++j)
        {
            if(m_planeUnits[i][j]->isDead())
            {
                delete m_planeUnits[i][j];
                m_planeUnits[i].erase(m_planeUnits[i].begin() + j);
                break;
            }
        }
    }
}

BasePlaneUnit* PlaneUnitMgr::checkAndGetPlaneUnit(const cocos2d::CCPoint& point_)
{
    for(int i = 0; i < PLANE_FORCES_COUNT; ++i)
    {
        for(int j = 0; j < m_planeUnits[i].size(); ++j)
        {
            if(CheckSpriteTouch(m_planeUnits[i][j]->getCCSprite(), point_))
                return m_planeUnits[i][j];
        }
    }
    return NULL;
}

bool PlaneUnitMgr::addPlaneUnit(const PLANE_FORCES& forces_,
                                                   const PLANE_UNIT_TYPE& type_,
                                                   const cocos2d::CCPoint& pos_)
{
    int nowForcesSize = m_planeUnits[forces_].size();
    
    if(nowForcesSize >= MAX_PLANE_UNITS)
        return false;
    
    BasePlaneUnit* plane = new BasePlaneUnit(forces_, type_);
    if(forces_ == PLANE_FORCES_BLUE || forces_ == PLANE_FORCES_YELLOW)
        plane->getCCSprite()->setRotation(180);
    plane->setCCPoint(pos_);
    
    m_planeUnits[forces_].push_back(plane);

    return true;
}

void PlaneUnitMgr::updatePlaneEvents(const float& dt_)
{
    
}



