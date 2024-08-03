#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>
#include <imgui-cocos.hpp>
#include "hacks.hpp"

using namespace geode::prelude;

#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;


static ImFont* openSans = nullptr;

static void createCheckbox(const char* label, bool* enabled, const char* tag) {
    if (ImGui::Checkbox(label, enabled)) {
        printf("Checkbox '%s' changed to %s\n", tag, *enabled ? "true" : "false");
    }
}

static void createIntValue(const char* label, int* value, const char* tag) {
    if (ImGui::InputInt(label, value)) {
        printf("Int value '%s' changed to %d\n", tag, *value);
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

                static bool noclipEnabled = hacks::getInstance().isHackEnabled("noclip");
                createCheckbox("Noclip", &noclipEnabled, "noclip");

                static bool iconHackEnabled = hacks::getInstance().isHackEnabled("icon-hack");
                createCheckbox("Icon Hack", &iconHackEnabled, "icon-hack");

                ImGui::PopFont();
            }
			if (ImGui::Begin("Global")) {
                ImGui::PushFont(openSans);

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