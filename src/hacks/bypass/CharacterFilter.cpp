#include "../../Hacks.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCTextInputNode.hpp>

class $modify(CCTextInputNode) {
    void updateLabel(std::string p0) {
        if (Hacks::getInstance().isHackEnabled("character")) {
            this->setAllowedChars("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789.,-!?:;)(/\\\"\'`*= +-_%[]<>|@&^#{}%$~");
            CCTextInputNode::updateLabel(p0);
        } else {
            CCTextInputNode::updateLabel(p0);
        }
    }
};