#include "../../hacks.hpp"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/CCTextInputNode.hpp>

class $modify(CCTextInputNode) {
    void updateLabel(gd::string p0) {
        if (hacks::getInstance().isHackEnabled("text-length")) {
            this->m_maxLabelLength = -1;
            CCTextInputNode::updateLabel(p0);
        } else {
            CCTextInputNode::updateLabel(p0);
        }
    }
};