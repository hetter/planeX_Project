//
//  PlaneUnit.h
//  plane_demo
//
//  Created by dummy on 13-7-6.
//
//

#ifndef __plane_demo__PlaneUnit__
#define __plane_demo__PlaneUnit__

#include <vector>
#include <map>

#include "../GAME_DECLARATION.h"

class TroughCompent;
class BasePlanePoint;

class BasePlaneUnit
{
public:
    static const int POINT_IN_HQ = -2;
    static const int POINT_IN_START = -1;
public:
    BasePlaneUnit(const PLANE_FORCES& forces_, const PLANE_UNIT_TYPE& type_ = PLANE_UNIT_NORMAL);
    virtual ~BasePlaneUnit();
    
    void goToPoint(const int& pointIndex_);
    void goForward(const int& step_);
    
    const PLANE_FORCES& getForces() {return m_forces;}
    
    cocos2d::CCSprite* getCCSprite() {return m_sprite;}
    
    void setCCPoint (const cocos2d::CCPoint& point_);
    
    const PlaneAttribute& getAttribute() {return m_attribute;}

    void doDamage(const int& dg_);
    
    bool isDead() {return m_bIsDead;}
        
public:
    enum ATK_FUNCTION
    {
        NORMAL_ATK,
        RUNAWAY_ATK,
        STRIKE_ATK,
        KILL_ATK,
    };
    void atkOtherPlane(BasePlaneUnit* plane_, const ATK_FUNCTION& fun_);
    
private:
    typedef std::vector<cocos2d::CCFiniteTimeAction*> CCActionVec;
    
    void _pushActionVec(cocos2d::CCFiniteTimeAction* inAction_,
                                     cocos2d::CCFiniteTimeAction*& outAction_,
                                     CCActionVec& outActionVec_);
    
    void _runSeqActions(cocos2d::CCFiniteTimeAction* action_, const CCActionVec& actionVec_);
    
    cocos2d::CCFiniteTimeAction* _getEaseElasticInOutAct(const int& index_);
    enum POINT_BATTALE_FUNCTION
    {
        NORMAL_BATTALE,
        STRIKE_BATTALE,
        KILL_BATTALE,
    };
    void _checkPointBattle(BasePlanePoint* point_, const POINT_BATTALE_FUNCTION& fun_);
private:
    cocos2d::CCSprite*      m_sprite;
    PlaneAttribute              m_attribute;
    PLANE_UNIT_TYPE        m_unitType;
    int                                m_pointIndex;
    PLANE_FORCES             m_forces;
    bool                             m_bIsDead;
    TroughCompent*         m_hpTrough;
    bool                             m_bIsInGoal;
};

class PlaneUnitMgr:public Singleton<PlaneUnitMgr>
{
public:
    enum UpdateMessage
    {
        PLANE_ATK,
        PLANE_STOP,
    };
    
    void sendMessage(UpdateMessage& msg_);
private:
    typedef std::map<UpdateMessage, cocos2d::CCPoint> MsgStructMap;
    MsgStructMap m_msgStructMap;
public:
    PlaneUnitMgr();
    ~PlaneUnitMgr();
    
    bool addPlaneUnit(const PLANE_FORCES& forces_,
                                 const PLANE_UNIT_TYPE& type_,
                                 const cocos2d::CCPoint& pos_);
    
    BasePlaneUnit* checkAndGetPlaneUnit(const cocos2d::CCPoint& point_);
    BasePlaneUnit* getPlaneUnitOnRing(const int& index);
    
    bool removePlaneUnit(BasePlaneUnit* unit_);
    bool removePlaneUnitByID(const PLANE_FORCES& forces_, const int& id_);
    
    void updatePlaneEvents(const float& dt_);
    
    void updatePlaneUnits();
    typedef std::vector<BasePlaneUnit*> PlaneUnits;
    PlaneUnits getPlanes(const PLANE_FORCES& forces_) {return m_planeUnits[forces_];}
private:
    typedef std::map<int, std::string> PlanePointMap;
    PlaneUnits m_planeUnits[PLANE_FORCES_COUNT];
};
#endif /* defined(__plane_demo__PlaneUnit__) */
