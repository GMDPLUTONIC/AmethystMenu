#include <Geode/Geode.hpp>
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class NoclipAccuracy {
public:    
    bool wouldDie = false;
    bool prevDied = false;
    int frames = 0;
    int deaths = 0;
    int deaths_full = 0;
    float totalDelta = 0;
    float prevX = 0;

    void handle_update(PlayLayer* self, float delta) {
        float x = self->m_player1->m_position.x;

        if (x != prevX) {
            frames += 1;
            totalDelta += delta;
        }

        if (wouldDie) {
            wouldDie = false;
            if (totalDelta >= 0.1 && x != prevX) {
                deaths += 1;
            }
            if (totalDelta >= 0.1 && x != prevX && !prevDied)
                deaths_full += 1;
            prevDied = true;
        } else
            prevDied = false;

        prevX = x;
    }

    void handle_reset(PlayLayer* self) {
        prevX = self->m_player1->m_position.x;
        frames = 0;
        totalDelta = 0;
        deaths = 0;
        deaths_full = 0;
        wouldDie = false;
    }

    void handle_death() {
        wouldDie = true;
    }

    float getPercentage() {
        if (frames == 0) {
            return 100.f;
        }

        return (float(frames - deaths) / float(frames)) * 100.f;
    }
};

inline NoclipAccuracy noclip_accuracy;

class $modify(PlayLayer) {
    void destroyPlayer(PlayerObject* player, GameObject* obj) {
        if (!m_fields->anticheat_obj)
            m_fields->anticheat_obj = obj;

        
        if (obj == m_fields->anticheat_obj)
            PlayLayer::destroyPlayer(player, obj);

        bool shouldDestroy = Mod::get()->getSettingValue<bool>("noclip");

        if (shouldDestroy) {
            PlayLayer::destroyPlayer(player, obj);
        }

        noclip_accuracy.handle_death();

        if (hacks::respawn_time_enabled) {
            if (auto* respawnSequence = this->getActionByTag(0x10)) {
                this->stopAction(respawnSequence);
                auto* newSequence = cocos2d::CCSequence::create(
                    cocos2d::CCDelayTime::create(hacks::respawn_time_value),
                    cocos2d::CCCallFunc::create(this, callfunc_selector(PlayLayer::delayedResetLevel)),
                    nullptr
                );
                newSequence->setTag(0x10);
                this->runAction(newSequence);
            }
        }
    }
};