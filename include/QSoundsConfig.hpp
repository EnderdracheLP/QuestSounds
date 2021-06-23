#pragma once
#include "main.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"
#define CONFIG_VERSION "SoundsConfig_v2"

namespace QSoundsConfig {
    static std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());;

    //static const ConfigValue soundsConfigParent = getConfig().config[CONFIG_VERSION].GetObject();
    //Config stuff
    struct Config_t
    {
        bool hitSound_Active = true;
        bool badHitSound_Active = true;
        bool menuMusic_Active = true;
        bool menuClick_Active = true;
        bool firework_Active = true;
        bool levelCleared_Active = true;
        bool lobbyAmbience_Active = true;
        std::string hitSound_filepath = soundPath + "HitSound.ogg";
        std::string badHitSound_filepath = soundPath + "BadHitSound.ogg";
        std::string menuMusic_filepath = soundPath + "MenuMusic.ogg";
        std::string menuClick_filepath = soundPath + "MenuClick.ogg";
        std::string firework_filepath = soundPath + "Firework.ogg";
        std::string levelCleared_filepath = soundPath + "LevelCleared.ogg";
        std::string lobbyAmbience_filepath = soundPath + "LobbyMusic.ogg";
        //float hitSound_Volume       = 1;
        //float badHitSound_Volume    = 1;
        //float menuClick_Volume      = 1;
        //float menuMusic_Volume      = 1;
        //float firework_Volume       = 1;
        //float levelCleared_Volume   = 1;
        //float lobbyAmbience_Volume  = 1;
    };
    extern Config_t Config;

    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, ConfigDocument::AllocatorType& allocator);
    bool ParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName);

    void SaveConfig();
    bool LoadConfig();
}
