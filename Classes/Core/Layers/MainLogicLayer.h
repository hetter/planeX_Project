//
//  MainLogicLayer.h
//  plane_demo
//
//  Created by dummy on 13-6-22.
//
//

#ifndef __plane_demo__MainLogicLayer__
#define __plane_demo__MainLogicLayer__

#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "touch_dispatcher/CCTouchDelegateProtocol.h"

#include "../../GAME_DECLARATION.h"

#include "BaseLayer.h"

class HeadQuarter;

class MainLogicLayer: public BaseLayer, public cocos2d::CCTouchDelegate
{
public:
    MainLogicLayer();
    ~MainLogicLayer();
 
    CREATE_LAYER_FUNC(MainLogicLayer)
    
    void onEnter();
    
    // over write from CCStandardTouchDelegate
    bool ccTouchBegan   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchMoved  (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchEnded   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    
    void popCallback(cocos2d::CCNode *pNode);
    
    void nextTurn();

public:
    HeadQuarter* getHeadQuarter(const PLANE_FORCES& forces_) {return  m_headQuarters[forces_];}
public:
    cocos2d::CCLabelTTF* m_labelTTF;
    cocos2d::CCLabelTTF* m_DiceL;
    cocos2d::CCLabelTTF* m_DiceR;
    
    void printfDiceM(const int& dice);
    void printfDiceLR(const int& diceL_, const int& diceR_);
private:
    HeadQuarter* m_headQuarters[PLANE_FORCES_COUNT];
    PLANE_FORCES m_nowFrocesTurn;
};

#endif /* defined(__plane_demo__MainLogicLayer__) */
