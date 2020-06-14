#include <dlfcn.h>
#include "../extern/beatsaber-hook/shared/utils/utils.h"
#include "../extern/beatsaber-hook/shared/config/config-utils.hpp"

static ModInfo modInfo2;

static Configuration& getConfig() {
    static Configuration config(modInfo2);
    return config;
}

static struct Config_t 
{
    bool hitSound_Active = true;
    bool badHitSound_Active = true;
    bool menuMusic_Active = true;
    bool menuClick_Active = true;
    bool firework_Active = true;
    bool levelCleared_Active = true;
    std::string hitSound_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/HitSound.ogg";
    std::string badHitSound_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/BadHitSound.ogg";
    std::string menuMusic_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/MenuMusic.ogg";
    std::string menuClick_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/MenuClick.ogg";
    std::string firework_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/Firework.ogg";
    std::string levelCleared_filepath = "sdcard/Android/data/com.beatgames.beatsaber/files/sounds/LevelCleared.ogg";
} Config;

extern void SaveConfig();
extern bool LoadConfig();