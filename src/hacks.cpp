#include "hacks.hpp"

bool hacks::isHackEnabled(char* tag) {
    return Mod::get()->getSavedValue<bool>(tag);
}

void hacks::setHackEnabled(char* tag, bool enabled) {
    Mod::get()->setSavedValue<bool>(tag, enabled);
}