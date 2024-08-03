#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>
#include <imgui-cocos.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include "hacks.hpp"

using namespace geode::prelude;
using namespace keybinds;

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
        hacks::getInstance().setHackIntValue(tag, *value);  // Ensure you are passing *value, not value
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
                hacks::getInstance().setHackIntValue("speedhack", 1);
                log::info("Initialized 'speedhack' to 1");
            }
            static int speedHackValue = hacks::getInstance().getIntValue("speedhack");
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