//
//  BaseLayer.h
//  PlaneX
//
//  Created by dummy on 14-1-2.
//
//

#ifndef __PlaneX__BaseLayer__
#define __PlaneX__BaseLayer__

#include <vector>

#include "layers_scenes_transitions_nodes/CCLayer.h"

#include "../../GAME_DECLARATION.h"

#define CREATE_LAYER_FUNC(__TYPE__) \
static __TYPE__* create() \
{ \
__TYPE__ *pRet = new __TYPE__(); \
if (pRet) \
{ \
pRet->autorelease(); \
return pRet; \
} \
else \
{ \
delete pRet; \
pRet = NULL; \
return NULL; \
} \
}

class BaseLayer: public cocos2d::CCObject
{
public:
    BaseLayer();
    ~BaseLayer();
    
    // preprocessor macro for "static create()" constructor ( node() deprecated )
    CREATE_LAYER_FUNC(BaseLayer)
    
    void pushLayer(BaseLayer* layer_);
    void closeLayer();
    void clearLayers();
    bool removeChildLayer(BaseLayer* layer_);
    
    cocos2d::CCLayer* getCCLayer() {return m_CCLayer;}
protected:
   
    void virtual onEnter();
    void virtual onLeave();
    
     void _addChild(BaseLayer* layer_);
protected:
    cocos2d::CCLayer* m_CCLayer;
    BaseLayer* m_parentLayer;
    typedef std::vector<BaseLayer*> LayerVec;
    LayerVec m_layerStack;
};



#endif /* defined(__PlaneX__BaseLayer__) */
