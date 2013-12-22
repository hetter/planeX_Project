//
//  PopupDialog.cpp
//  plane_demo
//
//  Created by dummy on 13-8-12.
//
//

#include "PopupDialog.h"

#include "cocos2d.h"

USING_NS_CC;

PopupDialog::PopupDialog()
: m_pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m_sfBackGround(NULL)
//, m_s9BackGround(NULL)
, m_ltContentText(NULL)
, m_ltTitle(NULL)
{}

PopupDialog::~PopupDialog()
{
    CC_SAFE_RELEASE_NULL(m_pMenu);
    CC_SAFE_RELEASE_NULL(m_sfBackGround);
    CC_SAFE_RELEASE_NULL(m_ltContentText);
    CC_SAFE_RELEASE_NULL(m_ltTitle);
    //CC_SAFE_RELEASE_NULL(m_s9BackGround);
}

bool PopupDialog::init()
{
    bool bRef = false;
    do
    {
        CC_BREAK_IF(!CCLayer::init());
        this->setContentSize(CCSizeZero);
        
        // 初始化需要的 Menu
        CCMenu* menu = CCMenu::create();
        menu->setPosition(CCPointZero);
        m_pMenu = menu;
        
        setTouchEnabled(true);
        
        bRef = true;
    }while(0);
    return bRef;
}

void PopupDialog::registerWithTouchDispatcher()
{
    // 这里的触摸优先级设置为 -128 这保证了，屏蔽下方的触摸
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -128, true);
}

bool PopupDialog::ccTouchBegan(cocos2d::CCTouch *pTouch_, cocos2d::CCEvent *pEvent_)
{
    //
    CCLog("PopupDialog touch");
    return true;
}

PopupDialog* PopupDialog::create(const char *bgImagePath_)
{
    PopupDialog* ml = PopupDialog::create();
    ml->m_sfBackGround = CCSprite::create(bgImagePath_);
    //ml->m_s9BackGround = CCScale9Sprite::create(bgImagePath_);
    return ml;
}

void PopupDialog::setTitle(const char *title_, int fontsize_)
{
    m_ltTitle = CCLabelTTF::create(title_, "", fontsize_);
}

void PopupDialog::setContentText(const char *text_, int fontsize_, int padding_, int paddingTop_)
{
    m_ltContentText = CCLabelTTF::create(text_, "", fontsize_);
    m_contentPadding = padding_;
    m_contentPaddingTop = paddingTop_;
}

void PopupDialog::setCallbackFunc(cocos2d::CCObject *target_, SEL_CallFuncN callfun_)
{
    m_callbackListener = target_;
    m_callback = callfun_;
}


bool PopupDialog::addButton(const char *normalImagePath_, const char *selectedImagePath_,
                                              const char* title_, const int& tag_)
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    // 创建图片菜单按钮
    CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImagePath_, selectedImagePath_,
                                                                                                   this, menu_selector(PopupDialog::buttonCallback));
    menuImage->setTag(tag_);
    menuImage->setPosition(pCenter);
    
    // 添加文字说明并设置位置
    CCSize imenu = menuImage->getContentSize();
    CCLabelTTF* ttf = CCLabelTTF::create(title_, "", 20);
    ttf->setColor(ccc3(0, 0, 0));
    ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
    menuImage->addChild(ttf);
    
    m_pMenu->addChild(menuImage);
    return true;
}

void PopupDialog::buttonCallback(cocos2d::CCObject *pSender_)
{
    CCNode* node = dynamic_cast<CCNode*>(pSender_);
    CCLog("touch tag: %d", node->getTag());
    if (m_callback && m_callbackListener)
    {
       // (m_callbackListener->*m_callback)(node);
    }
    this->removeFromParent();
}

void PopupDialog::onEnter()
{
    CCLayer::onEnter();
    
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);
    
    CCSize contentSize;
    //disable scale9Size
    //if (getContentSize().equals(CCSizeZero))
    {
        m_sfBackGround->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        this->addChild(m_sfBackGround, 0, 0);
        contentSize = m_sfBackGround->getTexture()->getContentSize();
    }
//    else
//    {
//        CCScale9Sprite *background = getSprite9BackGround();
//        background->setContentSize(getContentSize());
//        background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
//        this->addChild(background, 0, 0);
//        contentSize = getContentSize();
//    }
    
    
    // 添加按钮，并设置其位置
    this->addChild(m_pMenu);
    float btnWidth = contentSize.width / (m_pMenu->getChildrenCount() + 1);
    
    CCArray* array = m_pMenu->getChildren();
    CCObject* pObj = NULL;
    int i = 0;
    CCARRAY_FOREACH(array, pObj)
    {
        CCNode* node = dynamic_cast<CCNode*>(pObj);
        node->setPosition(ccp( winSize.width / 2 - contentSize.width / 2 + btnWidth * (i + 1), winSize.height / 2 - contentSize.height / 3));
        i++;
    }
    
    
    // 显示对话框标题
    if (m_ltTitle)
    {
        m_ltTitle->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 35.0f)));
        this->addChild(m_ltTitle);
    }
    
    // 显示文本内容
    if (m_ltContentText)
    {
        m_ltContentText->setPosition(ccp(winSize.width / 2, winSize.height / 2));
        m_ltContentText->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
        m_ltContentText->setHorizontalAlignment(kCCTextAlignmentLeft);
        this->addChild(m_ltContentText);
    }
    
    // 弹出效果
    //this->setScale(0);
    //CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
    //                                          CCScaleTo::create(0.06, 1.05),
    //                                          CCScaleTo::create(0.08, 0.95),
    //                                          CCScaleTo::create(0.08, 1.0), NULL);

    CCAction* popupLayer = CCSequence::create(CCFadeIn::create(0.5f), NULL);

    m_sfBackGround->runAction(popupLayer);
    //this->runAction(popupLayer);
}

void PopupDialog::onExit()
{
    CCLog("popup on exit.");
    CCLayer::onExit();
}