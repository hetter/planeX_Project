//
//  BaseLayer.cpp
//  PlaneX
//
//  Created by dummy on 14-1-2.
//
//

#include "BaseLayer.h"

#include "cocos2d.h"

USING_NS_CC;

BaseLayer::BaseLayer()
:m_parentLayer(NULL)
{
    m_CCLayer = CCLayer::create();
    m_CCLayer->retain();
}

BaseLayer::~BaseLayer()
{
    clearLayers();
    m_CCLayer->removeFromParent();
    m_CCLayer->release();
}

void BaseLayer::pushLayer(BaseLayer* layer_)
{
    layer_->m_parentLayer = this;
    layer_->retain();
    _addChild(layer_);
    m_layerStack.push_back(layer_);
    layer_->onEnter();
}

void BaseLayer::clearLayers()
{
    LayerVec::iterator itr = m_layerStack.begin();
    while (itr != m_layerStack.end())
    {
        BaseLayer* layer = (*itr);
        layer->release();
        ++itr;
    }
    m_layerStack.clear();
}

bool BaseLayer::removeChildLayer(BaseLayer* layer_)
{
    LayerVec::iterator itr = m_layerStack.begin();
    while (itr != m_layerStack.end())
    {
        BaseLayer* layer = (*itr);
        if(layer == layer_)
        {
            m_layerStack.erase(itr);
            layer->release();
            return true;
        }
        ++itr;
    }
    return false;
}

void BaseLayer::closeLayer()
{
    clearLayers();
    if(m_parentLayer)
        m_parentLayer->removeChildLayer(this);
    else
        release();
}

//载入过度、其他处理
void BaseLayer::onEnter()
{}

//离开过度、其他处理
void BaseLayer::onLeave()
{}

void BaseLayer::_addChild(BaseLayer *layer_)
{
    m_CCLayer->addChild(layer_->m_CCLayer);
}