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

#include "../GAME_DECLARATION.h"

class HeadQuarter;

class MainLogicLayer: public cocos2d::CCLayer
{
public:
    MainLogicLayer();
    ~MainLogicLayer();
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    void clean();
    
    // there's no 'id' in cpp, so we recommend to return the class instance pointer
    static cocos2d::CCScene* scene();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_FUNC(MainLogicLayer)
    
    // over write from CCStandardTouchDelegate
    bool ccTouchBegan   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchMoved  (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchEnded   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    
    void popCallback(cocos2d::CCNode *pNode);

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
};

#endif /* defined(__plane_demo__MainLogicLayer__) */
