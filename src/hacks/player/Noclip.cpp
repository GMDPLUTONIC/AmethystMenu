#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include "../../hacks.hpp"

using namespace geode::prelude;

class $modify(MyPlayLayer, PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        // Use the singleton instance to call the isHackEnabled method
        bool shouldDestroy = !hacks::getInstance().isHackEnabled("noclip");

        if (shouldDestroy) {
            PlayLayer::destroyPlayer(player, obj);
        }
    }
};
