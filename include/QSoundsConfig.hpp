#pragma once
#include "main.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#define SOUND_PATH_FORMAT "/sdcard/ModData/%s/Mods/QuestSounds/"
#define CONFIG_VERSION "SoundsConfig_v3"
#define CONFIG_VERSION_PRE_1_20 "SoundsConfig_v2"
#define CONFIG_VERSION_PRE_R "SoundsConfig_v1"
#define CONFIG_VERSION_LEGACY "Sounds"
#include "UnityEngine/UI/Toggle.hpp"


namespace QSoundsConfig {
    static std::string soundPath = string_format(SOUND_PATH_FORMAT, Modloader::getApplicationId().c_str());;

    static std::string MenuMusicPath = soundPath + "MenuMusic/";
    static std::string HitSoundPath = soundPath + "HitSounds/";
    static std::string BadHitSoundPath = soundPath + "BadHitSounds/";
    static std::string NoteMissedSoundPath = soundPath + "NoteMissedSounds/";
    static std::string MenuClickPath = soundPath + "MenuClicks/";
    static std::string FireworkSoundPath = soundPath + "Fireworks/";
    static std::string LevelClearedPath = soundPath + "LevelCleared/";
    static std::string LevelFailedPath = soundPath + "LevelFailed/";
    static std::string LobbyMusicPath = soundPath + "LobbyMusic/";

    extern bool LegacyConfig;

    //static const ConfigValue soundsConfigParent = getConfig().config[CONFIG_VERSION].GetObject();
    //Config stuff
    struct Config_t
    {
        bool hitSound_Active = true;
        bool badHitSound_Active = true;
        bool noteMissedSound_Active = true;
        bool menuMusic_Active = true;
        bool menuClick_Active = true;
        bool firework_Active = true;
        bool levelCleared_Active = true;
        bool levelFailed_Active = true;
        bool lobbyAmbience_Active = true;

        std::string hitSound_filepath = HitSoundPath + "HitSound.ogg";
        std::string badHitSound_filepath = BadHitSoundPath + "BadHitSound.ogg";
        std::string noteMissedSound_filepath = NoteMissedSoundPath + "NoteMissedSound.ogg";
        std::string menuMusic_filepath = MenuMusicPath + "MenuMusic.ogg";
        std::string menuClick_filepath = MenuClickPath + "MenuClick.ogg";
        std::string firework_filepath = FireworkSoundPath + "Firework.ogg";
        std::string levelCleared_filepath = LevelClearedPath + "LevelCleared.ogg";
        std::string levelFailed_filepath = LevelFailedPath + "LevelFailed.ogg";
        std::string lobbyAmbience_filepath = LobbyMusicPath + "LobbyMusic.ogg";

        float hitSound_audioVolumeOffset = 0;
        float badHitSound_audioVolumeOffset = 0;
        float noteMissedSound_audioVolumeOffset = 0;
        //float menuMusic_audioVolumeOffset = 0;
        //float menuClick_audioVolumeOffset = 0;
        //float firework_audioVolumeOffset = 0;
        //float levelCleared_audioVolumeOffset = 0;
        float levelFailed_audioVolumeOffset = 0;
        //float lobbyAmbience_audioVolumeOffset = 0;

        float hitSound_beatOffSet = 0.185f;
    };
    extern Config_t Config;

    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, ConfigDocument::AllocatorType& allocator);
    void AddChildSound(ConfigValue& parent, std::string_view soundName, bool active, std::string filePath, float beatOffSet, ConfigDocument::AllocatorType& allocator);
    bool ParseSound(bool& active, std::string& filepath, ConfigValue& parent, std::string_view soundName);
    bool ParseSound(bool& active, std::string& filepath, float& beatOffSet, ConfigValue& parent, std::string_view soundName);

    void SaveConfig();
    bool LoadConfig();
}
