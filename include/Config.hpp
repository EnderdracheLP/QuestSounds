#pragma once

#include "scotland2/shared/loader.hpp"
#include "rapidjson-macros/shared/macros.hpp"
#include <optional>

#define SOUND_PATH_FORMAT "/sdcard/ModData/{}/Mods/QuestSounds/"
// #define CONFIG_VERSION "SoundsConfig_v3"
// #define CONFIG_VERSION_PRE_1_20 "SoundsConfig_v2"
// #define CONFIG_VERSION_PRE_R "SoundsConfig_v1"
// #define CONFIG_VERSION_LEGACY "Sounds"

#define SOUND_VALUE(name, folder, ...) VALUE_DEFAULT(Sound, name, Sound(#name, soundPath + folder + "/", soundPath + folder + "/" + #name + ".ogg", __VA_ARGS__))
#define SOUND_VALUE_SIMPLE(name, folder) VALUE_DEFAULT(Sound, name, Sound(#name, soundPath + folder + "/", soundPath + folder + "/" + #name + ".ogg"))


namespace QuestSounds {
    inline modloader::ModInfo modInfo = {MOD_ID, VERSION, 0};

    static const std::string soundPath = fmt::format(SOUND_PATH_FORMAT, modloader::get_application_id().c_str());

    DECLARE_JSON_STRUCT(Sound) {
        // Sound(std::string name, std::string folderPath, std::string defaultFilePath, std::optional<float> volumeOffset = std::nullopt, std::optional<float> pitchMaxOffset = std::nullopt, std::optional<float> pitchMinOffset = std::nullopt, std::optional<float> beatOffset = std::nullopt) : 
        //     Name(name), FolderPath(folderPath), DefaultFilePath(defaultFilePath), VolumeOffset(volumeOffset), BeatOffset(beatOffset), PitchMaxOffset(pitchMaxOffset), PitchMinOffset(pitchMinOffset) {}
        Sound() {}
        Sound(std::string name, std::string folderPath, std::string defaultFilePath, std::optional<float> volumeOffset = std::nullopt, std::optional<float> beatOffset = std::nullopt) : 
            Name(name), FolderPath(folderPath), DefaultFilePath(defaultFilePath), VolumeOffset(volumeOffset), BeatOffset(beatOffset) {}

        std::string Name;
        std::string FolderPath;
        std::string DefaultFilePath;
        NAMED_VALUE_DEFAULT(bool, Active, true, "activated");
        NAMED_VALUE_DEFAULT(std::string, FilePath, self->DefaultFilePath, "filepath");
        // TODO: Replace above NAMED_VALUE_CUSTOM use NAMED_VALUE_DEFAULT instead. EDIT: actually not sure it expects a static value
        NAMED_VALUE_OPTIONAL(float, VolumeOffset, "audioVolumeOffset");
        NAMED_VALUE_OPTIONAL(float, BeatOffset, "beatOffSet");
        // VALUE_OPTIONAL(bool, EnablePitchRandomization);
        // VALUE_OPTIONAL(float, PitchMaxOffset);
        // VALUE_OPTIONAL(float, PitchMinOffset);
    };

    // DECLARE_JSON_STRUCT(SoundFiles) {
    //     std::string Name;
    //     std::string FolderPath;
    //     std::vector<std::string> DefaultFilePath;
    //     NAMED_VALUE_DEFAULT(bool, Active, true, "activated");
    //     NAMED_VALUE_OPTIONAL(std::string, FilePath, "filepath");
    //     NAMED_VALUE_DEFAULT(std::vector<std::string>, FilePaths, self->DefaultFilePath, "FilePaths");
    //     NAMED_VALUE_OPTIONAL(float, VolumeOffset, "audioVolumeOffset");
    //     NAMED_VALUE_OPTIONAL(float, BeatOffset, "beatOffSet");
    //     SoundFiles(std::string name, std::string folderPath, std::vector<std::string> defaultFilePath, std::optional<float> volumeOffset = std::nullopt, std::optional<float> pitchMaxOffset = std::nullopt, std::optional<float> pitchMinOffset = std::nullopt, std::optional<float> beatOffset = std::nullopt) : 
    //         Name(name), FolderPath(folderPath), DefaultFilePath(defaultFilePath), VolumeOffset(volumeOffset), BeatOffset(beatOffset) {}
    // };

    DECLARE_JSON_STRUCT(Sounds) {
        SOUND_VALUE(HitSound, "HitSounds", 0, 0.185f);
        SOUND_VALUE(BadHitSound, "BadHitSounds", 0);
        SOUND_VALUE(NoteMissedSound, "NoteMissedSounds", 0);
        SOUND_VALUE_SIMPLE(MenuMusic, "MenuMusic");
        SOUND_VALUE_SIMPLE(MenuClick, "MenuClicks");
        SOUND_VALUE_SIMPLE(Firework, "Fireworks");
        SOUND_VALUE_SIMPLE(LevelCleared, "LevelCleared");
        SOUND_VALUE(LevelFailed, "LevelFailed", 0);
        SOUND_VALUE_SIMPLE(LobbyMusic, "LobbyMusic");
        SOUND_VALUE(BombExplosionSound, "BombExplosionSounds", 0);
    };

    DECLARE_JSON_STRUCT(SoundsConfig) {
        VALUE_DEFAULT(std::string, ConfigVersion, "3.1.0");
        NAMED_VALUE(Sounds, Sounds, NAME_OPTS("SoundsConfig_v3", "SoundsConfig_v2", "SoundsConfig_v1", "Sounds"));
    };
    extern SoundsConfig Config;

    extern std::string& GetConfigPath();
}



// #define CONFIG_SOUND(name, folderpath, active, ...) {name, SoundConfig(name, folderpath, active, __VA_ARGS__)}

// namespace QuestSounds {
//     struct SoundConfig {
//         SoundConfig() = default;
//         SoundConfig(std::string name, std::string folderpath, bool active, std::optional<float> volumeOffset = std::nullopt, std::optional<float> beatOffset = std::nullopt) : FolderPath(folderpath), FilePath(folderpath + name + ".ogg"), Active(active), VolumeOffset(volumeOffset), BeatOffset(beatOffset) {}

//         bool Active = true;
//         std::string FilePath;
//         std::string FolderPath;
//         std::optional<float> VolumeOffset;
//         std::optional<float> BeatOffset;
//     };

//     static const std::string soundPath = fmt::format(SOUND_PATH_FORMAT, modloader::get_application_id().c_str());

//     struct Config {
//         std::map<std::string, SoundConfig> Sounds = {
//             CONFIG_SOUND("MenuMusic", soundPath + "MenuMusic/", true),
//             CONFIG_SOUND("HitSound", soundPath + "HitSounds/", true, 0, 0.185f),
//             CONFIG_SOUND("BadHitSound", soundPath + "BadHitSounds/", true),
//             CONFIG_SOUND("NoteMissedSound", soundPath + "NoteMissedSounds/", true),
//             CONFIG_SOUND("MenuClickSound", soundPath + "MenuClicks/", true),
//             CONFIG_SOUND("FireworkSound", soundPath + "Fireworks/", true),
//             CONFIG_SOUND("LevelClearedSound", soundPath + "LevelCleared/", true),
//             CONFIG_SOUND("LevelFailedSound", soundPath + "LevelFailed/", true),
//             CONFIG_SOUND("LobbyAmbience", soundPath + "LobbyMusic/", true)
//         };
//         // SoundConfig menuMusic = SoundConfig("MenuMusic", soundPath + "MenuMusic/", true, 0, 0);
//         // SoundConfig hitSound = SoundConfig("HitSound", soundPath + "HitSounds/", true, 0, 0);
//         // SoundConfig badHitSound = SoundConfig("BadHitSound", soundPath + "BadHitSounds/", true, 0, 0);
//         // SoundConfig noteMissedSound = SoundConfig("NoteMissedSound", soundPath + "NoteMissedSounds/", true, 0, 0);
//         // SoundConfig menuClickSound = SoundConfig("MenuClickSound", soundPath + "MenuClicks/", true, 0, 0);
//         // SoundConfig fireworkSound = SoundConfig("FireworkSound", soundPath + "Fireworks/", true, 0, 0);
//         // SoundConfig levelClearedSound = SoundConfig("LevelClearedSound", soundPath + "LevelCleared/", true, 0, 0);
//         // SoundConfig levelFailedSound = SoundConfig("LevelFailedSound", soundPath + "LevelFailed/", true, 0, 0);
//         // SoundConfig lobbyAmbience = SoundConfig("LobbyAmbience", soundPath + "LobbyMusic/", true, 0, 0);
//         bool LegacyConfig = false;
//     };

//     extern Config config;
// }



#undef NAMED_VALUE_CUSTOM
#undef SOUND_PATH_FORMAT
#undef SOUND_VALUE_SIMPLE
#undef SOUND_VALUE
// #undef CONFIG_SOUND