#include <Geode/Geode.hpp>
#include <Geode/ui/SceneManager.hpp>
#include <Geode/modify/CCScene.hpp>
#include <Geode/modify/CCTouchDispatcher.hpp>
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
static float PADDING = 0.0f;

// Function to get the theme color from settings
static ccColor3B getThemeColor() {
    std::string colorHex = Mod::get()->getSettingValue<std::string>("theme");
    int r, g, b;

    // Ensure the hex string is valid and parsed correctly
    if (sscanf(colorHex.c_str(), "#%02x%02x%02x", &r, &g, &b) != 3) {
        log::error("Invalid color format: {}", colorHex);
        return { 128, 0, 128 }; // Fallback to purple if parsing fails
    }

    // Log parsed RGB values for debugging
    log::info("Parsed color - R: {}, G: {}, B: {}", r, g, b);

    // Ensure the values are in the range of 0-255 and convert to ccColor3B
    return { static_cast<uint8_t>(r), static_cast<uint8_t>(g), static_cast<uint8_t>(b) };
}

static void createCheckbox(const char* label, bool* enabled, const char* tag) {
    if (ImGui::Checkbox(label, enabled)) {
        log::info("Checkbox '{}' changed to {}", tag, *enabled ? "true" : "false");
        Hacks::getInstance().setHackEnabled(tag, *enabled);
    }
}

static void createIntValue(const char* label, int* value, const char* tag) {
    if (ImGui::InputInt(label, value)) {
        log::info("Int value '{}' changed to {}", tag, *value);
        Hacks::getInstance().setHackIntValue(tag, *value);
    }
}

static void createFloatValue(const char* label, float* value, const char* tag) {
    if (ImGui::InputFloat(label, value)) {
        log::info("Float value '{}' changed to {}", tag, *value);
        Hacks::getInstance().setHackFloatValue(tag, *value);
    }
}

static void DrawWrappedWindows() {
    auto screenSize = CCDirector::sharedDirector()->getWinSizeInPixels();
    PADDING = screenSize.width / 50.f;

    struct WindowInfo {
        std::string name;
        std::function<void()> drawContent;
    };

    std::vector<WindowInfo> windows = {
        { "Player", []{
            if (!Hacks::getInstance().hackValueExists("noclip")) {
                Hacks::getInstance().setHackEnabled("noclip", false);
                log::info("Initialized 'noclip' to false");
            }
            static bool noclipEnabled = Hacks::getInstance().isHackEnabled("noclip");
            createCheckbox("Noclip", &noclipEnabled, "noclip");

            if (!Hacks::getInstance().hackValueExists("icon-hack")) {
                Hacks::getInstance().setHackEnabled("icon-hack", false);
                log::info("Initialized 'icon-hack' to false");
            }
            static bool iconHackEnabled = Hacks::getInstance().isHackEnabled("icon-hack");
            createCheckbox("Icon Hack", &iconHackEnabled, "icon-hack");

            if (!Hacks::getInstance().hackValueExists("color-hack")) {
                Hacks::getInstance().setHackEnabled("color-hack", false);
                log::info("Initialized 'color-hack' to false");
            }
            static bool colorHackEnabled = Hacks::getInstance().isHackEnabled("color-hack");
            createCheckbox("Color Hack", &colorHackEnabled, "color-hack");
        }},
        { "Global", []{
            if (!Hacks::getInstance().hackValueExists("speedhack")) {
                Hacks::getInstance().setHackFloatValue("speedhack", 1.f);
                log::info("Initialized 'speedhack' to 1.f");
            }
        }},
        { "Bypass", []{
            if (!Hacks::getInstance().hackValueExists("character")) {
                Hacks::getInstance().setHackEnabled("character", false);
                log::info("Initialized 'character' to false");
            }
            static bool characterFilterEnabled = Hacks::getInstance().isHackEnabled("character");
            createCheckbox("Character Filter", &characterFilterEnabled, "character");

            if (!Hacks::getInstance().hackValueExists("text-length")) {
                Hacks::getInstance().setHackEnabled("text-length", false);
                log::info("Initialized 'text-length' to false");
            }
            static bool textLengthEnabled = Hacks::getInstance().isHackEnabled("text-length");
            createCheckbox("Text Length", &textLengthEnabled, "text-length");
        }},
        { "April Fools", []{
            if (Mod::get()->getSettingValue<bool>("april-fools")) {
                if (!Hacks::getInstance().hackValueExists("april-fools-usernames")) {
                    Hacks::getInstance().setHackEnabled("april-fools-usernames", false);
                    log::info("Initialized 'april-fools-usernames' to false");
                }
            }
        }},
    };

    float windowWidth = screenSize.width / 8.5f;
    float windowHeight = screenSize.height / 6.f;
    float xPos = PADDING;
    float yPos = PADDING;

    for (auto& window : windows) {
        ImVec2 windowSize = ImVec2(windowWidth, windowHeight);
        ImVec2 windowPos = ImVec2(xPos, yPos);

        // Push custom title bar colors
        auto themeColor = getThemeColor();
        ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(themeColor.r / 255.0f, themeColor.g / 255.0f, themeColor.b / 255.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(themeColor.r / 255.0f, themeColor.g / 255.0f, themeColor.b / 255.0f, 1.0f));

        ImGui::SetNextWindowPos(windowPos, ImGuiCond_Once);
        ImGui::SetNextWindowSize(windowSize, ImGuiCond_Once);
        ImGui::Begin(window.name.c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

        window.drawContent();

        ImGui::End();

        ImGui::PopStyleColor(2); // Pop both title bar colors

        xPos += windowWidth + PADDING;

        if (xPos + windowWidth > screenSize.width - PADDING) {
            xPos = PADDING;
            yPos += windowHeight + PADDING;
        }
    }
}

$execute {
    ImGuiCocos::get()
        .setup([]{
            openSans = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "opensans.ttf").string().c_str(), 20.0f * Mod::get()->getSettingValue<double>("font-size"));
        })
        .draw([]{
            ImGui::PushFont(openSans);
            DrawWrappedWindows();
            ImGui::PopFont();
        })
        .setVisible(false);
}
