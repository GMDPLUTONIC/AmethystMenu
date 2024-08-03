#include "Hacks.hpp"

bool Hacks::isHackEnabled(char* tag) {
    return Mod::get()->getSavedValue<bool>(tag);
}

void Hacks::setHackEnabled(char* tag, bool enabled) {
    Mod::get()->setSavedValue<bool>(tag, enabled)
}