#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "../../Hacks.hpp"

// come back to this later

/*

class $modify(CCScheduler) {
    void update(float dt) {
        auto speedhackValue = Hacks::getInstance().getIntValue("speedhack");
        if (speedhack == 1) return CCScheduler::update(dt);
        speedhack->value.floatValue = std::max(speedhack->value.floatValue, 0.01f);
        if (speedhack->value.floatValue == 1.0F && !current_macro.isEnabled) return CCScheduler::update(dt);
        float speedHackValue = stof(Utils::setPrecision(speedhack->value.floatValue, 3));
        dt *= speedHackValue; // def not copied!
        if (current_macro.isEnabled && PlayLayer::get() != nullptr) { 
            float dt2 = (1.f / current_macro.framerate);
            auto startTime = std::chrono::high_resolution_clock::now();
            int mult = static_cast<int>((dt + current_macro.loaf)/dt2);  
            using namespace std::literals;
            for (int i = 0; i < mult; i++) {
                CCScheduler::update(dt2);
                if (std::chrono::high_resolution_clock::now() - startTime > 33.333ms) {
                    mult = i + 1;
                    break;
                }
            }
            current_macro.loaf += (dt - dt2 * mult); 
            if (Hacks::isHackEnabled("Playback") && !PlayLayer::get()->m_levelSettings->m_platformerMode) {
                syncCooldown++;
                if (syncCooldown >= 20 && current_macro.loaf > 1) {
                    syncCooldown = 0;
                }
            }
            return;
        }
        CCScheduler::update(dt);
    }
};

*/