#include <Geode/Geode.hpp>

using namespace geode::prelude;

class hacks {
public:
    // Singleton access method
    static hacks& getInstance() {
        static hacks instance;
        return instance;
    }

    bool isHackEnabled(const std::string& tag);
    int getIntValue(const std::string& tag);
    void setHackEnabled(const std::string& tag, bool enabled);
    bool hackValueExists(const std::string& tag);
    void setHackIntValue(const std::string& tag, int value);

private:
    // Private constructor to prevent instantiation
    hacks() = default;
};
