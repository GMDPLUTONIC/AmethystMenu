#include <Geode/Geode.hpp>
#include <Geode/modify/CCScheduler.hpp>
#include "Speedhack.hpp"

using namespace geode::prelude;

FMOD::ChannelGroup* masterGroup;

float speedhackLogic(float dt)
{
    #ifndef GEODE_IS_IOS
    if (!masterGroup)
        FMODAudioEngine::sharedEngine()->m_system->getMasterChannelGroup(&masterGroup);
    #endif

    if (!masterGroup)
        return dt;

    float speedMultiplier = hacks::getInstance().getIntValue("speedhack");

    if (speedMultiplier != 1)
    {
        #ifdef GEODE_IS_IOS
        reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::ChannelControl*, float)>(geode::base::get() + 0x4ffec4)(masterGroup, speedMultiplier);
        #else
        masterGroup->setPitch(speedMultiplier);
        #endif
        ColourUtility::update(dt * speedMultiplier);
        return dt * speedMultiplier;
    }

    #ifdef GEODE_IS_IOS
    reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::ChannelControl*, float)>(geode::base::get() + 0x4ffec4)(masterGroup, 1);
    #else
    masterGroup->setPitch(1);
    #endif
    ColourUtility::update(dt);
    return dt;
}

#ifdef GEODE_IS_MACOS

class $modify (CCScheduler)
{
    virtual void update(float dt)
    {
        dt = speedhackLogic(dt);
        CCScheduler::update(dt);
    }
};

#else

void myUpdate(CCScheduler* ins, float dt)
{
    dt = speedhackLogic(dt);
    ins->update(dt);
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(
            geode::addresser::getVirtual(&CCScheduler::update)
        ),
        &myUpdate,
        "cocos2d::CCScheduler::update",
        tulip::hook::TulipConvention::Thiscall
    );
}

#endif

#ifdef GEODE_IS_IOS

FMOD_RESULT FMOD_System_createChannelGroup(FMOD::System* self, const char *name, FMOD::ChannelGroup **channelgroup) {
    auto res = reinterpret_cast<FMOD_RESULT(__cdecl*)(FMOD::System*, const char*, FMOD::ChannelGroup**)>(geode::base::get() + 0x4c8964)(self, name, channelgroup);

    if (!masterGroup)
        masterGroup = *channelgroup;

    log::info("WE HAVE A MASTER GROUP LETS GO!");

    return res;
}

$execute {
    Mod::get()->hook(
        reinterpret_cast<void*>(geode::base::get() + 0x4c8964), 
        &FMOD_System_createChannelGroup, 
        "FMOD::System::createChannelGroup", 
        tulip::hook::TulipConvention::Cdecl 
    );
}

#endif