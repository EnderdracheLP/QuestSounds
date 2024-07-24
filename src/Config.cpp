#include "Config.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"
// TODO: See if I need this cpp file at all
namespace QuestSounds {
    SoundsConfig Config;

    std::string& GetConfigPath() {
        static std::string configPath = Configuration::getConfigFilePath(modInfo);
        return configPath;
    }
}