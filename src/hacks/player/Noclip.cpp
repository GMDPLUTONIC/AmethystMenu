#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../../Hacks.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        bool shouldDestroy = !Hacks::getInstance().isHackEnabled("noclip");

        if (shouldDestroy) {
            PlayLayer::destroyPlayer(player, obj);
        }
    }
};
