#include "GameMain.h"
#include "SimpleAudioEngine.h"
#include "Core/Layers/MainLogicLayer.h"
#include "Core/Layers/GameLoginLayer.h"
#include "Core/PlanePoint.h"
#include "Core/PlaneUnit.h"
#include "Compent/EgCommon.h"
#include "Core/HeadQuarter.h"
#include "Base/IniReader.h"
#include "Core/Configs/IniDataConfigs.hpp"

using namespace cocos2d;
using namespace CocosDenshion;

GameMain* GameMain::SELF = NULL;

GameMain* GameMain::create()
{
    GameMain *pRet = new GameMain(); 
    if (pRet)
    {
        SELF = pRet;
        if(pRet->init())
        {
            pRet->autorelease();
            return pRet;
        }
        else
            return NULL;
    }
    else
    {
        delete pRet;
        pRet = NULL;
        return NULL;
    } 
}

GameMain::GameMain():
m_topLayer(NULL)
{}

GameMain::~GameMain()
{
    clean();
}

bool GameMain::init()
{
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    IniDataConfigs::NewInstance();
    
    m_topLayer = BaseLayer::create();
    addChild(m_topLayer->getCCLayer());
    m_topLayer->retain();
    
    BaseLayer* login = GameLoginLayer::create();
    m_topLayer->pushLayer(login);

    this->schedule( schedule_selector(GameMain::update) );
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this,0,true);
    return true;
}

void GameMain::clean()
{
    if(m_topLayer)
    {
        m_topLayer->release();
        m_topLayer = NULL;
    }
    IniDataConfigs::ReleaseInstance();
}

void GameMain::update(float dt)
{
    //PlaneUnitMgr::GetInstance()->updatePlaneUnits();
}

bool GameMain::ccTouchBegan(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{
    return false;
}

void GameMain::ccTouchMoved(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{
}

void GameMain::ccTouchEnded(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{}


void GameMain::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
