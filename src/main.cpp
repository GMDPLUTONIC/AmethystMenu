#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>
#include <imgui-cocos.hpp>
#include <geode.custom-keybinds/include/Keybinds.hpp>
#include "hacks.hpp"

using namespace geode::prelude;
using namespace keybinds;

static ImFont* openSans = nullptr;

static void createCheckbox(const char* label, bool* enabled, const char* tag) {
    if (ImGui::Checkbox(label, enabled)) {
        log::info("Checkbox '{}' changed to {}", tag, *enabled ? "true" : "false");
        hacks::getInstance().setHackEnabled(tag, *enabled);  // Ensure you are passing *enabled, not enabled
    }
}

static void createIntValue(const char* label, int* value, const char* tag) {
    if (ImGui::InputInt(label, value)) {
        log::info("Int value '{}' changed to {}", tag, *value);
        hacks::getInstance().setHackIntValue(tag, *value);  // Ensure you are passing *value, not value
    }
}

$execute {
    ImGuiCocos::get()
        .setup([]{
            openSans = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "opensans.ttf").string().c_str(), 20.0f);
        })
        .draw([]{
            if (ImGui::Begin("Player")) {
                ImGui::PushFont(openSans);

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

                ImGui::PopFont();
            }
            if (ImGui::Begin("Global")) {
                ImGui::PushFont(openSans);
                
                if (!hacks::getInstance().hackValueExists("speedhack")) {
                    hacks::getInstance().setHackIntValue("speedhack", 1);
                    log::info("Initialized 'speedhack' to 1");
                }
                static int speedHackValue = hacks::getInstance().getIntValue("speedhack");
                createIntValue("Speedhack", &speedHackValue, "speedhack");

                ImGui::PopFont();
            }
            ImGui::End();
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