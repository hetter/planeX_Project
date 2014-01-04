//
//  HpTrough.h
//  plane_demo
//
//  Created by dummy on 13-8-18.
//
//

#ifndef __plane_demo__TroughCompent__
#define __plane_demo__TroughCompent__

#include "../GAME_DECLARATION.h"

class TroughCompent
{
public:
    TroughCompent(const char* sprPath_, const int& radix_);
    ~TroughCompent();
    
    void changeData(const int& data_);
    
    cocos2d::CCSprite* getSprite() {return m_troughSprite;}
protected:
    cocos2d::CCSprite* m_troughSprite;
    int m_limitRadix;
    int m_nowRadix;
};

#endif /* defined(__plane_demo__HpTrough__) */
