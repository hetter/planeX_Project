#include "GameMain.h"
#include "SimpleAudioEngine.h"
#include "Core/MainLogicLayer.h"
#include "Core/PlanePoint.h"
#include "Core/PlaneUnit.h"
#include "Compent/EgCommon.h"
#include "Core/HeadQuarter.h"
#include "Base/IniReader.h"

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

GameMain::GameMain()
{}

GameMain::~GameMain()
{
    clean();
}

bool GameMain::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    std::string batConfigfullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("battleConfig.ini");
    m_iniReader = new IniReader();
    m_iniReader->loadIniFile(batConfigfullPath.c_str());


    // add a "close" icon to exit the progress. it's an autorelease object
    //CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
    //                                                      "CloseNormal.png",
    //                                                      "CloseSelected.png",
    //                                                      this,
    //                                                      menu_selector(GameMain::menuCloseCallback) );
    //pCloseItem->setPosition( ccp(CCDirector::sharedDirector()->getWinSize().width - 20, 20) );
    
    // create menu, it's an autorelease object
    //CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
    //pMenu->setPosition( CCPointZero );
    //this->addChild(pMenu, 1);
    
    MainLogicLayer::create();
    addChild(m_mainLogicLayer);
    
    PlanePointMgr::NewInstance();
    PlaneUnitMgr::NewInstance();
    

    
    this->schedule( schedule_selector(GameMain::update) );
    
    return true;
}

void GameMain::clean()
{
    if(m_mainLogicLayer)
    {
        m_mainLogicLayer->release();
        m_mainLogicLayer = NULL;
    }
    PlanePointMgr::ReleaseInstance();
    PlaneUnitMgr::ReleaseInstance();
    delete m_iniReader;
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
