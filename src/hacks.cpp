#include "hacks.hpp"

bool hacks::isHackEnabled(const std::string& tag) {
    return Mod::get()->getSavedValue<bool>(tag);
}

int hacks::getIntValue(const std::string& tag) {
    return Mod::get()->getSavedValue<int>(tag);
}

void hacks::setHackEnabled(const std::string& tag, bool enabled) {
    Mod::get()->setSavedValue<bool>(tag, enabled);
}

void hacks::setHackIntValue(const std::string& tag, int value) {
    Mod::get()->setSavedValue<int>(tag, value);
}

bool hacks::hackValueExists(const std::string& tag) {
    return Mod::get()->hasSavedValue(tag);
}