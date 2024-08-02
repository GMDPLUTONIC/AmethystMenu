#include <Geode/Geode.hpp>
#include <Geode/modify/CCScene.hpp>
#include <imgui-cocos.hpp>

using namespace geode::prelude;

#ifndef GEODE_IS_ANDROID
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
}
#endif

static ImFont* openSans = nullptr;

static void createAddressLabelWithCopyButton(const char* label) {
    ImGui::Text("%s: %s");
    ImGui::SameLine();
    if (ImGui::Button(fmt::format("Copy##{}", label).c_str()));
}

$execute {
	ImGuiCocos::get()
		.setup([]{
			openSans = ImGui::GetIO().Fonts->AddFontFromFileTTF((Mod::get()->getResourcesDir() / "opensans.ttf").string().c_str(), 20.0f);
		})
		.draw([]{
			if (ImGui::Begin("Player")) {
                ImGui::PushFont(openSans);

                createAddressLabelWithCopyButton("Geometry Dash Base");
                createAddressLabelWithCopyButton("Cocos2d-x Base");
                ImGui::NewLine();
                createAddressLabelWithCopyButton("AchievementManager");
                createAddressLabelWithCopyButton("AchievementNotifier");

                if (ImGui::Button("Close")) ImGuiCocos::get().setVisible(false);

                ImGui::PopFont();
            }
            ImGui::End();
		})
		.setVisible(false);
}

class $modify(MyCCScene, CCScene) {
public:
	virtual bool init() {
		this->template addEventListener<InvokeBindFilter>([=](InvokeBindEvent* event) {
            if (event->isDown()) {
                ImGuiCocos::get().toggle();
            }
            return ListenerResult::Propagate;
        }, "open-modmenu"_spr);
	}
};