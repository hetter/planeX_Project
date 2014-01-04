//
//  GameLoginLayer.cpp
//  plane_demo
//
//  Created by dummy on 13-6-22.
//
//


#include "GameLoginLayer.h"

#include "cocos2d.h"
#include "cocos-ext.h"
#include "../../GameMain.h"
#include "MainLogicLayer.h"


USING_NS_CC;
USING_NS_CC_EXT;

GameLoginLayer::GameLoginLayer()
{
}

GameLoginLayer::~GameLoginLayer()
{}

// on "init" you need to initialize your instance
void GameLoginLayer::onEnter()
{
    UILayer* uiLayer = UILayer::create();
    uiLayer->scheduleUpdate();
    
    UILayout* ui_root =dynamic_cast<UILayout*>(GUIReader::shareReader()->widgetFromJsonFile("plane_1.json"));
    uiLayer->addWidget(ui_root);
    
    UIButton *left_button = dynamic_cast<UIButton*>(uiLayer->getWidgetByName("TextButton_34"));
    left_button->addReleaseEvent(this, SEL_ReleaseEvent(&GameLoginLayer::loginBtnCallback));
    
    m_CCLayer->addChild(uiLayer);
}

void GameLoginLayer::loginBtnCallback(CCObject* sender)
{
    MainLogicLayer* layer_ = MainLogicLayer::create();
    SGameMain->setMainLogicLayer(layer_);
    SGameMain->getTopLayer()->pushLayer(layer_);
    closeLayer();
}