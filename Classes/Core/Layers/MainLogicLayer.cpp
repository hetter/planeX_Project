//
//  MainLogicLayer.cpp
//  plane_demo
//
//  Created by dummy on 13-6-22.
//
//

#include "MainLogicLayer.h"

#include "cocos2d.h"
#include "../../GameMain.h"
#include "../PlanePoint.h"
#include "../PlaneUnit.h"
#include "../HeadQuarter.h"
#include "../BaseEvent.h"
#include "../../Compent/PopupDialog.h"

USING_NS_CC;



MainLogicLayer::MainLogicLayer()
{
    PlanePointMgr::NewInstance();
    PlaneUnitMgr::NewInstance();
}

MainLogicLayer::~MainLogicLayer()
{
    PlanePointMgr::ReleaseInstance();
    PlaneUnitMgr::ReleaseInstance();
    
    CCSpriteFrameCache::sharedSpriteFrameCache()->removeSpriteFramesFromFile("GameElement.plist");
}

// on "init" you need to initialize your instance
void MainLogicLayer::onEnter()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("GameElement.plist", "GameElement.png");
    
    // ask director the window size
    CCSize size = CCDirector::sharedDirector()->getWinSize();
        
    //this->schedule(schedule_selector(MainEditLayer::updateScene));
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this,0,true);
    
    m_labelTTF = CCLabelTTF::create("0", "微软雅黑", 100);
    m_labelTTF->setColor(ccc3(160, 20, 60));
    m_labelTTF->setPosition(ccp(size.width/2 - 150, size.height/2 - 220));
    m_CCLayer->addChild(m_labelTTF, 10);
    
    m_DiceL = CCLabelTTF::create("0", "微软雅黑", 100);
    m_DiceL->setColor(ccc3(160, 20, 60));
    m_DiceL->setPosition(ccp(size.width/2 - 150, size.height/2 - 50));
    m_CCLayer->addChild(m_DiceL, 10);
    
    m_DiceR = CCLabelTTF::create("0", "微软雅黑", 100);
    m_DiceR->setColor(ccc3(160, 20, 60));
    m_DiceR->setPosition(ccp(size.width/2 + 150, size.height/2 - 50));
    m_CCLayer->addChild(m_DiceR, 10);
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pointCenter = ccp(winSize.width / 2, winSize.height / 2);
    

    //        popupLayer();
    
    
    // 添加菜单
    CCMenu* menu = CCMenu::create();
    
    CCMenuItemFont* menuItem = CCMenuItemFont::create("popup", this, 0);
    menuItem->setPosition(ccp(200, 50));
    menuItem->setColor(ccc3(0, 0, 0));
    menu->addChild(menuItem);
    
    
    menu->setPosition(CCPointZero);
    m_CCLayer->addChild(menu);
    
    m_nowFrocesTurn = PLANE_FORCES_RED;
    m_headQuarters[PLANE_FORCES_BLUE] = new HeadQuarter(PLANE_FORCES_BLUE);
    m_headQuarters[PLANE_FORCES_BLUE]->addTolayer(m_CCLayer);
    m_headQuarters[PLANE_FORCES_YELLOW] = new HeadQuarter(PLANE_FORCES_YELLOW);
    m_headQuarters[PLANE_FORCES_YELLOW]->addTolayer(m_CCLayer);
    m_headQuarters[PLANE_FORCES_RED] = new HeadQuarter(PLANE_FORCES_RED);
    m_headQuarters[PLANE_FORCES_RED]->addTolayer(m_CCLayer);
    m_headQuarters[PLANE_FORCES_GREEN] = new HeadQuarter(PLANE_FORCES_GREEN);
    m_headQuarters[PLANE_FORCES_GREEN]->addTolayer(m_CCLayer);
    
//    std::string fullPath = CCFileUtils::sharedFileUtils()->fullPathForFilename("battleConfig.ini");
//    IniReader* ini = new IniReader();
//    ini->loadIniFile(fullPath.c_str());
//    int a = ini->getAttributeInt("stop_point", "stop");
    
    //init a default position
    m_CCLayer->setPosition(ccp(20, 210));
    
    GetPointMgr->init(m_CCLayer);
    GetUnitMgr->init(m_CCLayer);
}

void MainLogicLayer::nextTurn()
{
    m_nowFrocesTurn = (PLANE_FORCES) ((int)m_nowFrocesTurn + 1);
    if (m_nowFrocesTurn == PLANE_FORCES_COUNT)
    {
        m_nowFrocesTurn = (PLANE_FORCES) 0;
    }
    m_headQuarters[m_nowFrocesTurn]->turnStartRefresh();
}

void MainLogicLayer::printfDiceLR(const int& diceL_, const int& diceR_)
{
    char num[10] = {0};
    sprintf(num, "%d", diceL_);
    m_DiceL->setString(num);
    memset(num, 0, 10);
    sprintf(num, "%d", diceR_);
    m_DiceR->setString(num);
}

void MainLogicLayer::popCallback(cocos2d::CCNode *pNode)
{
    CCLog("button call back. tag: %d", pNode->getTag());
}

bool MainLogicLayer::ccTouchBegan(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{
//    // 定义一个弹出层，传入一张背景图
//    PopupDialog* pl = PopupDialog::create("BackGround.png");
//   // pl->retain();
//    // ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
//    pl->setContentSize(CCSizeMake(400, 350));
//    pl->setTitle("title");
//    pl->setContentText("this is a dialog message box", 20, 60, 250);
//    // 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
//    // 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
//    pl->setCallbackFunc(this, callfuncN_selector(MainLogicLayer::popCallback));
//    // 添加按钮，设置图片，文字，tag 信息
//    pl->addButton("pop_button.png", "pop_button.png", "OK", 0);
//    pl->addButton("pop_button.png", "pop_button.png", "CANCEL", 1);
//    // 添加到当前层
//    this->addChild(pl);
//    
//    return true;
    
    CCPoint relPoint = ccpSub(touch_->getLocation(), m_CCLayer->getPosition());
    
    BasePlaneUnit* unit = PlaneUnitMgr::GetInstance()->checkAndGetPlaneUnit(relPoint);
    if(unit && unit->getForces() == m_nowFrocesTurn)
    {
        int randNumber = 5;//GET_DICE;
        char num[10] = {0};
        sprintf(num, "%d", randNumber);
        m_labelTTF->setString(num);
        unit->goForward(randNumber);
        nextTurn();
        return false;
    }
    
    //for(int i = 0; i < PLANE_FORCES_COUNT; ++i)
    {
        int i = m_nowFrocesTurn;
        if(m_headQuarters[i]->onTouch(relPoint))
            return false;
    }
    //this->registerWithTouchDispatcher();
    return true;
}

void MainLogicLayer::ccTouchMoved(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{
    CCPoint offset = touch_->getDelta();
    m_CCLayer->setPosition(ccpAdd(m_CCLayer->getPosition(), offset));
}

void MainLogicLayer::ccTouchEnded(cocos2d::CCTouch *touch_, cocos2d::CCEvent *event_)
{

}
