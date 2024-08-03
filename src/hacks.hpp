#include <Geode/Geode.hpp>

using namespace geode::prelude;

class hacks {
public:
    // Singleton access method
    static hacks& getInstance() {
        static hacks instance;
        return instance;
    }

    bool isHackEnabled(const char* tag);
    void setHackEnabled(const char* tag, bool enabled);

private:
    // Private constructor to prevent instantiation
    hacks() = default;
};
