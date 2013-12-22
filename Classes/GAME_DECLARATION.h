//
//  CC2D_INCLUDE.h
//  plane_demo
//
//  Created by dummy on 13-7-6.
//
//

#ifndef plane_demo_CC2D_INCLUDE_h
#define plane_demo_CC2D_INCLUDE_h

#include "platform/CCPlatformMacros.h"
#include "cocoa/CCGeometry.h"
#include "ccMacros.h"

#include "Base/Singleton.hpp"

NS_CC_BEGIN
class CCSprite;
class CCMenu;
class CCLabelTTF;
class CCNode;
class CCObject;
class CCFiniteTimeAction;
class CCScene;

typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
NS_CC_END

class BasePlaneUnit;

enum PLANE_FORCES
{
    PLANE_FORCES_GREEN = 0,
    PLANE_FORCES_RED,
    PLANE_FORCES_YELLOW,
    PLANE_FORCES_BLUE,
    PLANE_FORCES_COUNT,
    ////////////////////////
    PLANE_FORCES_NULL,
};

enum ROAD_POINT_DIRECTION
{
    POINT_UP,
    POINT_DOWN,
    POINT_LEFT,
    POINT_RIGHT,
    POINT_UNKNOW,
};

enum PLANE_POINT_JUMP_TYPE
{
    NULL_JUMP,
    SKIP_JUMP,
    NORMAL_JUMP,
    SUPER_JUMP,
    FINAL_JUMP,
    GOAL_JUMP,
    GOAL_BREAK_BY_ENEMY_JUMP,
    JUMP_COUNT,
};

enum PLANE_POINT_PASS_EFF
{
    NULL_EFFECT,
    STOP_EFFECT,
};

struct PlaneAttribute
{
    float health;
    float atk;
    float def;
    float dex;
    float agi;
};

enum PLANE_UNIT_TYPE
{
    PLANE_UNIT_NORMAL,
    PLANE_UNIT_SPEED,
    PLANE_UNIT_ARMER,
    PLANE_UNIT_COUNT,
};

const int MAX_PLANE_UNITS = 4;



#define GET_DICE (int)(CCRANDOM_0_1() * 6 + 1)

//shell output
#define RED_POINT_ATK 0 
#define RED_POINT_STOP 1 
#define RED_GOAL 2 
#define BLUE_POINT_ATK 3 
#define BLUE_POINT_STOP 4 
#define BLUE_GOAL 5 
#define YELLOW_POINT_ATK 6 
#define YELLOW_POINT_STOP 7 
#define YELLOW_GOAL 8 
#define GREEN_POINT_ATK 9 
#define GREEN_POINT_STOP 10 
#define GREEN_GOAL 11 

#endif
