#include "hacks.hpp"

bool Hacks::isHackEnabled(const std::string& tag) {
    return Mod::get()->getSavedValue<bool>(tag);
}

int Hacks::getIntValue(const std::string& tag) {
    return Mod::get()->getSavedValue<int>(tag);
}

float Hacks::getFloatValue(const std::string& tag) {
    return Mod::get()->getSavedValue<float>(tag);
}

void Hacks::setHackEnabled(const std::string& tag, bool enabled) {
    Mod::get()->setSavedValue<bool>(tag, enabled);
}

void Hacks::setHackIntValue(const std::string& tag, int value) {
    Mod::get()->setSavedValue<int>(tag, value);
}

void Hacks::setHackFloatValue(const std::string& tag, float value) {
    Mod::get()->setSavedValue<float>(tag, value);
}

bool Hacks::hackValueExists(const std::string& tag) {
    return Mod::get()->hasSavedValue(tag);
}