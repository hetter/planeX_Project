#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "platform/platform.h"
#include "layers_scenes_transitions_nodes/CCLayer.h"
#include "Base/IniReader.h"

class MainLogicLayer;
class BaseLayer;

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
    
    BaseLayer* getTopLayer() {return m_topLayer;}
    
    void update(float dt);
    
    MainLogicLayer* getMainLogicLayer() {return m_mainLogicLayer;}
    void setMainLogicLayer(MainLogicLayer* layer_) {m_mainLogicLayer = layer_;}
private:
    MainLogicLayer* m_mainLogicLayer;
    IniReader* m_iniReader;
    BaseLayer* m_topLayer;
};

#define SGameMain (GameMain::SELF)

#define SGameMainLogic (GameMain::SELF->getMainLogicLayer())

#endif // __HELLOWORLD_SCENE_H__
