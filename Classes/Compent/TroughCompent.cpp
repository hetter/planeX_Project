//
//  HpTrough.cpp
//  plane_demo
//
//  Created by dummy on 13-8-18.
//
//

#include "TroughCompent.h"

#include "cocos2d.h"

USING_NS_CC;

TroughCompent::TroughCompent(const char* sprPath_, const int& radix_)
:m_limitRadix(radix_)
,m_nowRadix(radix_)
{
    m_troughSprite = CCSprite::create(sprPath_);
    m_troughSprite->setTextureRect(CCRect(0, 0, 100, 24));
    m_troughSprite->setOpacity(80);
}

TroughCompent::~TroughCompent()
{
}

void TroughCompent::changeData(const int &data_)
{
    m_nowRadix += data_;
    if(m_nowRadix > m_limitRadix)
    {
        m_nowRadix = m_limitRadix;
        m_troughSprite->setTextureRect(CCRect(0, 0, 100, 24));
    }
    else if(m_nowRadix < 0)
    {
        m_nowRadix = 0;
        m_troughSprite->setTextureRect(CCRect(0, 0, 0, 24));
    }
    else
    {
        float rate = ((float)m_nowRadix) / ((float)m_limitRadix) * 100.0f + 0.5f;
        m_troughSprite->setTextureRect(CCRect(0, 0, ((int)rate), 24));
    }
}