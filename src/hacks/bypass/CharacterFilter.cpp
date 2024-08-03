#include "../../hacks.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCTextInputNode.hpp>

class $modify(CCTextInputNode) {
    void updateLabel(gd::string p0) {
        if (hacks::getInstance().isHackEnabled("character")) {
            this->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,-!?:;)(/\\\"\'`*= +-_%[]<>|@&^#{}%$~");
            CCTextInputNode::updateLabel(p0);
        }
    }
};