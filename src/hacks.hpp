#include <Geode/Geode.hpp>

using namespace geode::prelude;

class Hacks {
public:
    // Singleton access method
    static Hacks& getInstance() {
        static Hacks instance;
        return instance;
    }

    bool isHackEnabled(const std::string& tag);
    int getIntValue(const std::string& tag);
    float getFloatValue(const std::string& tag);
    void setHackEnabled(const std::string& tag, bool enabled);
    bool hackValueExists(const std::string& tag);
    void setHackIntValue(const std::string& tag, int value);
    void setHackFloatValue(const std::string& tag, float value);

private:
    // Private constructor to prevent instantiation
    Hacks() = default;
};
