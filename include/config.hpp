#include "modloader/shared/modloader.hpp"
#include "beatsaber-hook/shared/config/config-utils.hpp"

#define SOUND_PATH_FORMAT "/sdcard/Android/data/%s/files/sounds/"

std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());
class SoundConfig
{
        static ModInfo modInfo;
        
        static Configuration& getConfig() {
            static Configuration config(modInfo);
            return config;
        }
        
        static struct Config_t 
        {
            std::string folderPath = soundPath;
            std::string hitSoundName = "HitSound";
            std::string badHitSoundName = "BadHitSound";
            std::string menuMusicName = "MenuMusic";
            std::string menuClickName = "MenuClick";
            std::string fireworkName = "Firework";
            std::string levelClearedName = "levelCleared";
        } Config;        
};