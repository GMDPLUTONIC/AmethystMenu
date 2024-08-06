#include <Geode/Geode.hpp>
#include <Geode/ui/SceneManager.hpp>
#include <Geode/modify/CCScene.hpp>
#include <imgui-cocos.hpp>
#include "hacks.hpp"

using namespace geode::prelude;

#ifdef GEODE_IS_WINDOWS

#include <geode.custom-keybinds/include/Keybinds.hpp>

using namespace keybinds;

$execute {
    BindManager::get()->registerBindable({
        "open-modmenu"_spr,
        "Open Amethyst Menu",
        "Opens the mod menu",
        { Keybind::create(KEY_Tab, Modifier::None) },
        "Amethyst Menu"
    });
    new EventListener([=](InvokeBindEvent* event) {
        if (event->isDown())
            ImGuiCocos::get().toggle();
        return ListenerResult::Propagate;
    }, InvokeBindFilter(nullptr, "open-modmenu"_spr));
}

#endif

static ImFont* openSans = nullptr;

// Default padding
static float PADDING = 0.0f;

// Function to create a checkbox with logging
static void createCheckbox(const char* label, bool* enabled, const char* tag) {
    if (ImGui::Checkbox(label, enabled)) {
        log::info("Checkbox '{}' changed to {}", tag, *enabled ? "true" : "false");
        hacks::getInstance().setHackEnabled(tag, *enabled);  // Ensure you are passing *enabled, not enabled
    }
}

// Function to create an integer value input with logging
static void createIntValue(const char* label, int* value, const char* tag) {
    if (ImGui::InputInt(label, value)) {
        log::info("Int value '{}' changed to {}", tag, *value);
        hacks::getInstance().setHackIntValue(tag, *value);
    }
}

// Function to create an float value input with logging
static void createFloatValue(const char* label, float* value, const char* tag) {
    if (ImGui::InputFloat(label, value)) {
        log::info("Float value '{}' changed to {}", tag, *value);
        hacks::getInstance().setHackFloatValue(tag, *value);
    }
}

// Draw windows with initial positions and sizes
static void DrawWrappedWindows() {
    // Compute padding based on screen width at runtime
    auto screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    PADDING = screenSize.width / 50.f;

    // List of window names and their associated content functions
    struct WindowInfo {
        std::string name;
        std::function<void()> drawContent;
        ImVec2 initialPos;
        ImVec2 initialSize;
    };

    std::vector<WindowInfo> windows = {
        { "Player", []{
            if (!hacks::getInstance().hackValueExists("noclip")) {
                hacks::getInstance().setHackEnabled("noclip", false);
                log::info("Initialized 'noclip' to false");
            }
            static bool noclipEnabled = hacks::getInstance().isHackEnabled("noclip");
            createCheckbox("Noclip", &noclipEnabled, "noclip");

            if (!hacks::getInstance().hackValueExists("icon-hack")) {
                hacks::getInstance().setHackEnabled("icon-hack", false);
                log::info("Initialized 'icon-hack' to false");
            }
            static bool iconHackEnabled = hacks::getInstance().isHackEnabled("icon-hack");
            createCheckbox("Icon Hack", &iconHackEnabled, "icon-hack");

            if (!hacks::getInstance().hackValueExists("color-hack")) {
                hacks::getInstance().setHackEnabled("color-hack", false);
                log::info("Initialized 'color-hack' to false");
            }
            static bool colorHackEnabled = hacks::getInstance().isHackEnabled("color-hack");
            createCheckbox("Color Hack", &colorHackEnabled, "color-hack");
        }},
        { "Global", []{
            if (!hacks::getInstance().hackValueExists("speedhack")) {
                hacks::getInstance().setHackFloatValue("speedhack", 1.f);
                log::info("Initialized 'speedhack' to 1.f");
            }
            static int speedHackValue = hacks::getInstance().getFloatValue("speedhack");
            createIntValue("Speedhack", &speedHackValue, "speedhack");
        }},
        { "Bypass", []{
            if (!hacks::getInstance().hackValueExists("character")) {
                hacks::getInstance().setHackEnabled("character", false);
                log::info("Initialized 'character' to false");
            }
            static bool characterFilterEnabled = hacks::getInstance().isHackEnabled("character");
            createCheckbox("Character Filter", &characterFilterEnabled, "character");

            if (!hacks::getInstance().hackValueExists("text-length")) {
                hacks::getInstance().setHackEnabled("text-length", false);
                log::info("Initialized 'text-length' to false");
            }
            static bool textLengthEnabled = hacks::getInstance().isHackEnabled("text-length");
            createCheckbox("Text Length", &textLengthEnabled, "text-length");
        }}
    };

    float windowWidth = screenSize.width / 8.5f; // Set initial width
    float windowHeight = screenSize.height / 6.f; // Set initial height
    float xPos = PADDING;
    float yPos = PADDING;

    for (auto& window : windows) {
        ImVec2 windowSize = ImVec2(windowWidth, windowHeight);
        ImVec2 windowPos = ImVec2(xPos, yPos);

        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
        ImGui::Begin(window.name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        window.drawContent(); // Draw the content of the window

        ImGui::End();

        xPos += windowWidth + PADDING; // Update the x position for the next window

        // Move to the next row if windows exceed screen width
        if (xPos + windowWidth > screenSize.width - PADDING) {
            xPos = PADDING; // Reset to left side
            yPos += windowHeight + PADDING; // Move down for next row
        }
    }
}

$execute {
    ImGuiCocos::get()
        .setup([]{
            openSans = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "opensans.ttf").string().c_str(), 20.0f);
        })
        .draw([]{
            ImGui::PushFont(openSans);

            // Draw the wrapped windows
            DrawWrappedWindows();

            ImGui::PopFont();
        })
        .setVisible(false);
}

#ifdef GEODE_IS_ANDROID

class BtnLayer : public CCLayer {
private:
    CCMenuItemSpriteExtra* btn;
    CCSprite* btnOverlay;
    CCMenu* menu;
    bool dragging = false;
    bool doingThing = false;
    bool touchStartedOnButton = false;
    CCPoint touchStartPos;
    CCPoint btnStartPos;
    CCPoint position;
    CCPoint initialPosition;

public:
    void toggleModMenu(CCObject* pSender) {
        ImGuiCocos::get().toggle();
    }

    virtual bool init() override {
        if (!CCLayer::init())
            return false;

        this->setTouchEnabled(true); // Enable touch for the layer
        this->setTouchMode(kCCTouchesOneByOne); // Ensure single touch mode

        auto winSize = CCDirector::sharedDirector()->getWinSize();

        // Correct initialization of initialPosition
        float posX = (winSize.width * -0.5f) + (btn->getContentWidth() / 2) + (winSize.width * 0.02f);
        initialPosition = CCPoint(posX, 0.f);
        position = initialPosition;

        menu = CCMenu::create();
        menu->setPosition(position);
        btn = CCMenuItemSpriteExtra::create(
            CCSprite::create("modMenuBtn_001.png"_spr),
            this,
            menu_selector(BtnLayer::toggleModMenu)
        );

        btnOverlay = CCSprite::create("qolmodButtonOverlay.png"_spr);
        btn->addChild(btnOverlay);

        menu->addChild(btn);
        this->addChild(menu);

        return true;
    }

    virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event) override {
        if (!CCLayer::ccTouchBegan(touch, event))
            return false;

        if (!this->isVisible())
            return false;

        doingThing = false;
        dragging = false;

        auto point = btn->boundingBox();
        point.origin += ccp(btn->getContentWidth() / 2, btn->getContentHeight() / 2);

        if (point.containsPoint(btn->convertToNodeSpace(touch->getLocation()))) {
            auto scale = CCEaseInOut::create(CCScaleTo::create(0.1f, 0.8f), 2);
            scale->setTag(69);

            btn->runAction(scale);
            doingThing = true;
        }

        return doingThing;
    }

    virtual void ccTouchMoved(CCTouch* touch, CCEvent* event) override {
        if (doingThing && !btn->getActionByTag(69)) {
            if (btn->getPosition().getDistance(touch->getLocation()) > 7.5f) {
                dragging = true;
            }

            if (dragging) {
                position = touch->getLocation();
                menu->setPosition(position);
            }
        }
        CCLayer::ccTouchMoved(touch, event);
    }

    virtual void ccTouchEnded(CCTouch* touch, CCEvent* event) override {
        if (doingThing) {
            if (!dragging)
                toggleModMenu(nullptr);

            auto scale = CCEaseBackOut::create(CCScaleTo::create(0.35f, 1));
            scale->setTag(69);

            btn->runAction(scale);
            doingThing = false;

            // Save the position if needed
            // Mod::get()->setSavedValue("posX", position.x);
            // Mod::get()->setSavedValue("posY", position.y);
        }
    }

    virtual void ccTouchCancelled(CCTouch* touch, CCEvent* event) override {
        if (doingThing) {
            auto scale = CCEaseBackOut::create(CCScaleTo::create(0.35f, 1));
            scale->setTag(69);

            btn->runAction(scale);
            doingThing = false;
        }
    }

    CREATE_FUNC(BtnLayer);
};

class $modify (CCScene) {
    void onEnter() override {
        CCScene::onEnter();
        auto btnLayer = BtnLayer::create();
        this->addChild(btnLayer, 9999); // Ensure the button layer is always on top
    }
};

#endif