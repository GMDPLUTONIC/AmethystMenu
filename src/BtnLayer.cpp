// #ifdef GEODE_IS_ANDROID

#include "BtnLayer.hpp"
#include "Hacks.hpp"
#include <Geode/modify/CCDirector.hpp>
#include <Geode/modify/CCNode.hpp>
#include <Geode/modify/CCMenu.hpp>
#include <Geode/modify/CCSprite.hpp>
#include <Geode/modify/CCTouch.hpp>
#include <Geode/ui/SceneManager.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>
#include <imgui-cocos.hpp>

USING_NS_CC;

void BtnLayer::toggleModMenu(CCObject* pSender) {
    ImGuiCocos::get().toggle();
}

bool BtnLayer::init() {
    if (!CCLayer::init()) {
        return false;
    }

    auto winSize = CCDirector::sharedDirector()->getWinSize();

    btn = CCMenuItemSpriteExtra::create(
        CCSprite::create("modMenuBtn_001.png"_spr),
        this,
        menu_selector(BtnLayer::toggleModMenu)
    );
    btn->setID("amethyst-btn");
    btn->setPosition({
        static_cast<float>((winSize.width * -0.5f) + (btn->getContentWidth() / 2) + (winSize.width * 0.02f)),
        0.f
    });

    auto menu = CCMenu::create();
    menu->setID("amethyst-menu");
    menu->addChild(btn);

    auto node = CCNode::create();
    node->setID("amethyst-node");
    node->addChild(menu);
    SceneManager::get()->keepAcrossScenes(node);

    this->setTouchEnabled(true);
    this->setMouseEnabled(true);

    return true;
}

bool BtnLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (!this->isVisible())
        return false;

    CCPoint touchLocation = touch->getLocation();
    CCPoint btnPos = btn->getPosition();
    CCRect btnRect = btn->boundingBox();
    btnRect.origin += ccp(btn->getContentWidth() / 2, btn->getContentHeight() / 2);

    if (btnRect.containsPoint(btn->convertToNodeSpace(touchLocation))) {
        touchStartedOnButton = true;
        touchStartPos = touchLocation;
        btnStartPos = btn->getPosition();
        doingThing = true;
        return true;
    }
    return false;
}

void BtnLayer::ccTouchMoved(CCTouch* touch, CCEvent* event) {
    if (doingThing && touchStartedOnButton) {
        CCPoint currentTouchPos = touch->getLocation();
        if (btn->getPosition().getDistance(currentTouchPos) > 7.5f) {
            dragging = true;
        }
        if (dragging) {
            CCPoint offset = currentTouchPos - touchStartPos;
            btn->setPosition(btnStartPos + offset);
        }
    }
}

void BtnLayer::ccTouchEnded(CCTouch* touch, CCEvent* event) {
    if (touchStartedOnButton && !dragging) {
        // Button clicked
        toggleModMenu(nullptr);
    }
    touchStartedOnButton = false;
    dragging = false;
    doingThing = false;
}

void BtnLayer::ccTouchCancelled(CCTouch* touch, CCEvent* event) {
    touchStartedOnButton = false;
    dragging = false;
    doingThing = false;
}

// #endif