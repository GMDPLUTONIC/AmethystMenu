#include "hacks.hpp"

bool hacks::isHackEnabled(const char* tag) {
    return Mod::get()->getSavedValue<bool>(tag);
}

void hacks::setHackEnabled(const char* tag, bool enabled) {
    Mod::get()->setSavedValue<bool>(tag, enabled);
}
