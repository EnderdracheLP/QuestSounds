#pragma once
#include "main.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"
#define CONFIG_VERSION "Soundsv2"

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

    inline void UpdateBool(ConfigValue& parent, std::string_view soundName, bool& active)
    {
        if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) {
            getLogger().error("Object %s does not exist", std::string(soundName).c_str());
            return;
        }
        ConfigValue value = parent[soundName.data()].GetObject();
        getLogger().debug("Updating Bool: \"%s\":%s to %s", std::string(soundName).c_str(), value["activated"].GetBool(), active);
        //if (!(value.HasMember("activated") && value["activated"].IsBool())) return;
        value["activated"].SetBool(active);
        getConfig().Write();
    }

    inline void UpdatePath(ConfigValue& parent, std::string_view soundName, std::string filepath)
    {
        //if (!parent.HasMember(soundName.data()) || !parent[soundName.data()].IsObject()) return;
        ConfigValue value = parent[soundName.data()].GetObject();
        //std::string test = value["filepath"].GetString();
        getLogger().debug("Updating FilePath: \"%s\":%s to %s", std::string(soundName).c_str(), std::string(value["filepath"].GetString()).c_str(), filepath.c_str());
        //if (!(value.HasMember("filepath") && value["filepath"].IsString())) return;
        value["filepath"].SetString((ConfigValue::StringRefType)filepath.c_str());
        getConfig().Write();
    }


    void SaveConfig();
    bool LoadConfig();
}
