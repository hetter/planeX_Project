//
//  GameLoginLayer.h
//  plane_demo
//
//  Created by dummy on 14-1-1.
//
//

#ifndef __plane_demo__GameLoginLayer__
#define __plane_demo__GameLoginLayer__

#include "layers_scenes_transitions_nodes/CCLayer.h"

#include "../../GAME_DECLARATION.h"

#include "BaseLayer.h"


class GameLoginLayer: public BaseLayer
{
public:
    GameLoginLayer();
    ~GameLoginLayer();
    
    CREATE_LAYER_FUNC(GameLoginLayer)
    
    //btn call back function
public:
    void loginBtnCallback(CCObject* sender);

protected:
    void onEnter();
};

#endif /* defined(__plane_demo__GameLoginLayer__) */
