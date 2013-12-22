//
//  PlanePoint.h
//  plane_demo
//
//  Created by dummy on 13-7-1.
//
//

#ifndef __plane_demo__PlanePoint__
#define __plane_demo__PlanePoint__

#include <map>
#include <vector>

#include "../GAME_DECLARATION.h"

class BasePlanePoint
{
public:
    BasePlanePoint(cocos2d::CCSprite* sprite_,
                            const PLANE_FORCES& forces_ = PLANE_FORCES_NULL,
                            const ROAD_POINT_DIRECTION& dir_ = POINT_UP,
                            const PLANE_POINT_JUMP_TYPE& jumpType_ = NORMAL_JUMP,
                            const cocos2d::CCPoint& offsetPoint_ = cocos2d::CCPoint(0, 0));
    virtual ~BasePlanePoint();
    
    const cocos2d::CCPoint& getCCPoint();
    
    void setSpriteAndOffset(cocos2d::CCSprite* sprite_, const cocos2d::CCPoint& offsetPoint_);
    
    CC_SYNTHESIZE_PASS_BY_REF(PLANE_POINT_JUMP_TYPE, m_jumpType,  JumpType);
    CC_SYNTHESIZE_PASS_BY_REF(int, m_index,  Index);
    
    const PLANE_FORCES& getForces() {return m_pointForces;}
    const ROAD_POINT_DIRECTION& getStaticDir() {return m_staticDir;}
    
    float getPlaneStaticRot(const PLANE_FORCES& force_);
    
    typedef std::vector<BasePlaneUnit*> PlaneUnitVec;
    
    virtual PLANE_POINT_PASS_EFF getPassEffect(){return NULL_EFFECT;}
    virtual void doReachEffect(BasePlaneUnit* plane_) {}

    cocos2d::CCSprite* getCCSprite() {return m_sprite;}
protected:
    PLANE_FORCES                       m_pointForces;
    ROAD_POINT_DIRECTION       m_staticDir;
    cocos2d::CCSprite*                m_sprite;
    cocos2d::CCPoint                   m_point;
    cocos2d::CCLabelTTF*           m_labelTag;
    int                                          m_level;
};

class AtkPlanePoint:public BasePlanePoint
{
public:
    AtkPlanePoint(cocos2d::CCSprite* sprite_,
                   const PLANE_FORCES& forces_ = PLANE_FORCES_NULL,
                   const ROAD_POINT_DIRECTION& dir_ = POINT_UP,
                   const PLANE_POINT_JUMP_TYPE& jumpType_ = NORMAL_JUMP,
                   const cocos2d::CCPoint& offsetPoint_ = cocos2d::CCPoint(0, 0));
    ~AtkPlanePoint();
    virtual void doReachEffect(BasePlaneUnit* plane_);
protected:
    int m_atk;
    int m_dex;
};

class StopPlanePoint:public BasePlanePoint
{
public:
    StopPlanePoint(cocos2d::CCSprite* sprite_,
                  const PLANE_FORCES& forces_ = PLANE_FORCES_NULL,
                  const ROAD_POINT_DIRECTION& dir_ = POINT_UP,
                  const PLANE_POINT_JUMP_TYPE& jumpType_ = NORMAL_JUMP,
                  const cocos2d::CCPoint& offsetPoint_ = cocos2d::CCPoint(0, 0));
    ~StopPlanePoint();
    virtual PLANE_POINT_PASS_EFF getPassEffect();
protected:
    int m_stopFactor;
};

class PlanePointMgr:public Singleton<PlanePointMgr>
{

struct TitleDataUnit
{
    std::string texName;
    std::vector<cocos2d::CCRect> rectVec;
};
    
public:
    PlanePointMgr();
    ~PlanePointMgr();
    
    void init();
    void loadTitleFile(const std::string& name_);
    cocos2d::CCSprite* createTitleSprite(const std::string& name_);
public:
    BasePlanePoint* getRingPlanePoint(const int& index_);
    BasePlanePoint* getGoalPlanePoint(const PLANE_FORCES& forces_, const int& index_);
    cocos2d::CCPoint getOrginPos(const PLANE_FORCES& forces_);
    const int& getOrginPosIndex(const PLANE_FORCES& forces_) {return m_firstRoadPointIndex[forces_];}
    const int& getGoalPosIndex(const PLANE_FORCES& forces_) {return m_endRoadPointIndex[forces_];}
    const cocos2d::CCPoint& getPlaneRoadPos(const PLANE_FORCES& forces_, const int& ringIdex_);
    const cocos2d::CCPoint& getPlaneGoalPos(const PLANE_FORCES& forces_, const int& ringIdex_);
    
    BasePlanePoint* getKillPoint(const PLANE_FORCES& forces_);
private:
    void _addRoadPoint(const cocos2d::CCPoint&   point_,
                                   const PLANE_FORCES& head_,
                                   const ROAD_POINT_DIRECTION& dir_,
                                   const int& lenth_, bool isInvert_ = false, bool isPure_ = false);
private:
    typedef std::map<int, std::string> PlanePointMap;

    TitleDataUnit m_titleDataUnit;
    
    typedef std::vector<BasePlanePoint*> PlanePointVec;
    PlanePointVec m_ringPointsVector;
    PlanePointVec m_goalPointsVector[PLANE_FORCES_COUNT];//except PLANE_FORCES_NULL
    int m_firstRoadPointIndex[PLANE_FORCES_COUNT];
    int m_endRoadPointIndex[PLANE_FORCES_COUNT];
    int m_strikePointIndex[PLANE_FORCES_COUNT];
};

#endif /* defined(__plane_demo__PlanePoint__) */
