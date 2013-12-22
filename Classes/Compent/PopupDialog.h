//
//  PopupLayer.h
//  plane_demo
//
//  Created by dummy on 13-8-12.
//
//


#ifndef __plane_demo__PopupLayer__
#define __plane_demo__PopupLayer__

#include "layers_scenes_transitions_nodes/CCLayer.h"

#include "../GAME_DECLARATION.h"

class PopupDialog: public cocos2d::CCLayer
{
public:
    PopupDialog();
    ~PopupDialog();
    
    virtual bool init();
    CREATE_FUNC(PopupDialog);
    
    virtual void registerWithTouchDispatcher(void);
    bool ccTouchBegan(cocos2d::CCTouch *pTouch_, cocos2d::CCEvent *pEvent_);
    
    static PopupDialog* create(const char* bgImagePath_);
    
    void setTitle(const char* title_, int fontsize_ = 20);
    void setContentText(const char* text_, int fontsize_ = 20, int padding_ = 50, int paddintTop_ = 100);
    
    void setCallbackFunc(cocos2d::CCObject* target_, cocos2d::SEL_CallFuncN callfun_);
    
    bool addButton(const char* normalImagePath_, const char* selectedImagePath_,
                             const char* title_, const int& tag_ = 0);
    virtual void onEnter();
    virtual void onExit();
    
    void buttonCallback(CCObject* pSender_);
    
private:

    
    // font empty
    int m_contentPadding;
    int m_contentPaddingTop;
    
    CCObject* m_callbackListener;
    cocos2d::SEL_CallFuncN m_callback;
    
    cocos2d::CCMenu* m_pMenu;
    cocos2d::CCSprite* m_sfBackGround;
    //cocos2d::CCScale9Sprite* m_s9BackGround;
    cocos2d::CCLabelTTF* m_ltTitle;
    cocos2d::CCLabelTTF* m_ltContentText;
};

#endif