#pragma once

#ifndef BTN_LAYER_HPP
#define BTN_LAYER_HPP

#include <cocos2d.h>
#include <Geode/ui/SceneManager.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

USING_NS_CC;

class BtnLayer : public CCLayer {
private:
    CCMenuItemSpriteExtra* btn;
    bool dragging = false;
    bool doingThing = false;
    bool touchStartedOnButton = false;
    CCPoint touchStartPos;
    CCPoint btnStartPos;

public:
    virtual bool init() override;
    void toggleModMenu(CCObject* pSender);

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override;
    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override;

    CREATE_FUNC(BtnLayer);
};

#endif
