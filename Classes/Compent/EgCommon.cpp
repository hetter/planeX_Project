#include "EgCommon.h"
#include "SimpleAudioEngine.h"
#include "GUI/CCEditBox/CCEditBox.h"

using namespace cocos2d;
using namespace CocosDenshion;
USING_NS_CC_EXT;

//==============================================
GLDrawLayer::GLDrawLayer()
{
    m_drawRectStart = ccp(0, 0);
    m_drawRectEnd = ccp(0, 0);
}

GLDrawLayer::~GLDrawLayer()
{}

void GLDrawLayer::move(const cocos2d::CCPoint &offset_)
{
   m_drawRectStart = ccpAdd(m_drawRectStart, offset_);
   m_drawRectEnd = ccpAdd(m_drawRectEnd, offset_);
}

void GLDrawLayer::setDrawRectStart(const cocos2d::CCPoint &start_)
{
    m_drawRectStart = start_;
}

void GLDrawLayer::setDrawRectEnd(const cocos2d::CCPoint &end_)
{
    m_drawRectEnd = end_;
}

void GLDrawLayer::setDrawRect(const cocos2d::CCRect& rect_)
{
    m_drawRectStart = rect_.origin;
    m_drawRectEnd = ccp(rect_.origin.x + rect_.size.width, rect_.origin.y - rect_.size.height);
}

void GLDrawLayer::setDrawPoint(const cocos2d::CCPoint &point_)
{
    float cx = (m_drawRectEnd.x + m_drawRectStart.x)/2;
    float cy = (m_drawRectEnd.y + m_drawRectStart.y)/2;
    CCPoint offset(point_.x - cx, point_.y - cy);
    m_drawRectStart.x += offset.x;
    m_drawRectStart.y += offset.y;
    m_drawRectEnd.x += offset.x;
    m_drawRectEnd.y += offset.y;
}

cocos2d::CCRect GLDrawLayer::getNowRect()
{
    int rx = MIN(m_drawRectStart.x, m_drawRectEnd.x);
    int ry = MAX(m_drawRectStart.y, m_drawRectEnd.y);
    int w = abs(m_drawRectStart.x - m_drawRectEnd.x);
    int h = abs(m_drawRectStart.y - m_drawRectEnd.y);
    return CCRect(rx, ry, w, h);
}

void GLDrawLayer::draw()
{
    ccDrawColor4B(155, 200, 200, 200);
    glLineWidth(1);
    ccDrawRect(m_drawRectStart, m_drawRectEnd);
}
//==============================================

IconListComponent::IconListComponent(const cocos2d::CCPoint& firstIconPos_,
                                     const cocos2d::CCSize& iconSize_,
                                     const int& maxShow_ /* = 8 */)
:m_iconSize(iconSize_), m_firstIconPos(firstIconPos_), m_maxShow(maxShow_),
m_maxPage(0),m_nowPage(0)
{
    m_layer = CreatePureColorSpr(0, 0, 0xffffffff);
    m_layer->retain();
    
    m_showList = CCArray::create();
    m_showList->retain();
    
    m_trueList = CCArray::create();
    m_trueList->retain();
}

IconListComponent::~IconListComponent()
{
    m_layer->release();
    m_showList->release();
    m_trueList->release();
}

void IconListComponent::addList(cocos2d::CCSprite* spr_, bool bIsDetail)
{
    int showIndex = m_trueList->count() - m_maxPage * m_maxShow;
    bool bIsAddPage = false;
    if(showIndex >= m_maxShow)
    {
        showIndex -= m_maxShow;
        bIsAddPage = true;
        ++m_maxPage;
    }
    
    cocos2d::CCRect rect;
    rect.size = m_iconSize;
    rect.origin.x = m_firstIconPos.x;
    rect.origin.y = m_firstIconPos.y - showIndex * m_iconSize.height;
    
    int color = 0xE0F0F0F0;
    if (showIndex % 2)
    {
        color = 0xE0C0C0C0;
    }
    
    CCSprite* addIcon = NULL;
    
    if(bIsDetail)
    {
        addIcon = CreatePureColorSpr(rect.size.width, rect.size.height, color);

        addIcon->addChild(spr_);
        ScaleAndMoveCCSprite2Rect(spr_, rect);
        spr_->setPosition(ccp(addIcon->getTextureRect().size.width/2, addIcon->getTextureRect().size.height/2));
    }
    else
    {
        addIcon = CCSprite::createWithTexture(spr_->getTexture(), spr_->getTextureRect());        
    }
    
    addIcon->setPosition(ccp(rect.origin.x + rect.size.width/2, rect.origin.y - rect.size.height/2));
    m_trueList->addObject(addIcon);
    
    if (m_nowPage == m_maxPage)
    {
        m_showList->addObject(addIcon);
        m_layer->addChild(addIcon);
    }
    else
    {
        m_nowPage = m_maxPage;
        _goPage(m_maxPage);
    }
}

void IconListComponent::swapIndex(const int& trueIndex1_, const int& trueIndex2_)
{
    m_trueList->exchangeObjectAtIndex(trueIndex1_, trueIndex2_);
    CCNode* node1 = (CCNode*)m_trueList->objectAtIndex(trueIndex1_);
    CCNode* node2 = (CCNode*)m_trueList->objectAtIndex(trueIndex2_);
    
    CCPoint temp = node1->getPosition();
    node1->setPosition(node2->getPosition());
    node2->setPosition(temp);
    
    _goPage(m_nowPage);
}

void IconListComponent::copyIndex(const int& showIndex_)
{   
    CCSprite* cpy =(CCSprite*) ( (CCSprite*) m_trueList->objectAtIndex(getTrueIndexByShowIndex(showIndex_)))->getChildren()->objectAtIndex(0);
    
    addList(CCSprite::createWithTexture(cpy->getTexture(), cpy->getTextureRect()));
}

void IconListComponent::updateIcon(const int& showIndex_)
{
    
}

void IconListComponent::goLeftPage()
{
    if (m_nowPage == 0)
        return;
    --m_nowPage;
    _goPage(m_nowPage);
}

void IconListComponent::goRightPage()
{
    if (m_nowPage == m_maxPage)
        return;
    ++m_nowPage;
    _goPage(m_nowPage);
}

void IconListComponent::clean()
{
    m_trueList->removeAllObjects();
    m_showList->removeAllObjects();
    m_layer->removeAllChildren();
    m_nowPage = 0;
}

int IconListComponent::getTrueIndexByShowIndex(const int& showIndex_)
{
    return (showIndex_ + m_nowPage * m_maxShow);
}

int IconListComponent::getShowIndexByTrueIndex(const int& trueIndex_)
{
    int ret = (trueIndex_ - m_nowPage * m_maxShow);
    if( ret < m_maxShow &&  ret >= 0)
        return ret;
    else
        return -1;
}

const cocos2d::CCPoint& IconListComponent::getIconPositionByShowIndex(const int& showIndex_)
{
    return ((CCNode*)m_showList->objectAtIndex(showIndex_))->getPosition();
}

bool IconListComponent::deletIconByShowIndex(const int& showIndex_)
{
    if(m_showList->count() > 0 && showIndex_ < m_showList->count())
    {
        int trueIndex = getTrueIndexByShowIndex(showIndex_);
        return deletIconByTrueIndex(trueIndex);
    }
    return false;
}

bool IconListComponent::deletIconByTrueIndex(const int& trueIndex_)
{
    if(m_trueList->count() > 0 && trueIndex_ < m_trueList->count())
    {
        CCNode* delNode = (CCNode*) m_trueList->objectAtIndex(trueIndex_);
        CCPoint lastPoint = delNode->getPosition();
        
        for(int i = trueIndex_ + 1; i < m_trueList->count(); ++i)
        {
            CCPoint setPoint = lastPoint;
            CCNode* checkNode = (CCNode*) m_trueList->objectAtIndex(i);
            lastPoint = checkNode->getPosition();
            checkNode->setPosition(setPoint);
        }
        
        m_layer->removeChild(delNode, false);
        m_trueList->removeObjectAtIndex(trueIndex_);
        
        int showIndex = getShowIndexByTrueIndex(trueIndex_);
        if(showIndex >= 0)
        {
            m_showList->removeObjectAtIndex(showIndex);
            
            if (m_maxPage == m_nowPage && m_showList->count() == 0 && m_nowPage != 0)
            {
                --m_maxPage;
                --m_nowPage;
                _goPage(m_nowPage);
            }
            else if (m_maxPage > m_nowPage)
            {
                if (m_trueList->count() == m_maxPage * m_maxShow)
                    --m_maxPage;
                _goPage(m_nowPage);
            }
        }        
        return true;
    }
    return false;
}

void IconListComponent::_goPage(const int& page_)
{
    m_showList->removeAllObjects();
    m_layer->removeAllChildren();
    
    int start = page_ * m_maxShow;
    int end = start + m_maxShow;
    if(end > m_trueList->count())
        end = m_trueList->count();
    for (int i = start; i != end; ++i)
    {
        CCSprite* spr = (CCSprite*)m_trueList->objectAtIndex(i);
        m_showList->addObject(spr);
        m_layer->addChild(spr);
    }    
}
//==============================================

void CCMenuItemImageFont::setFont(const char* str_, const int& size_)
{
    CCSprite* imgSpr = (CCSprite*)m_pNormalImage;
    float imgW = imgSpr->getTextureRect().size.width;
    float imgH = imgSpr->getTextureRect().size.height;

    m_labelTTF = CCLabelTTF::create(str_, "微软雅黑", size_);
    m_labelTTF->setColor(ccc3(60, 233, 60));
    
    
    float strW = m_labelTTF->getTextureRect().size.width + m_labelTTF->getTextureRect().size.width * 0.25f;
    float strH = m_labelTTF->getTextureRect().size.height + m_labelTTF->getTextureRect().size.height * 0.25f;
    
    
    setScaleX(strW / imgW);
    setScaleY(strH / imgH);
    
    addChild(m_labelTTF, 1);
    
    m_labelTTF->setScaleY(imgH / strH);
    m_labelTTF->setScaleX(imgW/ strW);
    m_labelTTF->setPosition(ccp(imgW/2, imgH/2 - m_labelTTF->getFontSize()*0.05f));
    m_labelPos = m_labelTTF->getPosition();
}

CCMenuItemImageFont* CCMenuItemImageFont::create()
{
    CCMenuItemImageFont *pRet = new CCMenuItemImageFont();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

CCMenuItemImageFont* CCMenuItemImageFont::create(const char *normalImage, const char *selectedImage,
                                                                                      cocos2d::CCObject *target, SEL_MenuHandler selector)
{
    CCMenuItemImageFont *pRet = create();
    pRet->initWithNormalImage(normalImage, selectedImage, NULL, target, selector);
    return pRet;
}


void CCMenuItemImageFont::selected()
{
    CCMenuItemImage::selected();
    m_labelTTF->setPosition(ccpAdd(m_labelPos, ccp(0 , -m_labelTTF->getFontSize()*0.05f)));
}

void CCMenuItemImageFont::unselected()
{
    CCMenuItemImage::unselected();
    m_labelTTF->setPosition(m_labelPos);
}


//==============================================

bool floatEqu(const float& f1, const float& f2)
{
    float sub = f1 - f2;
    if(sub < 0.0000001f && sub > -0.0000001f)
        return true;
    else
        return false;
}

std::string GetResPath(const char* res_)
{
    std::string ret = SOURCE_ROOT;
    ret += res_;
    return ret;
}

#ifdef _WIN32
std::string G2U(const char* gb2312)
{
    int len = MultiByteToWideChar(CP_ACP, 0, gb2312, -1, NULL, 0);
    wchar_t* wstr = new wchar_t[len+1];
    memset(wstr, 0, len+1);
    MultiByteToWideChar(CP_ACP, 0, gb2312, -1, wstr, len);
    len = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_UTF8, 0, wstr, -1, str, len, NULL, NULL);
    if(wstr) delete[] wstr;
    std::string ret(str);
    delete[] str;
    return ret;
}

std::string W2C(const wchar_t* src_)
{
    int len = WideCharToMultiByte(CP_UTF8, 0, src_, -1, NULL, 0, NULL, NULL);
    char* str = new char[len+1];
    memset(str, 0, len+1);
    WideCharToMultiByte(CP_ACP, 0, src_, -1, str, len, NULL, NULL);
    std::string ret = str;
    delete [] str;
    return ret;
}
#endif

CCSprite* CreatePureColorSpr(const int& w_, const int& h_, const unsigned int& col_)
{
    GLubyte alpha = (col_ & 0xff000000) >> 24;
    GLubyte r = (col_ & 0x00ff0000) >> 16;
    GLubyte g = (col_ & 0x0000ff00) >> 8;
    GLubyte b = (col_ & 0x000000ff) >> 0;
    
    GLubyte* pixels;
    int totalBytes = h_ * w_ * 3;//rgb888
    pixels = new GLubyte[totalBytes];
    
    for (int i = 0; i < h_; ++i)
    {
        for (int j = 0; j < w_*3; j += 3)
        {
            int index = i * w_ *3 + j;
            pixels[index] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
        }
    }
    
    CCTexture2D *texture = new CCTexture2D();
    texture->initWithData(pixels, kCCTexture2DPixelFormat_RGB888, 1, 1, CCSizeMake(w_, h_));
    
    CCSprite* spr = CCSprite::createWithTexture(texture);
    spr->setOpacity(alpha);
    texture->release();
    delete [] pixels;
    
    return spr;
}

cocos2d::CCRect MakeRect(const int& x1, const int& y1, const int& x2, const int& y2)
{
    int rx = MIN(x1, x2);
    int ry = MAX(y1, y2);
    int w = abs(x1 - x2);
    int h = abs(y1 - y2);
    return CCRect(rx, ry, w, h);
}

void ScaleAndMoveCCSprite2Rect(cocos2d::CCSprite* spr_, const cocos2d::CCRect& rect_)
{
    float scale = 1.0f;
    float hSub = spr_->getTextureRect().size.height / rect_.size.height;
    float wSub = spr_->getTextureRect().size.width / rect_.size.width;
    if(hSub >= 1.0 && hSub >= wSub)
        scale = rect_.size.height / spr_->getTextureRect().size.height;
    else if(wSub >= 1.00 && wSub >= hSub)
        scale = rect_.size.width / spr_->getTextureRect().size.width;
    spr_->setScale(scale);
    int posX = rect_.origin.x + rect_.size.width/2;
    int posY = rect_.origin.y - rect_.size.height/2;
    spr_->setPosition(ccp(posX, posY));
}

void GetEdge1DPoint(const float& p1, const float& p2, const float& pi, float& outp)
{
    if(pi < p1)
        outp = p1;
    else if(pi > p2)
        outp = p2;
    else
        outp = pi;
}

void GetEdge2DPoint(const CCPoint& p1, const CCPoint& p2, const CCPoint& pi, CCPoint& outp)
{
    GetEdge1DPoint(p1.x, p2.x, pi.x, outp.x);
    GetEdge1DPoint(p1.y, p2.y, pi.y, outp.y);
}

void SetBoxTextByVar(const int& var_, cocos2d::extension::CCEditBox* box_)
{
    const int boxChars = 15;
    char toBox[boxChars] = {0};
    sprintf(toBox, "%d",  var_);
    box_->setText(toBox);
}

CCMenuItem* AddFontBtn(const char *value_, CCObject* target_, SEL_MenuHandler selector_, const CCPoint& pos_,
                                                CCMenu* menu_, const int& fontSize_)
{

    
    CCMenuItemImageFont* addBtn = CCMenuItemImageFont::create(RES_PATH("btn01.png"),
                                                              RES_PATH("btn02.png"),
                                                              target_,
                                                              selector_);
    addBtn->setPosition(pos_);
    addBtn->setFont(value_, fontSize_);
    menu_->addChild(addBtn);
    return addBtn;
}

bool CheckSpriteTouch(cocos2d::CCSprite* spr_, const cocos2d::CCPoint& touch_)
{
    CCPoint pPoint =  spr_->getPosition();
    CCSize  pSize = spr_->getContentSize();
    CCRect  pRect = CCRectMake(pPoint.x - pSize.width / 2, pPoint.y - pSize.height / 2,
                               pSize.width, pSize.height);
    if(pRect.containsPoint(touch_))
        return true;
    else
        return false;
}

cocos2d::CCSprite* CheckSpriteArrTouch(cocos2d::CCArray* sprArray_, const cocos2d::CCPoint& touch_,
                                                                int& outIndex_, bool bIsNewSpr_ )
{
    for (int i = 0; i < sprArray_->count(); ++i)
    {
        CCSprite* pSprite = dynamic_cast<CCSprite*>(sprArray_->objectAtIndex(i));
        if (pSprite)
        {
            if(CheckSpriteTouch(pSprite, touch_))
            {
                outIndex_ = i;
                if(bIsNewSpr_)
                {
                    CCSprite* newSprite = CCSprite::createWithTexture(pSprite->getTexture(), pSprite->getTextureRect());
                    return newSprite;
                }
                return pSprite;
            }
        }
    }
    outIndex_ = -1;
    return NULL;
}

void AddSprArrToLayer(cocos2d::CCArray* sprArray_, cocos2d::CCLayer* lay_)
{
    if(!sprArray_ || !lay_)
        return;
    for (int i = 0; i < sprArray_->count(); ++i)
    {
        CCSprite* pSprite = dynamic_cast<CCSprite*>(sprArray_->objectAtIndex(i));
        if(pSprite)
            lay_->addChild(pSprite);
    }
}

void RemoveSprArrFromLayer(cocos2d::CCArray* sprArray_, cocos2d::CCLayer* lay_)
{
    if(!sprArray_ || !lay_)
        return;
    for (int i = 0; i < sprArray_->count(); ++i)
    {
        CCSprite* pSprite = dynamic_cast<CCSprite*>(sprArray_->objectAtIndex(i));
        if(pSprite)
            lay_->removeChild(pSprite);
    }
}
