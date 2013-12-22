#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "platform/platform.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "Base/IniReader.h"

class MainLogicLayer;
class IniReader;

class GameMain : public cocos2d::CCLayer
{
public:
    static GameMain* SELF;
public:
    GameMain();
    ~GameMain();
    
    // Method 'init' in cocos2d-x returns bool, instead of 'id' in cocos2d-iphone (an object pointer)
    virtual bool init();
    
    void clean();
    
    // a selector callback
    void menuCloseCallback(CCObject* pSender);

    // preprocessor macro for "static create()" constructor ( node() deprecated )
    static GameMain* create();
    
    // over write from CCStandardTouchDelegate
    bool ccTouchBegan   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchMoved  (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    void ccTouchEnded   (cocos2d::CCTouch* touch_, cocos2d:: CCEvent* event_);
    
    MainLogicLayer* getMainLogicLayer() {return m_mainLogicLayer;}
    void setMainLogicLayer(MainLogicLayer* logic_) {m_mainLogicLayer = logic_;}
    
    IniReader* getIniReader() {return m_iniReader;}
    
    void update(float dt);
private:
    MainLogicLayer* m_mainLogicLayer;
    IniReader* m_iniReader;
};

#define SGameMain (GameMain::SELF)

#define SGameMainLogic (GameMain::SELF->getMainLogicLayer())

#define SGameMainIni (GameMain::SELF->getIniReader())

#endif // __HELLOWORLD_SCENE_H__
