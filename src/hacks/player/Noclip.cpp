#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        bool shouldDestroy = !Mod::get()->getSavedValue<bool>("noclip");

        if (shouldDestroy) {
            PlayLayer::destroyPlayer(player, obj);
        }
    }
};